//
//========================================================================
//
#define		UNICODE
#define		_UNICODE

#include	<windows.h>
#include	<stdio.h>
#include	"SrvFunc.h"
//
//========================================================================
//
#define NTAPI	__stdcall
typedef LONG	NTSTATUS ;

typedef struct _UNICODE_STRING 
{
	USHORT  Length ;
	USHORT  MaximumLength ;
	PWSTR  Buffer ;

} UNICODE_STRING , *PUNICODE_STRING ;

typedef NTSTATUS (NTAPI * PZWLOADDRIVER )(
								IN PUNICODE_STRING DriverServiceName ) ;

typedef NTSTATUS (NTAPI * PZWUNLOADDRIVER )(
								IN PUNICODE_STRING DriverServiceName ) ;

//
//========================================================================
//
PZWLOADDRIVER						ZwLoadDriver = NULL ;
PZWUNLOADDRIVER						ZwUnloadDriver = NULL ;

#define		NTDLL_NAME					L"NTDLL.DLL"
#define		FUNC_ZWLOADDRIVER_NAME		"ZwLoadDriver"
#define		FUNC_ZWUNLOADDRIVER_NAME	"ZwUnloadDriver"
//
//========================================================================
//
#define		BM_ERR_GETPROC_FAILED		0x1234

DWORD	__stdcall	BM_GetNtdllFuncAddr( )
{
	DWORD		dwStatus = 0 ;
	HMODULE		hModule = LoadLibrary( NTDLL_NAME ) ;
	
	if( NULL == hModule ) 
	{
		dwStatus = GetLastError() ;
		return dwStatus ;
	}
	
	ZwLoadDriver = ( PZWLOADDRIVER )GetProcAddress( 
										hModule , FUNC_ZWLOADDRIVER_NAME ) ;
	ZwUnloadDriver = ( PZWUNLOADDRIVER )GetProcAddress( 
										hModule , FUNC_ZWUNLOADDRIVER_NAME ) ;

	if( NULL == ZwLoadDriver ||
		NULL == ZwUnloadDriver )
	{
		dwStatus = BM_ERR_GETPROC_FAILED ;
	}
	
	FreeLibrary( hModule ) ;

	return dwStatus ;
}
//
//========================================================================
//
#define		aSeloaddriverpr		L"SeLoadDriverPrivilege"
#define		aServiceNameFmt		L"System\\CurrentControlSet\\Services\\%s"	

#define		aType				L"Type"
#define		aStart				L"Start"
#define		aImagePath			L"ImagePath"
#define		aErrorControl		L"ErrorControl"
#define		aImageFmt			L"\\??\\%s"
#define		aRegistryMachin		L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\%s"
#define		aSSecurity			L"%s\\Security"
#define		aSEnum				L"%s\\Enum"

//
//========================================================================
//

BOOL __stdcall AdjustPrivileges( LPCTSTR privilege )
{
	HANDLE				hToken = NULL ;
	TOKEN_PRIVILEGES	tkp ;  
	BOOL				bResult = FALSE ;
 
	__try
	{
		if( !OpenProcessToken(
				GetCurrentProcess(), 
				TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ))
		{
			__leave ;
		}

		if( !LookupPrivilegeValue(
					NULL, privilege, 
					&tkp.Privileges[0].Luid ))
		{
			__leave ;
		}
 
		tkp.PrivilegeCount = 1 ;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED ; 
 
		bResult = AdjustTokenPrivileges(
							hToken, FALSE, &tkp, 0, 
							(PTOKEN_PRIVILEGES) NULL, 0 ) ;
	}
	__finally
	{
		if( NULL != hToken )
		{
			CloseHandle( hToken ) ;
		}
	}
	

    return bResult ;
}
	
//
//========================================================================
//
DWORD __stdcall	RTC_LoadDriver( 
						IN PWCHAR	pwServiceName , 
						IN PWCHAR	pwDriverPath )
{
	UNICODE_STRING		UniStr ;

	WCHAR		wszMainKey[MAX_PATH] ;
	WCHAR		wszTmpPath[MAX_PATH] ;
	HKEY		hSubKey ;
	DWORD		dwType = SERVICE_KERNEL_DRIVER ;
	DWORD		dwStart = SERVICE_DEMAND_START ;
	DWORD		dwErrControl = SERVICE_ERROR_NORMAL ;
	LONG		lErrCode = ERROR_SUCCESS ;

	__try
	{
		lErrCode = BM_GetNtdllFuncAddr( ) ;
		if( 0 != lErrCode )
		{
			__leave ;
		}
		
		if( !AdjustPrivileges( SE_LOAD_DRIVER_NAME )) //aSeloaddriverpr
		{
			lErrCode = GetLastError() ;
			__leave ;
		}
		swprintf( wszMainKey , aServiceNameFmt , pwServiceName ) ;
		
		lErrCode = RegCreateKey( HKEY_LOCAL_MACHINE , wszMainKey , &hSubKey ) ;
		if( ERROR_SUCCESS != lErrCode )
		{
			__leave ;
		}

		lErrCode = RegSetValueEx(
						hSubKey , aType , 0 , REG_DWORD ,
						(PBYTE )&dwType , sizeof( DWORD )) ;
		if( ERROR_SUCCESS != lErrCode )
		{
			__leave ;
		}
		lErrCode = RegSetValueEx( 
						hSubKey , aStart , 0,  REG_DWORD , 
						(PBYTE )&dwStart , sizeof( DWORD )) ;
		if( ERROR_SUCCESS != lErrCode )
		{
			__leave ;
		}

		lErrCode = RegSetValueEx( 
						hSubKey , 
						aErrorControl , 
						0 ,
						REG_DWORD , 
						(PBYTE )&dwErrControl ,
						sizeof( DWORD )) ;
		if( ERROR_SUCCESS != lErrCode )
		{
			__leave ;
		}
		
		swprintf( wszTmpPath , aImageFmt , pwDriverPath ) ;

		lErrCode = RegSetValueEx( 
						hSubKey , 
						aImagePath , 
						0 ,
						REG_SZ , 
						( PBYTE )wszTmpPath ,
						( wcslen( wszTmpPath ) + 1 ) * sizeof( WCHAR )) ;
		if( ERROR_SUCCESS != lErrCode )
		{
			__leave ;
		}

		RegCloseKey( hSubKey ) ;
		
		swprintf( wszTmpPath, aRegistryMachin , pwServiceName ) ;
		UniStr.Buffer = wszTmpPath ;
		UniStr.Length = wcslen( wszTmpPath ) * sizeof( WCHAR ) ;
		UniStr.MaximumLength = UniStr.Length + sizeof( UNICODE_NULL ) ;

		lErrCode = ZwLoadDriver( &UniStr ) ;
	}
	__finally
	{

	}
		
	return lErrCode ;
}

DWORD __stdcall	RTC_UnloadDriver( 
					IN PWCHAR	pwServiceName )
{
	UNICODE_STRING		UniStr ;
	WCHAR	wszTmpPath[MAX_PATH] ;
	swprintf( wszTmpPath, aRegistryMachin , pwServiceName ) ;
	UniStr.Buffer = wszTmpPath ;
	UniStr.Length = wcslen( wszTmpPath ) * sizeof( WCHAR ) ;
	UniStr.MaximumLength = UniStr.Length + sizeof( UNICODE_NULL ) ;

	return ZwUnloadDriver( &UniStr ) ;
}

//
//========================================================================
//
