//
//========================================================================
//
#define		UNICODE
#define		_UNICODE

#include	<windows.h>
#include	<stdio.h>
#include	<tchar.h>
#include	<winioctl.h>
#include	"..\RTIOCTL.H"
#include	"RTCINTER.H"
#include	"SrvFunc.h"
#include	"RVDSTRU.H"
#include	"VirtualizerSDK.h"

#define		RICH_DISKLESS
//#define		OEM_SERVERPARK
//#define		OEM_HP
//
//========================================================================
//

#define		WINNT_SYS_NAME		L"\\\\.\\RTKCPFXP"
#define		DRIVER_NAME			L"RTKCPFXP"
#define		SYS_FILE_NAME		L"RTKCPFXP.SYS"

static		HANDLE	g_hDriver = INVALID_HANDLE_VALUE ; 	
static		HANDLE	g_hMutex = NULL ;
static		BOOL	g_bLoadDriver = FALSE ;
//
//========================================================================
//

BOOL APIENTRY DllMain( HANDLE hModule , 
                       DWORD  dwReason , 
                       LPVOID lpReserved )
{
    return TRUE ;
}

BOOL __stdcall IsWin7( )
{
	OSVERSIONINFO	ov ;

	memset( &ov , 0 , sizeof(ov) ) ;
	ov.dwOSVersionInfoSize = sizeof(ov) ;
	
	GetVersionEx( &ov ) ;

	return (( ov.dwMajorVersion == 6 ) ? TRUE : FALSE ) ;
}

#define STATUS_IMAGE_ALREADY_LOADED		0xC000010E

DWORD __stdcall RTW32_Initialize( VOID )
{
	DWORD		dwStatus = 0 ;
	WCHAR		wszSysPath[MAX_PATH] ;
	PWCHAR		pwExt = NULL ;
	
	__try
	{
		g_hMutex = ::CreateMutex( NULL , FALSE , L"_RTKC_XP_MUTEX_" ) ;

		if( g_hMutex != NULL)
		{
			dwStatus = GetLastError( ) ;
			if( dwStatus == ERROR_ALREADY_EXISTS )			
			{
				__leave ;
			}
			dwStatus = 0 ;
		}
		else
		{
			dwStatus = GetLastError() ;
			__leave ;
		}
		GetModuleFileName( NULL , wszSysPath , MAX_PATH ) ;
			
		pwExt = wcsrchr( wszSysPath , L'\\' ) ;
		
		* ( pwExt + 1 ) = 0 ;
		wcscat( wszSysPath , SYS_FILE_NAME ) ;

		dwStatus = RTC_LoadDriver( DRIVER_NAME , wszSysPath ) ;
		if( 0 != dwStatus )
		{
			if( STATUS_IMAGE_ALREADY_LOADED != dwStatus )
			{
				__leave ;
			}
			dwStatus = 0 ;
		}
		else
		{
			g_bLoadDriver = TRUE ;
		}
		g_hDriver = CreateFile( 
						WINNT_SYS_NAME ,
						GENERIC_READ | GENERIC_WRITE ,
						0 ,
						NULL ,
						OPEN_EXISTING ,
						FILE_ATTRIBUTE_NORMAL ,
						NULL ) ;
		if( INVALID_HANDLE_VALUE == g_hDriver )
		{
			dwStatus = GetLastError( ) ;			
		}
	}
	__finally
	{
		if( dwStatus != 0 )
		{
			if( g_hMutex != NULL )
			{
				CloseHandle( g_hMutex ) ;
				g_hMutex = NULL ;
			}
			if( g_bLoadDriver )
			{
				RTC_UnloadDriver( DRIVER_NAME ) ;
				g_bLoadDriver = FALSE ;
			}
		}		
	}
	return dwStatus ;
}

VOID  __stdcall RTW32_Destroy( VOID )
{
	if( INVALID_HANDLE_VALUE != g_hDriver )
	{
		CloseHandle( g_hDriver ) ;
		g_hDriver = INVALID_HANDLE_VALUE ;
	}
	if( g_bLoadDriver )	
	{
		RTC_UnloadDriver( DRIVER_NAME  ) ;
		g_bLoadDriver = FALSE ;
	}
	if( NULL != g_hMutex )
	{
		CloseHandle( g_hMutex ) ;
		g_hMutex = NULL ;
	}
}

