//
//========================================================================
//
#define		_WINNT

#include	"ntddk.h"
#include	"stdio.h"
#include	"stdarg.h"
#include	"TypeInfo.h"
#include	"RTKCPFXP.H"
#include	"RTIOCTL.H"
//
//========================================================================
//
static		PVOID			g_pCPFBuffer = NULL ;
#define		CP_BUFFER_SIZE	(1024*1024) //1M
//
//========================================================================
//
NTSTATUS RTKCF_CreateClose(
				IN PDEVICE_OBJECT DeviceObject ,
				IN PIRP Irp )
{
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0 ;

	IoCompleteRequest( Irp , IO_NO_INCREMENT ) ;

	return STATUS_SUCCESS ;
}

VOID RTKCF_Unload(
				IN PDRIVER_OBJECT DriverObject )
{
	UNICODE_STRING DosDeviceName ;

	RtlInitUnicodeString( &DosDeviceName , DOSDEVICENAME ) ;
	
	IoDeleteDevice( DriverObject->DeviceObject ) ;

	IoDeleteSymbolicLink( &DosDeviceName ) ;

	if( NULL != g_pCPFBuffer )
	{
		ExFreePool( g_pCPFBuffer ) ;
	}
}

//
//========================================================================
//
// Copy Source File to Dest file 
// 
//========================================================================
//
NTSTATUS RTCALL INTER_CopyFile( 
							IN PWCHAR	pwSourceFile , 
							IN PWCHAR	pwDestFile )
{
	NTSTATUS			ntStatus = STATUS_SUCCESS ;
	HANDLE				hSourceFile = NULL , hDestFile = NULL ;
	OBJECT_ATTRIBUTES	ObjectAttribute ;
	UNICODE_STRING		UniFileName ;
	IO_STATUS_BLOCK		IoStatusBlock ;
	FILE_STANDARD_INFORMATION	fsi ;
	LARGE_INTEGER		BytesOffset = { 0 } ;
	WCHAR				wszFileName[280] ;

	DWORD				i = 0 , j = 0 ;
	DWORD				dwBlockCount = 0 ;
	DWORD				dwRemainBytes = 0 ;
	DWORD				dwBufferSize = 0 ;

	//
	// 读源文件
	//
		if( pwSourceFile[1] == L':' )
	{
		swprintf( wszFileName , L"\\??\\%ws" , pwSourceFile ) ;
	}
	else
	{
		swprintf( wszFileName , L"\\??\\%ws" , pwSourceFile + 4 ) ;
	}

	RtlInitUnicodeString( &UniFileName , wszFileName ) ;

	InitializeObjectAttributes(
						&ObjectAttribute ,
						&UniFileName ,
						OBJ_CASE_INSENSITIVE ,
						NULL ,
						NULL ) ;
	
	ntStatus = ZwCreateFile(
						&hSourceFile ,
						SYNCHRONIZE ,
						&ObjectAttribute ,
						&IoStatusBlock ,
						NULL ,
						0 ,
						FILE_SHARE_READ ,
						FILE_OPEN ,
						FILE_SYNCHRONOUS_IO_NONALERT | FILE_NON_DIRECTORY_FILE ,
						NULL ,
						0 ) ;
	if( !NT_SUCCESS( ntStatus ))
	{
		goto __ExitProc ;
	}
	
	ntStatus = ZwQueryInformationFile(	
									hSourceFile ,
									&IoStatusBlock ,
									&fsi ,
									sizeof( fsi ) ,
									FileStandardInformation ) ;
	if( !NT_SUCCESS( ntStatus ))
	{
		goto __ExitProc ;
	}
	
	//
	// 写目标文件
	//
	
	swprintf( wszFileName , L"\\??\\%ws" , pwDestFile ) ;
	RtlInitUnicodeString( &UniFileName , wszFileName ) ;

	InitializeObjectAttributes(
						&ObjectAttribute ,
						( PUNICODE_STRING )&UniFileName ,
						OBJ_CASE_INSENSITIVE ,
						NULL ,
						NULL ) ;
	
	ntStatus = ZwCreateFile(
						&hDestFile ,
						SYNCHRONIZE | FILE_WRITE_ACCESS ,
						&ObjectAttribute ,
						&IoStatusBlock ,
						NULL ,
						FILE_ATTRIBUTE_NORMAL ,
						FILE_SHARE_READ ,
						FILE_OVERWRITE_IF ,
						FILE_SYNCHRONOUS_IO_NONALERT | FILE_NON_DIRECTORY_FILE ,
						NULL ,
						0 ) ;
	if( !NT_SUCCESS( ntStatus ))
	{
		goto __ExitProc ;
	}

	dwBlockCount = ( DWORD )( fsi.EndOfFile.QuadPart / CP_BUFFER_SIZE ) ;
	dwRemainBytes = ( DWORD )( fsi.EndOfFile.QuadPart % CP_BUFFER_SIZE ) ;
	dwBufferSize = CP_BUFFER_SIZE ;

	for( i = 0 ; i < 2 ; i ++ )
	{
		for( j = 0 ; j < dwBlockCount ; j ++ )
		{
			ntStatus = ZwReadFile( 
							hSourceFile , 
							NULL , NULL , NULL , 
							&IoStatusBlock , 
							g_pCPFBuffer , 
							dwBufferSize ,
							&BytesOffset , 
							NULL ) ;
			if( !NT_SUCCESS( ntStatus ))
			{
				goto __ExitProc ;
			}
			ntStatus = ZwWriteFile( 
							hDestFile , 
							NULL , NULL , NULL , 
							&IoStatusBlock , 
							g_pCPFBuffer , 
							dwBufferSize ,
							&BytesOffset , 
							NULL ) ;
			if( !NT_SUCCESS( ntStatus ))
			{
				goto __ExitProc ;
			}

			BytesOffset.QuadPart += dwBufferSize ;
		}

		if( dwRemainBytes == 0 )
		{
			break ;
		}
		dwBlockCount = 1 ;
		dwBufferSize = dwRemainBytes ;
	}

__ExitProc :

	if( NULL != hSourceFile )
	{
		ZwClose( hSourceFile ) ;
	}
	if( NULL != hDestFile )
	{
		ZwClose( hDestFile ) ;
	}

	return ntStatus ;
}

