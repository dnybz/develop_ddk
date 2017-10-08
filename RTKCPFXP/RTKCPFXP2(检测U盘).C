//
//========================================================================
//
#define		_WINNT

#include	"ntddk.h"
#include	"stdio.h"
#include	"stdarg.h"
#include	"initguid.h"
#include	"wdmguid.h"

#include	"TypeInfo.h"
#include	"RTKCPFXP.H"
#include	"RTIOCTL.H"

//
//========================================================================
//
static		PVOID			g_pCPFBuffer = NULL ;
#define		CP_BUFFER_SIZE	(1024*1024) //1M

static	PVOID	g_pNotificationHandle = NULL ;

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
	if( NULL != g_pNotificationHandle )
	{
		IoUnregisterPlugPlayNotification( g_pNotificationHandle ) ;
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
	swprintf( wszFileName , L"\\??\\%ws" , pwSourceFile ) ;
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

NTSTATUS RTKCF_DeviceIoControl(
				IN PDEVICE_OBJECT DeviceObject ,
				IN PIRP Irp )
{
	NTSTATUS			ntStatus = STATUS_SUCCESS ;

	PIO_STACK_LOCATION	IoStack = IoGetCurrentIrpStackLocation( Irp ) ;
	PVOID				pInputBuffer = Irp->AssociatedIrp.SystemBuffer ;
	PVOID				pOutputBuffer = Irp->AssociatedIrp.SystemBuffer ;
	PWCHAR				pwSourceFile = NULL , pwDestFile = NULL ;

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
// 得到指定PCI设备信息
//
//========================================================================
//
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

#define		PCI_FORMAT		L"VEN_%.4X&DEV_%.4X&SUBSYS_%.8X&REV_%.2X\n"
#define		PCI_KEY_PATH	L"\\Registry\\Machine\\System\\CurrentControlSet\\Enum\\PCI"

BOOLEAN __stdcall INTER_GetPCIInfo( 
							IN WORD	wPCIVendorID , 
							IN WORD	wPCIDeviceID , 
							OUT PWCHAR	pwPCIDeviceName )
{    
    DWORD	io_CF8 ;   // port 0xcf8 
    DWORD	io_CFC ;   // port 0xcfc 
    int		i ; 
	WORD	wDeviceID , wVendorID ;
	BYTE	RevisionID ;
	DWORD	dwSubSys ;
	BYTE	BusNumber , DeviceNumber , FunctionNumber ;

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
			wVendorID = ( WORD )io_CFC ;
			wDeviceID = ( WORD )( io_CFC >> 16 ) ;
			
			if( wVendorID == wPCIVendorID && 
				wDeviceID == wPCIDeviceID )
			{
				BusNumber = ( BYTE )(( io_CF8 & 0x00FF0000 ) >> 16 ) ;
				DeviceNumber = ( BYTE )(( io_CF8 & 0x0000F800 ) >> 11 ) ;
				FunctionNumber = ( BYTE )(( io_CF8 & 0x700 ) >> 8 ) ;				

				OUT_DWORD( 0xcf8 , io_CF8 + 2 * 4 ) ;			
				RevisionID = ( BYTE )IN_DWORD( 0xcfc ) ;

				OUT_DWORD( 0xcf8 , io_CF8 + 11 * 4 ) ;
				dwSubSys = IN_DWORD( 0xcfc ) ;
#if 0
				DbgPrint( "PCI Bus : 0x%x Device : 0x%x Function : 0x%x\n" , 
						BusNumber , DeviceNumber , FunctionNumber ) ;

				DbgPrint( "VendorID : 0x%x DeviceId : 0x%x Revision : 0x%x SubSystemID : 0x%x\n" , 
					wVendorID , wDeviceID , RevisionID , dwSubSys ) ;

				DbgPrint( PCI_FORMAT , wVendorID , wDeviceID , dwSubSys , RevisionID ) ;
#endif
				swprintf( pwPCIDeviceName , PCI_FORMAT , 
					wVendorID , wDeviceID , dwSubSys , RevisionID ) ;

				return TRUE ;
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

NTSTATUS __stdcall INTER_QueryRegKey(
								IN PWCHAR	pwPCIDeviceName )
{
	NTSTATUS				ntStatus = STATUS_SUCCESS ;
	UNICODE_STRING			UniPCIRegPath ;
	OBJECT_ATTRIBUTES		ObjectAttributes ;
	HANDLE					hKey = NULL ;
	WCHAR					wszRegFullPath[256] ;
	BYTE					QueryBuffer[128] ;
	WCHAR					wszKeyName[64] ;
	DWORD					dwResultLen ;
	PKEY_BASIC_INFORMATION	pKeyBasicInfo = NULL ;

	swprintf( wszRegFullPath , L"%ws\\%ws" , PCI_KEY_PATH , pwPCIDeviceName ) ;

	RtlInitUnicodeString( &UniPCIRegPath , wszRegFullPath ) ;

	InitializeObjectAttributes(
						&ObjectAttributes , 
						&UniPCIRegPath , 
    					OBJ_CASE_INSENSITIVE , 
						NULL , 
						NULL ) ;
	
    ntStatus = ZwOpenKey(
						&hKey, KEY_READ, &ObjectAttributes ) ;
    if( ntStatus != STATUS_SUCCESS )
    {
    	return ntStatus ;
    }

	ntStatus = ZwEnumerateKey( 
						hKey , 
						0 , 
						KeyBasicInformation ,
						QueryBuffer , 128 ,
						&dwResultLen ) ;
	ZwClose( hKey ) ;
	if( ntStatus != STATUS_SUCCESS )
    {
    	return ntStatus ;
    }

	pKeyBasicInfo = ( PKEY_BASIC_INFORMATION )QueryBuffer ;
	wcsncpy( wszKeyName , pKeyBasicInfo->Name , pKeyBasicInfo->NameLength / 2 ) ;
	wszKeyName[pKeyBasicInfo->NameLength/2] = 0 ;

	DbgPrint( "PCI Sub Key is %ws \n" , wszKeyName ) ;
	
	return ntStatus ;
}

	
//
//========================================================================
//
// DriverEntry
//
//========================================================================
//
NTSTATUS __stdcall RT_MountVolume( VOID )
{
	HANDLE				hFile = NULL ;
	IO_STATUS_BLOCK		ioStatus ;
	OBJECT_ATTRIBUTES   objectAttributes ;
	NTSTATUS			status = STATUS_SUCCESS ;
	UNICODE_STRING		DriveName ;
	WCHAR				NameBuffer[] = L"\\DosDevices\\k:\\" ;

	RtlInitUnicodeString( &DriveName , NameBuffer ) ;
	InitializeObjectAttributes(
					&objectAttributes , 
					&DriveName, 
					OBJ_CASE_INSENSITIVE, 
					NULL, NULL ) ;
	status = ZwCreateFile( 
					&hFile , 
					SYNCHRONIZE , 									
					&objectAttributes ,
					&ioStatus , 									
					NULL , 
					FILE_ATTRIBUTE_NORMAL , 									
					FILE_SHARE_READ | FILE_SHARE_WRITE, 
					FILE_OPEN, 
					FILE_SYNCHRONOUS_IO_NONALERT ,
					NULL, 0 ) ;
	if( !NT_SUCCESS( status ))
	{
		DbgPrint( "ZwCreateFile failed:0x%x!\n" , status ) ;
	}
	else
	{
		ZwClose( hFile ) ;
		DbgPrint( "Open UDisk successful!\n" ) ;
	}

	return status ;
}

#define RT_IsEqualGUID(rguid1, rguid2) (sizeof(GUID) == RtlCompareMemory(rguid1, rguid2, sizeof(GUID)))

#if 0
typedef enum _PROCESSINFOCLASS
{
	ProcessDeviceMap = 23,

} PROCESSINFOCLASS ;

typedef struct _PROCESS_DEVICEMAP_INFORMATION {
    union {
        struct {
            HANDLE DirectoryHandle;
        } Set;
        struct {
            ULONG DriveMap;
            UCHAR DriveType[ 32 ];
        } Query;
    };
} PROCESS_DEVICEMAP_INFORMATION, *PPROCESS_DEVICEMAP_INFORMATION;
#endif
static	DWORD		g_dwDriveMask = 0 ;
static KEVENT		g_WaitEvent ;


NTKERNELAPI                                                     
NTSTATUS
ZwQueryInformationProcess( 
					IN HANDLE ProcessHandle,
					IN PROCESSINFOCLASS ProcessInformationClass,
					OUT PVOID ProcessInformation,
					IN ULONG ProcessInformationLength,
					OUT PULONG ReturnLength OPTIONAL );

#define		WAIT_TIME					(-1 * 10000 * 1000) //	1 second

static PDRIVER_OBJECT		gFastFatDriverObject = NULL ;

static BOOLEAN				g_bGetFastFatDriverObject = FALSE ;
static BOOLEAN				g_bDriveANameOK = FALSE ;
static PDRIVER_OBJECT		g_DriverObject = NULL ;

VOID  __stdcall RT_MountFloppyThread( PVOID Context )
{
	LARGE_INTEGER		Time ;
	HANDLE				hFile = NULL ;
	IO_STATUS_BLOCK		ioStatus ;
	OBJECT_ATTRIBUTES   objectAttributes;
	NTSTATUS			status = STATUS_SUCCESS ;

	PDRIVER_OBJECT		MyDriverObject = NULL ;

	UNICODE_STRING		UnicodeFastFat ;
	PDEVICE_OBJECT		FatDeviceObject = NULL ;
	PFILE_OBJECT		FatFileObject = NULL ;

	Time.QuadPart = WAIT_TIME ;

	for( ; ; )
	{
		if( ! g_bGetFastFatDriverObject )
		{
			RtlInitUnicodeString( &UnicodeFastFat , L"\\Fat" ) ;
			status = IoGetDeviceObjectPointer( 
									&UnicodeFastFat , FILE_READ_DATA ,
									&FatFileObject , &FatDeviceObject ) ;										
			if( NT_SUCCESS( status ))
			{
				//
				// for some reason \Fat has been mounted , 
				// and the deviceobject we got here belongs to the highest one ,
				// but fileobject is safe to use .
				//
				FatDeviceObject = FatFileObject->DeviceObject ;
				gFastFatDriverObject = FatDeviceObject->DriverObject ;
				ObDereferenceObject( FatFileObject ) ; // Added By Boxer , 2004.5.25
				g_bGetFastFatDriverObject = TRUE ;
			}
		}

		//
		// Mount软驱
		//
		if( !g_bDriveANameOK && ( NULL != gFastFatDriverObject ))
		{
			BOOLEAN			bIsMyDeviceObject = FALSE ;
			PDEVICE_OBJECT	MyDeviceObject = gFastFatDriverObject->DeviceObject ;
			PDEVICE_OBJECT	TempDeviceObject = MyDeviceObject ;
			
			while( NULL != TempDeviceObject )
			{
				if( TempDeviceObject->DriverObject == g_DriverObject )
				{
					bIsMyDeviceObject = TRUE ;
					break ;
				}
				TempDeviceObject = TempDeviceObject->AttachedDevice ;
			}

			if( !bIsMyDeviceObject )
			{
				TempDeviceObject = MyDeviceObject ;
				while( NULL != TempDeviceObject->AttachedDevice )
				{
					TempDeviceObject = TempDeviceObject->AttachedDevice ;
				}
				
				g_bDriveANameOK = TRUE ;
				
				TempDeviceObject = MyDeviceObject->NextDevice ;
				if( NULL == TempDeviceObject )
				{
					continue ; // go to sleep
				}
				while( NULL != TempDeviceObject )
				{
					if( TempDeviceObject->DriverObject == g_DriverObject )
					{
						break ;
					}
					TempDeviceObject = TempDeviceObject->AttachedDevice ;
				}
				if( NULL == TempDeviceObject )
				{
					TempDeviceObject = MyDeviceObject->NextDevice ;
					while( NULL != TempDeviceObject->AttachedDevice )
					{
						TempDeviceObject = TempDeviceObject->AttachedDevice ;
					}
				}
			}
		}

		KeWaitForSingleObject( &g_WaitEvent , Executive , KernelMode ,
								FALSE , &Time ) ;								
	}

	PsTerminateSystemThread( STATUS_SUCCESS ) ;
}

VOID  __stdcall  RT_DetectUDiskThread( PVOID Context )
{
	NTSTATUS			status ;
	DWORD				dwDriverBitMask = 0 ;
	LARGE_INTEGER		Time ;

	PDRIVER_OBJECT		MyDriverObject = NULL ;
	PROCESS_DEVICEMAP_INFORMATION	pdi ;
	DWORD				i = 0 ;

	Time.QuadPart = WAIT_TIME ;

	for( ; ; )
	{
		KeWaitForSingleObject( &g_WaitEvent , Executive , KernelMode ,
								FALSE , NULL ) ;
		for( i = 3 ; i > 0 ; i -- )
		{
			status = ZwQueryInformationProcess(
										( HANDLE )-1 , 
										ProcessDeviceMap , 												  
										&pdi ,
										sizeof( pdi ),
										NULL ) ;
												  												  
			if( !NT_SUCCESS( status ))
			{
				break ;
			}
			dwDriverBitMask = pdi.Query.DriveMap ;

			if( g_dwDriveMask != dwDriverBitMask )
			{
				DbgPrint( "Detect UDisk\n" ) ;
				RT_MountVolume( ) ;
				break ;
			}

			KeDelayExecutionThread( KernelMode , FALSE , &Time ) ;
		}								
	}

	PsTerminateSystemThread( STATUS_SUCCESS ) ;
}

NTSTATUS RT_PnpNotifyInterfaceChange(
							IN  PDEVICE_INTERFACE_CHANGE_NOTIFICATION NotificationStruct,
							IN  PVOID                        Context )
{
	LARGE_INTEGER	WaitTime ;
	NTSTATUS		status = 0 ;
	if( !RT_IsEqualGUID(( LPGUID )&(NotificationStruct->InterfaceClassGuid ) ,
                      (LPGUID)&GUID_CLASS_USB_DEVICE )) 
	{
        DebugPrint(("Bad interfaceClassGuid\n"));
        return STATUS_SUCCESS ;
    }

	if( RT_IsEqualGUID((LPGUID)&(NotificationStruct->Event), 
                     (LPGUID)&GUID_DEVICE_INTERFACE_ARRIVAL )) 
	{
        DebugPrint(("Arrival Notification\n"));
		//
		// 枚举本地磁盘
		//
#if 0
		WaitTime.QuadPart = -100000000 ;
		KeDelayExecutionThread( KernelMode , FALSE , &WaitTime ) ;
		RT_MountVolume( ) ;
#else
		{
			KeSetEvent( &g_WaitEvent , IO_NO_INCREMENT , FALSE ) ;
		}	
#endif	
	}

	if( RT_IsEqualGUID((LPGUID)&(NotificationStruct->Event), 
                     (LPGUID)&GUID_DEVICE_INTERFACE_REMOVAL )) 
	{
        DebugPrint(("Removal Notification\n"));
	}
	
	return STATUS_SUCCESS ;
}

NTSTATUS DriverEntry(
				IN PDRIVER_OBJECT DriverObject ,
				IN PUNICODE_STRING RegistryPath )
{
    NTSTATUS                ntStatus = STATUS_SUCCESS ;
    UNICODE_STRING          DevNameUniString ;
    UNICODE_STRING          DevLinkUniString ;
	PDEVICE_OBJECT			CtlDeviceObject = NULL ;
	WCHAR					wszPCIDeviceName[128] ;
	PROCESS_DEVICEMAP_INFORMATION	pdi ;

	g_DriverObject = DriverObject ;

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
	
#if 1
	
	ntStatus = IoRegisterPlugPlayNotification (
                EventCategoryDeviceInterfaceChange ,            
                PNPNOTIFY_DEVICE_INTERFACE_INCLUDE_EXISTING_INTERFACES ,
                ( PVOID )&GUID_CLASS_USB_DEVICE ,
                DriverObject,
                ( PDRIVER_NOTIFICATION_CALLBACK_ROUTINE )RT_PnpNotifyInterfaceChange ,
                NULL ,
                &g_pNotificationHandle ) ;


	ntStatus = ZwQueryInformationProcess(
						( HANDLE )-1 , ProcessDeviceMap , 										  
						&pdi , sizeof( pdi ), NULL ) ;
										  										  
	if( NT_SUCCESS( ntStatus ))
	{
		HANDLE hThread ;

		g_dwDriveMask = pdi.Query.DriveMap ;
		KeInitializeEvent( &g_WaitEvent , SynchronizationEvent , FALSE ) ;
		
		ntStatus = PsCreateSystemThread(
								&hThread , ( ACCESS_MASK )0L ,							 
								NULL , NULL , NULL ,							 							 
								RT_DetectUDiskThread , NULL ) ;
		if( NT_SUCCESS( ntStatus ))
		{
			ZwClose( hThread ) ;
		}
	}

#endif

#if 0
	if( INTER_GetPCIInfo( 0x1106 , 0x3065 , wszPCIDeviceName ))
	{
		INTER_QueryRegKey( wszPCIDeviceName ) ;
	}
#endif
	
    return ntStatus ;
}

//
//========================================================================
//