DWORD __stdcall RTW32_CopyFile( 
							IN PWCHAR	pwSourceFileName , 
							IN PWCHAR	pwDestFileName )
{
	WCHAR	wszFileNameBuffer[520] ;
	DWORD	dwBufferSize = 0 ;
	DWORD	dwFileNameLen = 0 ;
	DWORD	dwStatus = 0 ;
	DWORD	dwRetBytes = 0 ;

	dwFileNameLen = wcslen( pwSourceFileName ) + 1 ; 
	wcsncpy( wszFileNameBuffer , pwSourceFileName , dwFileNameLen ) ;
	dwBufferSize += dwFileNameLen ;

	dwFileNameLen = wcslen( pwDestFileName ) + 1 ;
	wcsncpy( wszFileNameBuffer + dwBufferSize , pwDestFileName , dwFileNameLen ) ;
	dwBufferSize += dwFileNameLen ;
				
	dwBufferSize *= sizeof( WCHAR ) ;
	__try
	{
		if( !DeviceIoControl( 
						g_hDriver ,
						IOCTL_RTKCPF_COPYFILE ,
						wszFileNameBuffer , dwBufferSize , 
						&dwStatus , sizeof( DWORD ) ,
						&dwRetBytes , 
						NULL ))
		{
			dwStatus = GetLastError() ;
			__leave ;
		}
	}
	__finally
	{

	}

	return dwStatus ;
}

//
//========================================================================

DWORD __stdcall INTER_CheckIfRTVDisk( 
								IN TCHAR	szDriveLetter , 
								IN BOOL *	pbRTVdisk )
{
	DWORD	dwStatus = 0 ;
	TCHAR	tszDeviceName[] = _T("\\\\.\\A:") ;
	tszDeviceName[4] = szDriveLetter ;
	HANDLE	hDevice = CreateFile( 
							tszDeviceName , 
							GENERIC_READ | GENERIC_WRITE ,
							FILE_SHARE_READ | FILE_SHARE_WRITE , 
							NULL , 
							OPEN_EXISTING , 
							0 , 
							NULL ) ;
	if( INVALID_HANDLE_VALUE == hDevice )
	{
		dwStatus = GetLastError() ;
		return dwStatus ;
	}

	{
		STORAGE_PROPERTY_QUERY		PropertyQuery ;
		PSTORAGE_DEVICE_DESCRIPTOR	pAdapterDesc = NULL ;
		BYTE						btOutputBuffer[256] ;
		DWORD						dwRetBytes ;
		PCHAR						lpszVendorID = NULL ;
		PCHAR						lpszProductID = NULL ;

		PropertyQuery.PropertyId = StorageDeviceProperty ;
		PropertyQuery.QueryType = PropertyStandardQuery ;

		if( !DeviceIoControl( 
						hDevice , 
						IOCTL_STORAGE_QUERY_PROPERTY , 
						&PropertyQuery , 
						sizeof( STORAGE_PROPERTY_QUERY ) ,
						btOutputBuffer , 
						256 , 
						&dwRetBytes , 
						NULL ))
		{
			dwStatus = GetLastError() ;
		}
		else
		{
			pAdapterDesc = ( PSTORAGE_DEVICE_DESCRIPTOR )btOutputBuffer ;

			* pbRTVdisk = FALSE ;

			if( pAdapterDesc->DeviceType == DIRECT_ACCESS_DEVICE )
			{
				lpszVendorID = ( PCHAR )( btOutputBuffer + pAdapterDesc->VendorIdOffset ) ;
				lpszProductID = ( PCHAR )( btOutputBuffer + pAdapterDesc->ProductIdOffset ) ;

//				printf( "Drive : %C VendorId: %s\n" , szDriveLetter , lpszVendorID ) ;
			
#ifdef	RICH_DISKLESS	
#define	RTVDISK_VENDOR_ID	"richdisk"				
				if( !_strnicmp( lpszVendorID , RTVDISK_VENDOR_ID , 8 ))
#endif

#ifdef  OEM_SERVERPARK
#define SERVERPARK_ID	"Serverpark"
				if( !_strnicmp( lpszProductID , SERVERPARK_ID , 10 ))
#endif

#ifdef  OEM_HP
#define HP_ID	"RCS Virtual"
				if( !_strnicmp( lpszProductID , HP_ID , 11 ))
#endif
				{
					* pbRTVdisk = TRUE ;
				}
			}
		}
	}

	CloseHandle( hDevice ) ;

	return dwStatus ;
}

DWORD __stdcall RTW32_EnumRTVDiskList(
								OUT PDWORD	pdwVDiskMask )
{
	DWORD	dwStatus = 0 ;
	DWORD	i ;
	TCHAR	cDrive ;
	BOOL	bRTVdisk = FALSE ;
	DWORD	dwDriveMask = GetLogicalDrives( ) ;
	DWORD	dwVDiskMask = 0 ;

	for( i = 2 ; i < 26 ; i ++ ) 
	{ 
		if( dwDriveMask & ( 1 << i )) 
		{ 
			cDrive = ( TCHAR )i + 'A' ; 
			if( ( 0 == INTER_CheckIfRTVDisk( cDrive , &bRTVdisk )) && bRTVdisk )
			{
				dwVDiskMask |= ( 1 << i ) ;
			}
		} 
	} 

	* pdwVDiskMask = dwVDiskMask ;

	return dwStatus ;
}