//
//========================================================================
//
// 得到指定PCI网卡设备信息
//
//========================================================================
//
typedef unsigned long	DWORD ;
typedef unsigned short	WORD ;
typedef unsigned long * PDWORD ;
typedef unsigned short * PWORD ;

typedef void * PVOID;
typedef unsigned char   BYTE;
typedef BYTE * PBYTE ;

__declspec( naked ) DWORD __stdcall IN_DWORD( 
										IN WORD wIoPort ) 
{   
	__asm 
	{ 
		mov dx , WORD PTR [esp+4] ;
		in  eax , dx ;
		ret 4 ;
	}
}

__declspec( naked ) VOID __stdcall OUT_DWORD( 
										IN WORD		wIoPort , 
										IN DWORD	dwValue ) 
{
	__asm 
	{
		mov dx , WORD PTR [esp+4] ;
		mov eax , DWORD PTR [esp+8] ;
		out dx , eax ;
		ret 8 ;
	}
}

// VEN_1106&DEV_3065&SUBSYS_C03C1462&REV_78

#define		PCI_FORMAT		L"VEN_%.4X&DEV_%.4X&SUBSYS_%.8X&REV_%.2X"
#define		PCI_KEY_PATH	L"\\Registry\\Machine\\System\\CurrentControlSet\\Enum\\PCI"