DWORD __stdcall RTW32_SetNicPnp( )
{
	DWORD	dwStatus = 0 ;
	DWORD	dwRetBytes ;
	if( !DeviceIoControl( 
					g_hDriver ,
					IOCTL_RTKCPF_SETNICPNP ,
					NULL , 0 , 
					NULL , 0 ,
					&dwRetBytes , 
					NULL ))
	{
		dwStatus = GetLastError() ;
	}

	return dwStatus ;
}

//
//========================================================================
//

DWORD __stdcall INTER_GetDiskIndex( 
								IN WCHAR	DriveLetter , 
								OUT PDWORD	pdwDiskIndex )
{
	DWORD		dwStatus = 0 ;
	HANDLE		hVolume = NULL ;
	WCHAR		szVolumeName[] = L"\\\\.\\A:" ;
	DWORD		dwRetBytes ;
	BOOL		bRet ;
	STORAGE_DEVICE_NUMBER sdn = { 0  } ;	

	szVolumeName[4] = DriveLetter ;

	hVolume = CreateFile(
					szVolumeName,
					GENERIC_READ | GENERIC_WRITE ,
					FILE_SHARE_READ | FILE_SHARE_WRITE ,
					NULL ,
					OPEN_EXISTING ,
					FILE_FLAG_NO_BUFFERING,
					NULL ) ;
	
	if( INVALID_HANDLE_VALUE != hVolume )
	{
		bRet = DeviceIoControl(
					hVolume , 
					IOCTL_STORAGE_GET_DEVICE_NUMBER , 
					NULL , 0 ,
					&sdn , sizeof( sdn ) ,
					&dwRetBytes , NULL ) ;
		if( bRet )
		{
			* pdwDiskIndex = sdn.DeviceNumber ;			
		}
		else
		{
			dwStatus = GetLastError() ;
		}

		CloseHandle( hVolume ) ;
	}
	else
	{
		dwStatus = GetLastError() ;
	}

	return dwStatus ;
}
//
//========================================================================
//

DWORD __stdcall INTER_GetDiskOffsetInfo( 
								IN WCHAR	DriveLetter , 
								OUT PLARGE_INTEGER	lpDriveOffInDisk )
{
	DWORD		dwStatus = 0 ;
	HANDLE		hVolume = NULL ;
	WCHAR		szVolumeName[] = L"\\\\.\\A:" ;
	DWORD		dwRetBytes ;
	BOOL		bRet ;
	PARTITION_INFORMATION pi = { 0 } ;

	szVolumeName[4] = DriveLetter ;

	hVolume = CreateFile(
					szVolumeName,
					GENERIC_READ | GENERIC_WRITE ,
					FILE_SHARE_READ | FILE_SHARE_WRITE ,
					NULL ,
					OPEN_EXISTING ,
					FILE_FLAG_NO_BUFFERING,
					NULL ) ;
	
	if( INVALID_HANDLE_VALUE != hVolume )
	{
		bRet = DeviceIoControl(
					hVolume , 
					IOCTL_DISK_GET_PARTITION_INFO , 
					NULL , 0 ,
					&pi , sizeof( pi ) ,
					&dwRetBytes , NULL ) ;
		if( bRet )
		{
			* lpDriveOffInDisk = pi.StartingOffset ;
		}
		else
		{
			dwStatus = GetLastError() ;
		}

		CloseHandle( hVolume ) ;
	}
	else
	{
		dwStatus = GetLastError() ;
	}

	return dwStatus ;
}

//
//========================================================================
//
#define		SECTOR_SIZE			512
#define		DISK_SIGNATURE_OFF	0x1B8
#define		DEST_DISK_OFF		0x1B4
#define		RICH_FLAG			0x9A9B9C9D

#pragma pack( 1 )

typedef struct _VISTA_BCD_CONFIG_ENTRY
{
	LARGE_INTEGER	PartitionOffset ;
	LARGE_INTEGER	Unknown1 ;
	LARGE_INTEGER	Unknown2 ;
	DWORD			dwDiskSignature ;

} VISTA_BCD_CONFIG_ENTRY , * PVISTA_BCD_CONFIG_ENTRY ;

#pragma pack( )

#define REMAIN_ENTRY_LEN	(sizeof( VISTA_BCD_CONFIG_ENTRY ) - sizeof( DWORD ))

int __stdcall memfind( 
					IN PBYTE	lpSrcMem , 
					IN int	iSrcMemSize , 
					IN PBYTE	lpFindMem , 
					IN int	iFindMemSize )
{
    int i, j, p;
	int	iTotal = iSrcMemSize - iFindMemSize ;
	int iPos = -1 ;
    
    for(i = 0; i < iTotal + 1; i++)
    {
        p = i;
        for(j = 0 ; j < iFindMemSize ; j++ )
        {
            if( lpSrcMem[p] != lpFindMem[j] )
			{
				break;
			}
            
            p ++ ;
        }

        if( j == iFindMemSize )
		{
			iPos = p - j ;
			break;
		}
    }

    return iPos ;
}

int __stdcall memfind2( 
					IN PBYTE	lpSrcMem , 
					IN int	iSrcMemSize , 
					IN PBYTE	lpFindMem , 
					IN int	iFindMemSize )
{
	PBYTE	lpCur = NULL , lpEnd = NULL ;
	int iPos = -1 ;

	lpCur = lpSrcMem ;
	lpEnd = lpSrcMem + iSrcMemSize - iFindMemSize + 1 ;

	while( lpCur < lpEnd )
	{
		if( !memcmp( lpCur , lpFindMem , iFindMemSize ))
		{
			iPos = lpCur - lpSrcMem ;
			break ;
		}	
		else
		{
			lpCur ++ ;
		}
	}

	return iPos ;
}

DWORD __stdcall RT_RemoveReadOnly( 
							IN LPCTSTR	lpszFileName )
{
	DWORD	dwFileAttribute = GetFileAttributes( lpszFileName ) ;
	if( dwFileAttribute & FILE_ATTRIBUTE_READONLY )
	{
		dwFileAttribute &= ~FILE_ATTRIBUTE_READONLY ;
		if( !SetFileAttributes( lpszFileName , dwFileAttribute ))
		{
			return GetLastError() ;
		}
	}

	return 0 ;
}

//
//========================================================================
//

DWORD __stdcall INTER_EditBCDFileData(
								IN PBYTE	lpBCDFileDataBuf , 
								IN DWORD	dwBCDFileSize ,
								IN DWORD	dwOldDiskSignature , 
								IN PLARGE_INTEGER	lpOldDriveOffset ,
								IN DWORD	dwNewDiskSignature , 
								IN PLARGE_INTEGER	lpNewDriveOffset )
{
	DWORD	dwStatus = 0 ;
	int		iDiskSignaturePos = 0 ;
	PBYTE	lpSearchBuf = lpBCDFileDataBuf ;
	int		iSearchSize = ( int )dwBCDFileSize ;
	PVISTA_BCD_CONFIG_ENTRY	lpBCDEntry = NULL ;

	VIRTUALIZER_START ;

	for( ; ; )
	{
		iDiskSignaturePos = memfind( 
							lpSearchBuf , iSearchSize , 
							( PBYTE )&dwOldDiskSignature , sizeof( DWORD )) ;
		if( -1 == iDiskSignaturePos )
		{
			break ;
		}

		if( iDiskSignaturePos > REMAIN_ENTRY_LEN )
		{
			lpBCDEntry = ( PVISTA_BCD_CONFIG_ENTRY )( lpSearchBuf + iDiskSignaturePos - REMAIN_ENTRY_LEN ) ;
			if( lpBCDEntry->PartitionOffset.QuadPart == lpOldDriveOffset->QuadPart )
			{
				lpBCDEntry->PartitionOffset.QuadPart = lpNewDriveOffset->QuadPart ;
				lpBCDEntry->dwDiskSignature = dwNewDiskSignature ;
			}			
		}

		lpSearchBuf += iDiskSignaturePos + sizeof( DWORD ) ;
		iSearchSize -= iDiskSignaturePos + sizeof( DWORD ) ;
	}
	
	VIRTUALIZER_END ;

	return dwStatus ;
}