BOOLEAN __stdcall INTER_GetNicPCIInfo( 
							OUT PWCHAR	pwPCIDeviceName ,
							OUT PBYTE	pDeviceNumber , 
							OUT PBYTE	pFunctionNumber )
{    
    DWORD	io_CF8 ;   // port 0xcf8 
    DWORD	io_CFC ;   // port 0xcfc 
    int		i ; 
	WORD	wDeviceID , wVendorID ;
	BYTE	RevisionID ;
	DWORD	dwSubSys ;
	BYTE	BusNumber , DeviceNumber , FunctionNumber ;
	WORD	wPCIType ;

	//
	// //because the first bit is enable/disable , so must be 1 , so from 0x800000000 
	// 
    io_CF8 = 0x80000000 ;   
    for( ; ; )             
    {    		
        OUT_DWORD( 0xcf8 , io_CF8 ) ; 
        io_CFC = IN_DWORD( 0xcfc ) ; 
		
		//
		// //if =0xffffffff,then is a invalid bus number and device number 
		//
        if( io_CFC != 0xFFFFFFFF )
        {      
			OUT_DWORD( 0xcf8 , io_CF8 + 2 * 4 ) ;
			
			wPCIType = ( WORD )(( IN_DWORD( 0xcfc ) & 0xffff0000 ) >> 16 ) ;
			if( wPCIType == 0x0200 )
			{
				wVendorID = ( WORD )io_CFC ;
				wDeviceID = ( WORD )( io_CFC >> 16 ) ;
						
				{
					BusNumber = ( BYTE )(( io_CF8 & 0x00FF0000 ) >> 16 ) ;
					DeviceNumber = ( BYTE )(( io_CF8 & 0x0000F800 ) >> 11 ) ;
					FunctionNumber = ( BYTE )(( io_CF8 & 0x700 ) >> 8 ) ;	

					* pDeviceNumber = DeviceNumber ;
					* pFunctionNumber = FunctionNumber ;

					OUT_DWORD( 0xcf8 , io_CF8 + 2 * 4 ) ;			
					RevisionID = ( BYTE )IN_DWORD( 0xcfc ) ;

					OUT_DWORD( 0xcf8 , io_CF8 + 11 * 4 ) ;
					dwSubSys = IN_DWORD( 0xcfc ) ;

					swprintf( pwPCIDeviceName , PCI_FORMAT , 
						wVendorID , wDeviceID , dwSubSys , RevisionID ) ;

					return TRUE ;
				}
			}
        } 
        io_CF8 += 0x800 ;     
		
        if( io_CF8 >= 0x80FFFF00 )    
		{
			break ; 
		}
    } 
    
	return FALSE ; 
}

//
//========================================================================
//

NTSTATUS __stdcall INTER_QuerySubKeyName( 
								IN HANDLE	hMainKey , 
								IN DWORD	dwIndex , 
								OUT PWCHAR	pwSubKeyName )
{
	NTSTATUS	ntStatus = STATUS_SUCCESS ;
	DWORD		dwResultLen ;
	BYTE		pSwapBuffer[128] ;
	PKEY_BASIC_INFORMATION	pKeyBasicInfo = NULL ;

	ntStatus = ZwEnumerateKey( 
						hMainKey ,
						dwIndex ,
						KeyBasicInformation ,
						pSwapBuffer , 
						128 ,
						&dwResultLen ) ;
	if( NT_SUCCESS( ntStatus ))
	{
		pKeyBasicInfo = ( PKEY_BASIC_INFORMATION )pSwapBuffer ;
		RtlCopyMemory( pwSubKeyName , pKeyBasicInfo->Name , pKeyBasicInfo->NameLength ) ;
		pwSubKeyName[pKeyBasicInfo->NameLength/2] = 0 ;
	}

	return ntStatus ;
}

NTSTATUS __stdcall INTER_OpenKey( 
							IN PWCHAR	pwKeyName , 
							OUT PHANDLE	phKey )
{
	OBJECT_ATTRIBUTES		ObjectAttributes ;
	UNICODE_STRING			UniRegPath ;
	
	RtlInitUnicodeString( &UniRegPath , pwKeyName ) ;

	InitializeObjectAttributes(
					&ObjectAttributes , 
					&UniRegPath , 
    				OBJ_CASE_INSENSITIVE , 
					NULL , 
					NULL ) ;
	
	return ZwOpenKey(
					phKey , 
					KEY_READ | KEY_WRITE ,
					&ObjectAttributes ) ;
}