DWORD __stdcall INTER_EditRichVDiskBCD( 
								IN WCHAR	wRichDrive , 
								IN DWORD	dwOldDiskSignature , 
								IN PLARGE_INTEGER	lpOldDriveOffset ,
								IN DWORD	dwNewDiskSignature , 
								IN PLARGE_INTEGER	lpNewDriveOffset )
{
	DWORD	dwStatus = 0 ;
	WCHAR	wszBCDFilePath[128] ;
	HANDLE	hFile = INVALID_HANDLE_VALUE ;	
	DWORD	dwFileSize = 0 ;
	HANDLE	hFileMapping = NULL ;
	PBYTE	lpFileImage = NULL ;

	VIRTUALIZER_START ;

	wszBCDFilePath[0] = wRichDrive ;
	wszBCDFilePath[1] = L':' ;
	wszBCDFilePath[2] = L'\\' ;
	wszBCDFilePath[3] = L'b' ;
	wszBCDFilePath[4] = L'o' ;
	wszBCDFilePath[5] = L'o' ;
	wszBCDFilePath[6] = L't' ;
	wszBCDFilePath[7] = L'\\' ;
	wszBCDFilePath[8] = L'b' ;
	wszBCDFilePath[9] = L'c' ;
	wszBCDFilePath[10] = L'd' ;
	wszBCDFilePath[11] = 0 ;


	do
	{
		hFile = CreateFile(
			wszBCDFilePath , 
			GENERIC_READ | GENERIC_WRITE , 
			FILE_SHARE_READ , 
			NULL ,
			OPEN_EXISTING , 
			0 , NULL ) ;
		if( INVALID_HANDLE_VALUE == hFile )
		{
			dwStatus = GetLastError() ;

			if( ERROR_ACCESS_DENIED == dwStatus )
			{
				RT_RemoveReadOnly( wszBCDFilePath ) ;

				hFile = CreateFile(
					wszBCDFilePath , 
					GENERIC_READ | GENERIC_WRITE , 
					FILE_SHARE_READ , 
					NULL ,
					OPEN_EXISTING , 
					0 , NULL ) ;
				if( INVALID_HANDLE_VALUE == hFile )
				{
					dwStatus = GetLastError() ;
				}
			}
		}

		if( 0 != dwStatus )
		{
			break ;
		}

		dwFileSize = GetFileSize( hFile , NULL ) ;
		if( INVALID_FILE_SIZE == dwFileSize )
		{
			dwStatus = GetLastError() ;		
			break ;
		}

		hFileMapping = CreateFileMapping( 
								hFile , 
								NULL ,
								PAGE_READWRITE ,
								0 , 0 , 
								NULL ) ;
		if( NULL == hFileMapping )
		{
			dwStatus = GetLastError() ;
			break ;
		}

		lpFileImage = ( PBYTE )MapViewOfFile( 
								hFileMapping ,
								FILE_MAP_ALL_ACCESS ,
								0 , 0 , 0 ) ;
		if( NULL == lpFileImage )
		{
			dwStatus = GetLastError() ;
			break ;
		}

		INTER_EditBCDFileData( 
				lpFileImage , dwFileSize , 
				dwOldDiskSignature , lpOldDriveOffset , 
				dwNewDiskSignature , lpNewDriveOffset ) ;

		FlushViewOfFile( lpFileImage , 0 ) ;

	} while( FALSE ) ;

	if( NULL != lpFileImage )
	{
		UnmapViewOfFile( lpFileImage ) ;
	}
	if( NULL != hFileMapping )
	{
		CloseHandle( hFileMapping ) ;
	}

	if( INVALID_HANDLE_VALUE != hFile )
	{
		CloseHandle( hFile ) ;
	}

	VIRTUALIZER_END ;

	return	dwStatus ;
}

DWORD __stdcall INTER_GetDiskSignature( 
								IN  DWORD	dwDiskIndex ,
								OUT PDWORD	lpdwDiskSignature )
{
	DWORD	dwStatus = 0 ;
	DWORD	dwRetBytes ;
	BYTE	btSectorBuffer[SECTOR_SIZE] ;
	WCHAR	szPhysicalDrive[MAX_PATH]  ;
	HANDLE	hPhysicalDrive = INVALID_HANDLE_VALUE ;

	VIRTUALIZER_START ;

	do
	{
		swprintf( szPhysicalDrive , L"\\\\.\\PhysicalDrive%d" , dwDiskIndex ) ;

		hPhysicalDrive = CreateFile( 
								szPhysicalDrive , 
								GENERIC_READ | GENERIC_WRITE ,
								FILE_SHARE_READ | FILE_SHARE_WRITE , 
								NULL , OPEN_EXISTING , 0 , NULL ) ;

		if( INVALID_HANDLE_VALUE == hPhysicalDrive )
		{
			dwStatus = GetLastError() ;
			break ;
		}

		if( !ReadFile( 
					hPhysicalDrive , btSectorBuffer , SECTOR_SIZE , &dwRetBytes , NULL ))
		{
			dwStatus = GetLastError() ;
		}

		CloseHandle( hPhysicalDrive ) ;

		if( 0 != dwStatus )
		{
			break ;
		}

		* lpdwDiskSignature = * ( PDWORD )( btSectorBuffer + DISK_SIGNATURE_OFF ) ;				

	} while( FALSE ) ;

	VIRTUALIZER_END ;

	return dwStatus ;
}
									   
DWORD __stdcall INTER_SetVDiskVistaFlag( 								
								IN  DWORD	dwDiskIndex , 
								IN  DWORD	dwDiskSignature )
{
	DWORD	dwStatus = 0 ;
	DWORD	dwRetBytes ;
	BYTE	btSectorBuffer[SECTOR_SIZE] ;
	WCHAR	szPhysicalDrive[MAX_PATH]  ;	
	HANDLE	hPhysicalDrive = INVALID_HANDLE_VALUE ;

	VIRTUALIZER_START ;

	do
	{	
		swprintf( szPhysicalDrive , L"\\\\.\\PhysicalDrive%d" , dwDiskIndex ) ;

		hPhysicalDrive = CreateFile( 
								szPhysicalDrive , 
								GENERIC_READ | GENERIC_WRITE ,
								FILE_SHARE_READ | FILE_SHARE_WRITE , 
								NULL , OPEN_EXISTING , 0 , NULL ) ;

		if( INVALID_HANDLE_VALUE == hPhysicalDrive )
		{
			dwStatus = GetLastError() ;
			break ;
		}

		if( !ReadFile( 
					hPhysicalDrive , btSectorBuffer , SECTOR_SIZE , &dwRetBytes , NULL ))
		{
			dwStatus = GetLastError() ;
		}
		else
		{
			* ( PDWORD )( btSectorBuffer + DEST_DISK_OFF ) = dwDiskSignature ;
			
			SetFilePointer( hPhysicalDrive , 0 , NULL , FILE_BEGIN ) ;

			if( !WriteFile( hPhysicalDrive , btSectorBuffer , SECTOR_SIZE , &dwRetBytes , NULL ))
			{
				dwStatus = GetLastError() ;
			}
		}	

		CloseHandle( hPhysicalDrive ) ;

	} while( FALSE ) ;

	VIRTUALIZER_END ;

	return dwStatus ;
}