NTSTATUS __stdcall INTER_CreateKey( 
							IN PWCHAR	pwKeyName , 
							OUT PHANDLE	phKey )
{
	OBJECT_ATTRIBUTES		ObjectAttributes ;
	UNICODE_STRING			UniRegPath ;
	
	RtlInitUnicodeString( &UniRegPath , pwKeyName ) ;

	InitializeObjectAttributes(
					&ObjectAttributes , 
					&UniRegPath , 
    				OBJ_CASE_INSENSITIVE , 
					NULL , 
					NULL ) ;

	return ZwCreateKey( 
					phKey ,
					KEY_READ | KEY_WRITE ,
					&ObjectAttributes ,
					0 ,
					NULL ,
					0 , 
					NULL ) ;
}

BOOLEAN __stdcall INTER_CopyKey( 
							IN HANDLE	hSrcKey ,
							IN PWCHAR	pwSrcKeyPath ,
							IN HANDLE	hDestKey ,
							IN PWCHAR	pwDestKeyPath )
{
    HANDLE		hNewSrcKey = NULL , hNewDestKey = NULL ;
    int			i ;
	NTSTATUS	ntStatus = STATUS_SUCCESS ;
	WCHAR		wszSubSrcKeyPath[150] ;
	WCHAR		wszSubDestKeyPath[150] ;
	WCHAR		wszKeyName[50] ;
	DWORD		dwResultLen ;
	UNICODE_STRING	UniKeyValueName ;
	PKEY_VALUE_FULL_INFORMATION	pKeyValueFullInfo = NULL ;
	
	PVOID		pRegValueBuffer = ExAllocatePool( NonPagedPool , 1024 ) ;
	if( NULL == pRegValueBuffer )
	{
		ntStatus = STATUS_INSUFFICIENT_RESOURCES ;
		return FALSE ;
	}

	for( i = 0 ; ; i ++ )
	{
		ntStatus = INTER_QuerySubKeyName( hSrcKey , i , wszKeyName ) ;
		if( !NT_SUCCESS( ntStatus ))
		{
			break ;
		}
		
		swprintf( wszSubSrcKeyPath , L"%ws\\%ws" , pwSrcKeyPath , wszKeyName ) ;		
		ntStatus = INTER_OpenKey( wszSubSrcKeyPath , &hNewSrcKey ) ;

		swprintf( wszSubDestKeyPath , L"%ws\\%ws" , pwDestKeyPath , wszKeyName ) ;
		ntStatus = INTER_CreateKey( wszSubDestKeyPath , &hNewDestKey ) ;

		INTER_CopyKey( 
				hNewSrcKey , 
				wszSubSrcKeyPath , 
				hNewDestKey ,
				wszSubDestKeyPath ) ;

		ZwClose( hNewSrcKey ) ;
		ZwClose( hNewDestKey ) ;
	}

	for( i = 0 ; ; i ++ )
	{
		ntStatus = ZwEnumerateValueKey( 
								hSrcKey , 
								i ,
								KeyValueFullInformation , 
								pRegValueBuffer , 
								1024 , 
								&dwResultLen ) ;
		if( !NT_SUCCESS( ntStatus ))
		{
			break ;
		}
		pKeyValueFullInfo = ( PKEY_VALUE_FULL_INFORMATION )pRegValueBuffer ;
		UniKeyValueName.Buffer = pKeyValueFullInfo->Name ;
		UniKeyValueName.Length = UniKeyValueName.MaximumLength = ( USHORT )pKeyValueFullInfo->NameLength ;
	
		ntStatus = ZwSetValueKey( 
								hDestKey ,
								&UniKeyValueName ,
								0 ,
								pKeyValueFullInfo->Type , 
								( PBYTE )pKeyValueFullInfo + pKeyValueFullInfo->DataOffset , 
								pKeyValueFullInfo->DataLength ) ;
	}

	ExFreePool( pRegValueBuffer ) ;
	
    return TRUE;
}

NTSTATUS __stdcall INTER_ProcessNicPnp(
								IN PWCHAR	pwPCIDeviceName , 
								IN BYTE		DeviceNumber , 
								IN BYTE		FunctionNumber )
{
	NTSTATUS				ntStatus = STATUS_SUCCESS ;
	HANDLE					hKey = NULL ;
	WCHAR					wszRegFullPath[150] ;
	WCHAR					wszDestRegFullPath[150] ;
	WCHAR					wszKeyName[50] ;
	WCHAR					wszCode[4] ;
	BYTE					btCode = 0 ;
	PWCHAR					pwExt = NULL ;
	HANDLE					hSrcKey = NULL , hDestKey = NULL ;

	btCode = ( DeviceNumber / 2 ) << 4 ;

	swprintf( wszRegFullPath , L"%ws\\%ws" , PCI_KEY_PATH , pwPCIDeviceName ) ;

	ntStatus = INTER_OpenKey( 
							wszRegFullPath , &hKey ) ;
	if( !NT_SUCCESS( ntStatus ))
	{
		return ntStatus ;
	}

	ntStatus = INTER_QuerySubKeyName( 
							hKey , 0 , wszKeyName ) ;
	ZwClose( hKey ) ;

	if( !NT_SUCCESS( ntStatus ))
	{
		return ntStatus ;
	}
	
	wcscat( wszRegFullPath , L"\\" ) ;
	wcscat( wszRegFullPath , wszKeyName ) ;
	btCode += ( DeviceNumber % 2 ) * 8 ;

	//
	// 打开源注册表键
	//	
	ntStatus = INTER_OpenKey( wszRegFullPath , &hSrcKey ) ;
	if( !NT_SUCCESS( ntStatus ))
	{
		return ntStatus ;
	}

	btCode += FunctionNumber ;

	wcscpy( wszDestRegFullPath , wszRegFullPath ) ;
	swprintf( wszCode , L"%.2X" , btCode ) ;
	pwExt = wcsrchr( wszDestRegFullPath , L'&' ) ;
	wcsncpy( pwExt + 1 , wszCode , 2 ) ;
	

	//
	// 打开目标注册表键
	//
	ntStatus = INTER_OpenKey( 
						wszDestRegFullPath , &hDestKey ) ;

    if( NT_SUCCESS( ntStatus ))
    {
		ZwClose( hDestKey ) ;
		ZwClose( hSrcKey ) ;
    	return ntStatus ;
    }

	ntStatus = INTER_CreateKey(
						wszDestRegFullPath , &hDestKey ) ;
	if( !NT_SUCCESS( ntStatus ))
	{
		ZwClose( hSrcKey ) ;
		return ntStatus ;
	}

	INTER_CopyKey( 
			hSrcKey , wszRegFullPath ,
			hDestKey , wszDestRegFullPath ) ;

	ZwClose( hDestKey ) ;
	ZwClose( hSrcKey ) ;
	
	return ntStatus ;
}

//
//========================================================================
//