DWORD __stdcall SUB_ModifyBootStatus( 
							IN WCHAR	wRichDrive )
{
	DWORD	dwStatus = 0 ;
	WCHAR	wszBootstatFilePath[] = L"C:\\Windows\\BOOTSTAT.DAT";
	HANDLE	hFile = INVALID_HANDLE_VALUE ;	
	DWORD	dwFileSize = 0 ;
	HANDLE	hFileMapping = NULL ;
	PBYTE	lpFileImage = NULL ;

	VIRTUALIZER_START ;

	wszBootstatFilePath[0] = wRichDrive ;

	do
	{
		hFile = CreateFile(
			wszBootstatFilePath , 
			GENERIC_READ | GENERIC_WRITE , 
			FILE_SHARE_READ , 
			NULL ,
			OPEN_EXISTING , 
			0 , NULL ) ;
		if( INVALID_HANDLE_VALUE == hFile )
		{
			dwStatus = GetLastError() ;

			if( ERROR_ACCESS_DENIED == dwStatus )
			{
				RT_RemoveReadOnly( wszBootstatFilePath ) ;

				hFile = CreateFile(
					wszBootstatFilePath , 
					GENERIC_READ | GENERIC_WRITE , 
					FILE_SHARE_READ , 
					NULL ,
					OPEN_EXISTING , 
					0 , NULL ) ;
				if( INVALID_HANDLE_VALUE == hFile )
				{
					dwStatus = GetLastError() ;
				}
			}
		}

		if( 0 != dwStatus )
		{
			break ;
		}

		dwFileSize = GetFileSize( hFile , NULL ) ;
		if( INVALID_FILE_SIZE == dwFileSize )
		{
			dwStatus = GetLastError() ;		
			break ;
		}

		if( dwFileSize < 12 )
		{
			dwStatus = ERROR_BAD_FORMAT ;
			break ;
		}

		hFileMapping = CreateFileMapping( 
								hFile , 
								NULL ,
								PAGE_READWRITE ,
								0 , 0 , 
								NULL ) ;
		if( NULL == hFileMapping )
		{
			dwStatus = GetLastError() ;
			break ;
		}

		lpFileImage = ( PBYTE )MapViewOfFile( 
								hFileMapping ,
								FILE_MAP_ALL_ACCESS ,
								0 , 0 , 0 ) ;
		if( NULL == lpFileImage )
		{
			dwStatus = GetLastError() ;
			break ;
		}

		lpFileImage[10] = 1 ; // 正常启动
		lpFileImage[11] = 1 ; // 正常关机

		FlushViewOfFile( lpFileImage , 0 ) ;

	} while( FALSE ) ;

	if( NULL != lpFileImage )
	{
		UnmapViewOfFile( lpFileImage ) ;
	}
	if( NULL != hFileMapping )
	{
		CloseHandle( hFileMapping ) ;
	}

	if( INVALID_HANDLE_VALUE != hFile )
	{
		CloseHandle( hFile ) ;
	}

	VIRTUALIZER_END ;

	return	dwStatus ;
}

DWORD __stdcall MyDeleteFile( 
							IN LPCTSTR lpFilePathName )
{
	DWORD	dwStatus = 0 ;
	if( !DeleteFile( lpFilePathName ))
	{
		dwStatus = GetLastError() ;
		if( ERROR_ACCESS_DENIED == dwStatus )
		{
			DWORD dwFileAttributes = GetFileAttributes( lpFilePathName ) ;
			dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY ;
			SetFileAttributes( lpFilePathName , dwFileAttributes ) ;

			dwStatus = 0 ;

			if( !DeleteFile( lpFilePathName ))
			{
				dwStatus = GetLastError() ;
			}
		}
	}

	return dwStatus ;
}