NTSTATUS RTKCF_DeviceIoControl(
				IN PDEVICE_OBJECT DeviceObject ,
				IN PIRP Irp )
{
	NTSTATUS			ntStatus = STATUS_SUCCESS ;

	PIO_STACK_LOCATION	IoStack = IoGetCurrentIrpStackLocation( Irp ) ;
	PVOID				pInputBuffer = Irp->AssociatedIrp.SystemBuffer ;
	PVOID				pOutputBuffer = Irp->AssociatedIrp.SystemBuffer ;
	PWCHAR				pwSourceFile = NULL , pwDestFile = NULL ;
	WCHAR				wszNicRegPath[140] ;
	BYTE				btDeviceNumber , btFunctionNumber ;
	BYTE				i ;

	Irp->IoStatus.Status = STATUS_SUCCESS ;
	Irp->IoStatus.Information = 0 ;

	switch( IoStack->Parameters.DeviceIoControl.IoControlCode )
	{
	case IOCTL_RTKCPF_COPYFILE :
		{
			pwSourceFile = ( PWCHAR )pInputBuffer ;
			pwDestFile = pwSourceFile + wcslen( pwSourceFile ) + 1 ;
			ntStatus = INTER_CopyFile( pwSourceFile , pwDestFile ) ;

			* ( PDWORD )pOutputBuffer = ntStatus ;
			Irp->IoStatus.Information = sizeof( DWORD ) ;
		}
		break ;

	case IOCTL_RTKCPF_SETNICPNP :
		{
			//
			// Added by boxe , 2005.07.08 , for support Nic-Pnp
			//
			if( INTER_GetNicPCIInfo(
								wszNicRegPath , 
								&btDeviceNumber ,
								&btFunctionNumber ))
			{
				//
				// 这里假设主板最多有5个PCI插槽
				//
				for( i = btDeviceNumber + 1 ; i < btDeviceNumber + 5 ; i ++ )
				{
					INTER_ProcessNicPnp( 
								wszNicRegPath ,
								i ,
								btFunctionNumber ) ;
				}

				if( btDeviceNumber > 0 )
				{					
					i = ( btDeviceNumber < 5 ) ? 0 : ( btDeviceNumber - 4 ) ;

					for( ; i < btDeviceNumber  ; i ++ )
					{
						INTER_ProcessNicPnp( 
									wszNicRegPath ,
									i ,
									btFunctionNumber ) ;
					}
				}
			}		
		}
		break ;
	
	default :
		{
			Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST ;
		}
		break ;
	}
	
	IoCompleteRequest( Irp , IO_NO_INCREMENT ) ;

	return STATUS_SUCCESS ;
}
	
//
//========================================================================
//
// DriverEntry
//
//========================================================================
//
NTSTATUS DriverEntry(
				IN PDRIVER_OBJECT DriverObject ,
				IN PUNICODE_STRING RegistryPath )
{
    NTSTATUS                ntStatus = STATUS_SUCCESS ;
    UNICODE_STRING          DevNameUniString ;
    UNICODE_STRING          DevLinkUniString ;
	PDEVICE_OBJECT			CtlDeviceObject = NULL ;

	g_pCPFBuffer = ExAllocatePool( NonPagedPool , CP_BUFFER_SIZE ) ;
	if( NULL == g_pCPFBuffer )
	{
		return STATUS_INSUFFICIENT_RESOURCES ;
	}

    RtlInitUnicodeString( &DevNameUniString , NTDEVICENAME ) ;

    ntStatus = IoCreateDevice( 
						DriverObject ,
                        0 ,
                        &DevNameUniString ,
                        FILE_DEVICE_RTKCPF ,
                        0 ,
                        TRUE ,
                        &CtlDeviceObject ) ;
    if( !NT_SUCCESS( ntStatus )) 
	{
		ExFreePool( g_pCPFBuffer ) ;
		return ntStatus ;
    }
	    
    RtlInitUnicodeString( &DevLinkUniString , DOSDEVICENAME ) ;
                          
    ntStatus = IoCreateSymbolicLink(
						&DevLinkUniString ,
						&DevNameUniString ) ;                                         
    if( !NT_SUCCESS( ntStatus ))
	{
		ExFreePool( g_pCPFBuffer ) ;
		IoDeleteDevice( CtlDeviceObject ) ;
        return ntStatus ;
    }
    
	DriverObject->MajorFunction[IRP_MJ_CREATE]			= 
	DriverObject->MajorFunction[IRP_MJ_CLOSE]			= RTKCF_CreateClose ;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]	= RTKCF_DeviceIoControl ;

	DriverObject->DriverUnload = RTKCF_Unload ;

    return ntStatus ;
}

//
//========================================================================
//