DWORD __stdcall MyRemoveDirectory( 
								  IN LPCTSTR lpszDirPath )
{
	DWORD	dwStatus = 0 ;
	if( !RemoveDirectory( lpszDirPath ))
	{
		dwStatus = GetLastError() ;
		if( ERROR_ACCESS_DENIED == dwStatus )
		{
			DWORD dwFileAttributes = GetFileAttributes( lpszDirPath ) ;
			dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY ;
			SetFileAttributes( lpszDirPath , dwFileAttributes ) ;

			dwStatus = 0 ;

			if( !RemoveDirectory( lpszDirPath ))
			{
				dwStatus = GetLastError() ;
			}
		}
	}

	return dwStatus ;	
}

__inline BOOL __stdcall IsDots( LPCTSTR lpszFileName )
{
	BOOL	bResult = FALSE ;
	if( lpszFileName[0] == _T('.'))
	{
		if( lpszFileName[1] == _T('\0') ||
			( lpszFileName[1] == _T('.') &&
			lpszFileName[2] == _T('\0')))
		{
			bResult = TRUE;
		}
	}
	return bResult ;
}

BOOL __stdcall DXPE_DeleteDirTree(					 
						LPCTSTR	lpszScanDirPath )
{	
	DWORD				dwStatus = 0 ;
	WIN32_FIND_DATA		FindData;
	HANDLE				hFind = NULL ;
	BOOL				bFinished = FALSE;	

	int					iLen = 0 ;
	TCHAR				szFullPathName[MAX_PATH] ;
	
	_tcscpy( szFullPathName , lpszScanDirPath ) ;
	_tcscat( szFullPathName , _T( "\\*.*" )) ;
	
	ZeroMemory( &FindData,sizeof(FindData)) ;
	
	hFind = FindFirstFile( szFullPathName,&FindData ) ;
	 
	if( INVALID_HANDLE_VALUE == hFind ) 
	{
		return GetLastError() ;
	}
	
	while( !bFinished )
	{
		if( !IsDots( FindData.cFileName ))
		{			
			_stprintf( szFullPathName , _T("%s\\%s") , lpszScanDirPath , FindData.cFileName ) ;

			if( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				dwStatus = DXPE_DeleteDirTree( szFullPathName ) ;
			}
			else
			{				
				dwStatus = MyDeleteFile( szFullPathName ) ;
			}
		}
		
		if( !FindNextFile( hFind , &FindData ))
		{
			if(ERROR_NO_MORE_FILES == GetLastError())
			{				
				bFinished = TRUE;
			}
		}
	}

	FindClose( hFind ) ;

	if( _tcslen( lpszScanDirPath ) > 3 )
	{
		MyRemoveDirectory( lpszScanDirPath ) ;
	}

	return 0 ;
}

DWORD __stdcall SUB_RemoveWin7RecycleBin( 
								IN WCHAR wRichDrive )
{
	DWORD	dwStatus = 0 ;
	WCHAR	wszRecycleBinPath[] = L"C:\\$Recycle.Bin" ;

	wszRecycleBinPath[0] = wRichDrive ;

	dwStatus = DXPE_DeleteDirTree( wszRecycleBinPath ) ;

	return dwStatus ;
}

BOOL __stdcall MyCopyDirectory(					 
							   WCHAR *	lpszSrcDirPath , 
							   WCHAR *  lpszDestDirPath )
{	
	DWORD	dwStatus = 0 ;
	WIN32_FIND_DATA FindData;
	HANDLE hFind = NULL ;
	BOOL bFinished = FALSE;	

	int		iLen = 0 ;
	WCHAR	szFullPathName[MAX_PATH] ;
	WCHAR	szDestFullPathName[MAX_PATH] ;

	wcscpy( szFullPathName , lpszSrcDirPath ) ;
	wcscat( szFullPathName , L"\\*.*" ) ;

	ZeroMemory(&FindData,sizeof(FindData));

	hFind = FindFirstFile( szFullPathName,&FindData ) ;

	if( INVALID_HANDLE_VALUE == hFind ) 
	{
		return GetLastError() ;
	}

	CreateDirectory( lpszDestDirPath , NULL ) ;

	while( !bFinished )
	{
		if( !IsDots( FindData.cFileName ))
		{
			swprintf( szFullPathName , L"%ws\\%ws" , lpszSrcDirPath , FindData.cFileName ) ;
			swprintf( szDestFullPathName , L"%ws\\%ws" , lpszDestDirPath , FindData.cFileName ) ;

			if( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{				
				dwStatus = MyCopyDirectory( szFullPathName , szDestFullPathName ) ;
			}
			else
			{				
				if( !CopyFile( szFullPathName , szDestFullPathName , FALSE ))
				{
					dwStatus = GetLastError() ;
					if( ERROR_SHARING_VIOLATION == dwStatus )
					{
						dwStatus = RTW32_CopyFile( szFullPathName , szDestFullPathName ) ;
					}					
				}				
			}
		}

		if( !FindNextFile( hFind , &FindData ))
		{
			if(ERROR_NO_MORE_FILES == GetLastError())
			{				
				bFinished = TRUE;
			}
		}
	}

	FindClose( hFind ) ;

	return 0 ;
}

BOOL __stdcall SUB_IsExistHiddenPartition(
						IN WCHAR wSysDrive )
{
	WCHAR wszBootmgrFilePath[] = L"c:\\bootmgr";
	WCHAR wszBcdFilePath[] = L"c:\\boot\\bcd" ;

	wszBootmgrFilePath[0] = wSysDrive ;
	wszBcdFilePath[0] = wSysDrive ;

	if( ( INVALID_FILE_ATTRIBUTES == GetFileAttributes( wszBootmgrFilePath )) &&
		( INVALID_FILE_ATTRIBUTES == GetFileAttributes( wszBcdFilePath )) )
	{
		return TRUE ;
	}

	return FALSE ;
}

DWORD __stdcall SUB_CopySystemBootFiles( 
							IN WCHAR wRichDrive )
{
	DWORD	dwStatus = 0 ;
	WCHAR	wszBootmgrFilePath[] = L"\\\\.\\Harddisk0Partition1\\bootmgr"; //L"\\\\.\\HarddiskVolume1\\bootmgr";
	WCHAR	wszDestFilePath[] = L"c:\\bootmgr" ;

	WCHAR	wszBcdFilePath[] = L"\\\\.\\Harddisk0Partition1\\boot";
	WCHAR	wszDestFilePath2[] = L"c:\\boot" ;

	wszDestFilePath[0] = wRichDrive ;
	wszDestFilePath2[0] = wRichDrive ;

	if( !CopyFile( wszBootmgrFilePath , wszDestFilePath , FALSE ))
	{
		dwStatus = GetLastError() ;
	}

	if( 0 == dwStatus )
	{
		dwStatus = MyCopyDirectory( wszBcdFilePath , wszDestFilePath2 ) ;
	}

	return dwStatus ;
}

DWORD __stdcall RTW32_QueryInfo( 
						IN CHAR	chSystemDrive , 
						IN CHAR	chDestDrive )
{
	DWORD	dwStatus = 0 ;
	DWORD	dwSysDiskIndex = 0 ;
	DWORD	dwDestDiskIndex = 0 ;
	WCHAR	wSysDrive = 0 ;
	WCHAR	wDestDrive = 0 ;

	LARGE_INTEGER	SrcOffset ;
	LARGE_INTEGER	DestOffset ;

	DWORD			dwSrcDiskSignature = 0 ;
	DWORD			dwDestDiskSignature = 0 ;

	if( !IsWin7( ))
	{
		return dwStatus ;
	}

	wSysDrive = ( WCHAR )chSystemDrive ;
	wDestDrive = ( WCHAR )chDestDrive ;

	if( SUB_IsExistHiddenPartition( wSysDrive ))
	{
		dwStatus = SUB_CopySystemBootFiles( wDestDrive ) ;
		if( 0 != dwStatus )
		{
			return dwStatus ;
		}
	}

	dwStatus = INTER_GetDiskIndex( wSysDrive , &dwSysDiskIndex ) ;
	if( 0 != dwStatus )
	{
		return dwStatus ;
	}

	dwStatus = INTER_GetDiskOffsetInfo( wSysDrive , &SrcOffset ) ;
	if( 0 != dwStatus )
	{
		return dwStatus ;
	}

	dwStatus = INTER_GetDiskSignature( dwSysDiskIndex , &dwSrcDiskSignature ) ;
	if( 0 != dwStatus )
	{
		return dwStatus ;
	}

	dwStatus = INTER_GetDiskIndex( wDestDrive , &dwDestDiskIndex ) ;
	if( 0 != dwStatus )
	{
		return dwStatus ;
	}

	dwStatus = INTER_GetDiskOffsetInfo( wDestDrive , &DestOffset ) ;
	if( 0 != dwStatus )
	{
		return dwStatus ;
	}

	dwStatus = INTER_GetDiskSignature( dwDestDiskIndex , &dwDestDiskSignature ) ;
	if( 0 != dwStatus )
	{
		return dwStatus ;
	}

	dwStatus = INTER_SetVDiskVistaFlag( dwDestDiskIndex , dwDestDiskSignature ) ;
	if( 0 != dwStatus )
	{
		return dwStatus ;
	}

	dwStatus = SUB_ModifyBootStatus( wDestDrive ) ;
	if( 0 != dwStatus )
	{

	}

	dwStatus = SUB_RemoveWin7RecycleBin( wDestDrive ) ;
	if( 0 != dwStatus )
	{

	}

	dwStatus = INTER_EditRichVDiskBCD( 
						wDestDrive , 
						dwSrcDiskSignature , &SrcOffset , 
						dwDestDiskSignature , &DestOffset ) ;
	if( 0 != dwStatus )
	{
	}

	

	return dwStatus ;
}
//
//========================================================================
//

