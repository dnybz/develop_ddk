//////////////////////////////////////////////////////////////////////////
// Copyright (c) 2013-2013 TnsoftTech Corporation
//
// Module Name:
//		CopyFileDriver.h
//
// Abstract:
//		This is the main module for the CopyFileDriver driver.
// 
// Environment:
//		Kernel mode
//
// Version History:
//////////////////////////////////////////////////////////////////////////
#include <stdarg.h>
#include <stdio.h>

#include "CopyFileDriver.h"

int test( const char * fmt, ... );
int tnsoftndisDebugPrint( const char * fmt, ... );

//////////////////////////////////////////////////////////////////////////
// Routine Description:
//		This function initializes the driver, and creates
//		any objects needed to process I/O requests.
//
// Arguments:
//		Pointer to the Driver object
//		Registry path string for driver service key
//
// Return Value:
//		This function returns STATUS_XXX

#pragma INITCODE
extern "C"
NTSTATUS DriverEntry(IN PDRIVER_OBJECT  DriverObject,
					 IN PUNICODE_STRING RegistryPath)
{
	NTSTATUS	Status = STATUS_SUCCESS;
	int			i = 0;

	UNREFERENCED_PARAMETER(RegistryPath);
	KdPrint(("++++++Enter DriverEntry!++++++\n"));

	DriverObject->DriverUnload = Unload;

	for (i=0; i < arraysize(DriverObject->MajorFunction); ++i)
		DriverObject->MajorFunction[i] = DispatchAny;

	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceControl;
	Status = CreateDevice(DriverObject);

	KdPrint(("------Leave DriverEntry!------\n"));
	return STATUS_SUCCESS;
}

#pragma INITCODE 
NTSTATUS CreateDevice(IN PDRIVER_OBJECT DriverObject)
{
	NTSTATUS			Status = STATUS_SUCCESS;
	PDEVICE_OBJECT		fdo = NULL;
	UNICODE_STRING		DeviceName = {0};
	UNICODE_STRING		SymLinkName = {0};
	PDEVICE_EXTENSION	pdx = NULL;

	KdPrint(("++++++Enter CreateDevice!++++++\n"));
	RtlInitUnicodeString(&DeviceName, DEVICE_NAME);

	Status = IoCreateDevice(DriverObject,
							sizeof(DEVICE_EXTENSION),
							&DeviceName,
							FILE_DEVICE_UNKNOWN,
							0, TRUE, 
							&fdo);

	if (!NT_SUCCESS(Status))
	{
		KdPrint(("IoCreateDevice fail: Status = 0x%08X\n"));
		return Status;
	}

	RtlInitUnicodeString(&SymLinkName, DEVICE_SYMLINK_NAME);

	Status = IoCreateSymbolicLink(&SymLinkName, &DeviceName);
	if (!NT_SUCCESS(Status))
	{
		IoDeleteDevice(fdo);
		KdPrint(("IoCreateSymbolicLink fail: Status = 0x%08X\n"));
		return Status;
	}

	pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;
	pdx->fdo = fdo;
	pdx->DeviceName = DeviceName;
	pdx->SymLinkName = SymLinkName;
	fdo->Flags |= DO_BUFFERED_IO;

	KdPrint(("------Leave CreateDevice!------\n"));
	return STATUS_SUCCESS;
}

#pragma PAGEDCODE
NTSTATUS DeviceControl(IN PDEVICE_OBJECT fdo,
					   IN PIRP Irp)
{
	NTSTATUS			Status = STATUS_SUCCESS;	
	PIO_STACK_LOCATION	IrpStack = IoGetCurrentIrpStackLocation(Irp);
	ULONG				InputLen = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG				OutputLen = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;
	ULONG				IoctlCode = IrpStack->Parameters.DeviceIoControl.IoControlCode;
	PVOID				InputBuffer  = Irp->AssociatedIrp.SystemBuffer;
	PVOID				OutputBuffer = Irp->AssociatedIrp.SystemBuffer;
	ULONG_PTR			Info = 0;

	UNREFERENCED_PARAMETER(fdo);

	switch (IoctlCode)
	{
	case IOCTL_OPEN_FILE:
		{
			PCHAR		pszSourceFile = NULL;
			HANDLE		FileHandle = NULL;
			WCHAR		wszSourceFile[MAX_PATH] = {0};
			BOOLEAN		bOk = FALSE;

			//KdPrint(("Enter IOCTL_OPEN_FILE!\n"));

			if (InputBuffer == NULL)
			{
				Status = STATUS_INVALID_PARAMETER;
				break;
			}
			
			pszSourceFile = (PCHAR) InputBuffer;
			bOk = CharToWchar(pszSourceFile, wszSourceFile, sizeof(wszSourceFile));
			if (bOk)
				Status = KernelOpenFile(wszSourceFile, &FileHandle);
			else 
				Status = STATUS_INVALID_PARAMETER;
			
			if (!NT_SUCCESS(Status)) break;

			ULONG Address = (ULONG) FileHandle;
			PVOID pAddress = &Address;

			RtlZeroMemory(OutputBuffer, OutputLen);
			RtlCopyMemory(OutputBuffer, pAddress, sizeof(ULONG));
			Info = sizeof(ULONG);
		}
		break;

	case IOCTL_READ_FILE:
		{
			KdPrint(("Enter IOCTL_READ_FILE!\n"));
			if (InputBuffer == NULL || OutputBuffer == NULL)
			{
				Status = STATUS_INVALID_PARAMETER;
				break;
			}

			PREAD_BUFFER	pReadFilePar = (PREAD_BUFFER) InputBuffer;
			HANDLE			FileHandle = (HANDLE) pReadFilePar->ulFileHandle;
			LARGE_INTEGER	ByteOffset = pReadFilePar->ByteOffset;
			DWORD			BytesReturned = 0;

			Status = KernelReadFile(FileHandle,
									OutputBuffer,
									OutputLen,
									ByteOffset,
									&BytesReturned);

			if (!NT_SUCCESS(Status)) break;
			Info = BytesReturned;
		}
		break;

	case IOCTL_CLOSE_FILE:
		{
			HANDLE FileHandle = NULL;
			FileHandle = (HANDLE) (* (ULONG*) InputBuffer);
			Status = KernelCloseFile(FileHandle);
		}
		break;

	case IOCTL_COPY_FILE:
		{
			PSEND_DATA	pSendData = (PSEND_DATA) InputBuffer;
			ULONG		ulLen = FIELD_OFFSET(SEND_DATA, DestFileLen);

			if (pSendData == NULL || InputLen < ulLen)
			{
				Status = STATUS_INVALID_PARAMETER;
				break;
			}

			PCHAR	pszSourceFile = pSendData->szSourceFile;
			PCHAR	pszDestFile = pSendData->szDestFile;
			WCHAR	wszSourceFile[MAX_PATH] = {0};
			WCHAR	wszDestFile[MAX_PATH] = {0};
			BOOLEAN bOk, bRet;

			//KdPrint(("Enter IOCTL_COPY_FILE!\n"));

			bOk = CharToWchar(pszSourceFile, wszSourceFile, sizeof(wszSourceFile));
			bRet = CharToWchar(pszDestFile, wszDestFile, sizeof(wszDestFile));

			if (bOk && bRet)
				Status = KernelCopyFile(wszSourceFile, wszDestFile);
			else
				Status = STATUS_INVALID_PARAMETER;
		}
		break;

	case IOCTL_GET_FILE_SIZE:
		{
			HANDLE			FileHandle = NULL;
			LARGE_INTEGER 	FileSize = {0};
			PVOID			pAddress = NULL;
			LONGLONG		Size64 = 0; 

			FileHandle = (HANDLE) (* (ULONG*) InputBuffer);
			Status = KernelGetFileSize(FileHandle, &FileSize);
			if (!NT_SUCCESS(Status)) break;

			Size64 = FileSize.QuadPart;
			// pAddress = &FileSize;
			pAddress = &Size64;

			RtlZeroMemory(OutputBuffer, OutputLen);
			RtlCopyMemory(OutputBuffer, pAddress, sizeof(LONGLONG));
			Info = sizeof(LONGLONG);
		}
		break;

	default:
		Status = STATUS_INVALID_DEVICE_REQUEST;
		break;
	}

	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = Info;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Status;
}

BOOLEAN CharToWchar(IN  PCHAR	CharBuffer, 
					OUT PWCHAR	WcharBuffer, 
					IN  ULONG	WcharBufferSize)
{
	UNICODE_STRING	UnicodeString = {0};
	ANSI_STRING		AnsiString = {0};
	BOOLEAN			bOk = FALSE;

	RtlZeroMemory(WcharBuffer, WcharBufferSize);

	RtlInitAnsiString(&AnsiString, CharBuffer);
	RtlAnsiStringToUnicodeString(&UnicodeString, &AnsiString, TRUE);

	__try
	{
		if (WcharBufferSize - 2 >= UnicodeString.Length)
		{
			// RtlCopyMemory(WcharBuffer, UnicodeString.Buffer, UnicodeString.Length);
			// WcharBuffer[UnicodeString.Length - 2] = 0;
			wcscpy(WcharBuffer, UnicodeString.Buffer);
			bOk = TRUE;
		}
		else
			bOk = FALSE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		bOk = FALSE;
	}

	RtlFreeUnicodeString(&UnicodeString);
	return bOk;
}

NTSTATUS KernelOpenFile(IN PWCHAR FileName,
						IN OUT HANDLE *FileHandle)
{
	NTSTATUS			Status = STATUS_SUCCESS;
	HANDLE				hFile = NULL;
	OBJECT_ATTRIBUTES	ObjectAttributes = {0};
	UNICODE_STRING		UniFileName = {0};
	IO_STATUS_BLOCK		IoStatus = {0};
	WCHAR				wszFileName[MAX_PATH] = {0};

	swprintf(wszFileName, L"\\??\\%ws", FileName);
	//KdPrint(("###### %ws\n", wszFileName));
	RtlInitUnicodeString(&UniFileName, wszFileName);

	InitializeObjectAttributes(&ObjectAttributes, &UniFileName, OBJ_CASE_INSENSITIVE, NULL, NULL);

	// Read handle
	Status = ZwCreateFile(&hFile,
						  SYNCHRONIZE,
						  &ObjectAttributes,
						  &IoStatus,
						  NULL,
						  0,
						  FILE_SHARE_READ,
						  FILE_OPEN,
						  FILE_SYNCHRONOUS_IO_NONALERT | FILE_NON_DIRECTORY_FILE,
						  NULL, 0);
	
	if (!NT_SUCCESS(Status))
		return Status;

	if (FileHandle != NULL)
	{
		*FileHandle = hFile;
	} 
	else
	{
		//KdPrint(("@@@@@@ FileHandle == NULL\n"));
		Status = STATUS_INVALID_PARAMETER;
	}

	return Status;
}

NTSTATUS GetDestFileHandle(IN PWCHAR FileName,
						   IN OUT HANDLE *FileHandle)
{
	NTSTATUS			Status = STATUS_SUCCESS;
	HANDLE				hFile = NULL;
	OBJECT_ATTRIBUTES	ObjectAttributes = {0};
	UNICODE_STRING		UniFileName = {0};
	IO_STATUS_BLOCK		IoStatus = {0};
	WCHAR				wszFileName[MAX_PATH] = {0};

	swprintf(wszFileName, L"\\??\\%ws", FileName);
	//KdPrint(("###### %ws\n", wszFileName));
	RtlInitUnicodeString(&UniFileName, wszFileName);

	InitializeObjectAttributes(&ObjectAttributes, &UniFileName, OBJ_CASE_INSENSITIVE, NULL, NULL);

	// Write handle
	Status = ZwCreateFile(&hFile,
						  SYNCHRONIZE | FILE_WRITE_ACCESS,
						  &ObjectAttributes,
						  &IoStatus,
						  NULL,
						  FILE_ATTRIBUTE_NORMAL,
						  FILE_SHARE_READ,
						  FILE_OVERWRITE_IF,
						  FILE_SYNCHRONOUS_IO_NONALERT | FILE_NON_DIRECTORY_FILE,
						  NULL, 0);

	if (!NT_SUCCESS(Status))
		return Status;

	if (FileHandle != NULL)
	{
		*FileHandle = hFile;
	} 
	else
	{
		//KdPrint(("@@@@@@ FileHandle == NULL\n"));
		Status = STATUS_INVALID_PARAMETER;
	}

	return Status;
}

NTSTATUS KernelGetFileSize(IN HANDLE FileHandle,
						   IN OUT LARGE_INTEGER *pFileSize)
{
	NTSTATUS					Status = STATUS_SUCCESS;
	IO_STATUS_BLOCK				IoStatus = {0};
	FILE_STANDARD_INFORMATION	fsi = {0};

	Status = ZwQueryInformationFile(FileHandle,
									&IoStatus,
									&fsi,
									sizeof(fsi),
									FileStandardInformation);

	if (!NT_SUCCESS(Status))
		return Status;

	if (pFileSize != NULL)
	{
		*pFileSize = fsi.EndOfFile;
	} 
	else
	{
		//KdPrint(( "@@@@@@ FileSize == NULL\n" ));
		Status = STATUS_INVALID_PARAMETER;
	}

	return Status;
}

NTSTATUS KernelCopyFile(IN PWCHAR pwSourceFile,
						IN PWCHAR pwDestFile)
{
	NTSTATUS		Status = STATUS_SUCCESS;
	HANDLE			ReadHandle = NULL;
	HANDLE			WriteHandle = NULL;
	IO_STATUS_BLOCK	IoStatus = {0};
	LARGE_INTEGER	ByteOffset = {0};
	LARGE_INTEGER	FileSize = {0};
	PUCHAR			Buffer = NULL;
	ULONG_PTR		WriteLen = 0;

	Status = KernelOpenFile(pwSourceFile, &ReadHandle);
	if (!NT_SUCCESS(Status)) goto __Exit;
		
	Status = KernelGetFileSize(ReadHandle, &FileSize);
	if (!NT_SUCCESS(Status)) goto __Exit;

	Status = GetDestFileHandle(pwDestFile, &WriteHandle);
	if (!NT_SUCCESS(Status)) goto __Exit;

	Buffer = (PUCHAR) ExAllocatePoolWithTag(PagedPool, MAX_READ_SIZE, COPY_TAG); // 64k
	if (Buffer == NULL)
	{
		Status = STATUS_INSUFFICIENT_RESOURCES;
		goto __Exit;
	}

	while (FileSize.QuadPart > 0)
	{
		Status = ZwReadFile(ReadHandle,
							NULL, NULL, NULL,
							&IoStatus,
							Buffer,
							MAX_READ_SIZE,
							&ByteOffset,
							NULL);

		if (!NT_SUCCESS(Status)) break;

		WriteLen = IoStatus.Information;

		Status = ZwWriteFile(WriteHandle,
							 NULL, NULL, NULL,
							 &IoStatus,
							 Buffer,
							 WriteLen,
							 &ByteOffset,
							 NULL);

		if (!NT_SUCCESS(Status) || WriteLen != IoStatus.Information)
			break;

		FileSize.QuadPart -= WriteLen;
		ByteOffset.QuadPart += WriteLen;
	}

__Exit:

	if (ReadHandle != NULL) 
		ZwClose(ReadHandle);

	if (WriteHandle != NULL)
		ZwClose(WriteHandle);

	if (Buffer != NULL)
	{
		ExFreePoolWithTag(Buffer, COPY_TAG);
		Buffer = NULL;
	}

	return Status ;
}

NTSTATUS KernelReadFile(IN  HANDLE FileHandle,
						OUT PVOID Buffer,
						IN  ULONG Length,
						IN  LARGE_INTEGER ByteOffset,
						OUT LPDWORD lpBytesReturned)
{
	NTSTATUS		Status = STATUS_SUCCESS;
	IO_STATUS_BLOCK	IoStatus = {0};

	Status = ZwReadFile(FileHandle,
						NULL, NULL, NULL,
						&IoStatus,
						Buffer,
						Length,
						&ByteOffset,
						NULL);

	KdPrint(("*** IoStatus.Information = %ld\n", IoStatus.Information));

	if (STATUS_END_OF_FILE == Status)
	{
		Status = STATUS_SUCCESS;
	}

	if (!NT_SUCCESS(Status))
	{
		KdPrint(("###KernelReadFile: ByteOffset = %ld\n", ByteOffset.QuadPart));
		KdPrint(("###KernelReadFile: Status = 0x%08X\n", Status));
		return Status;
	}	
		
	if (lpBytesReturned != NULL)
		*lpBytesReturned = IoStatus.Information;

	KdPrint(("$$$$KernelReadFile: Status = 0x%08X\n", Status));
	return Status;
}

NTSTATUS KernelCloseFile(IN HANDLE Handle)
{
	NTSTATUS Status = STATUS_SUCCESS;
	Status = ZwClose(Handle);
	return Status;
}

#pragma PAGEDCODE
void Unload(IN PDRIVER_OBJECT DriverObject)
{
	PDEVICE_OBJECT pNextDevice = DriverObject->DeviceObject;

	PAGED_CODE();
	KdPrint(("++++++Enter Unload!++++++\n"));

	while (pNextDevice)
	{
		PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) pNextDevice->DeviceExtension;
		IoDeleteSymbolicLink(&pdx->SymLinkName);
		pNextDevice = pNextDevice->NextDevice;
		IoDeleteDevice(pdx->fdo);
	}

	KdPrint(("------Leave Unload!------\n"));
}

#pragma PAGEDCODE
NTSTATUS DispatchAny(IN PDEVICE_OBJECT fdo,
					 IN PIRP Irp)
{
	PAGED_CODE();
	UNREFERENCED_PARAMETER(fdo);
	//KdPrint(("++++++Enter DispatchAny!++++++\n"));

	//PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);
	static char* irpname[] = 
	{
		"IRP_MJ_CREATE",
		"IRP_MJ_CREATE_NAMED_PIPE",
		"IRP_MJ_CLOSE",
		"IRP_MJ_READ",
		"IRP_MJ_WRITE",
		"IRP_MJ_QUERY_INFORMATION",
		"IRP_MJ_SET_INFORMATION",
		"IRP_MJ_QUERY_EA",
		"IRP_MJ_SET_EA",
		"IRP_MJ_FLUSH_BUFFERS",
		"IRP_MJ_QUERY_VOLUME_INFORMATION",
		"IRP_MJ_SET_VOLUME_INFORMATION",
		"IRP_MJ_DIRECTORY_CONTROL",
		"IRP_MJ_FILE_SYSTEM_CONTROL",
		"IRP_MJ_DEVICE_CONTROL",
		"IRP_MJ_INTERNAL_DEVICE_CONTROL",
		"IRP_MJ_SHUTDOWN",
		"IRP_MJ_LOCK_CONTROL",
		"IRP_MJ_CLEANUP",
		"IRP_MJ_CREATE_MAILSLOT",
		"IRP_MJ_QUERY_SECURITY",
		"IRP_MJ_SET_SECURITY",
		"IRP_MJ_POWER",
		"IRP_MJ_SYSTEM_CONTROL",
		"IRP_MJ_DEVICE_CHANGE",
		"IRP_MJ_QUERY_QUOTA",
		"IRP_MJ_SET_QUOTA",
		"IRP_MJ_PNP"
	};

	//UCHAR type = IrpStack->MajorFunction;
	//if (type >= arraysize(irpname))
		//KdPrint(("@@@@@@Unknown IRP, major type %X\n", type));
	//else
		//KdPrint(("@@@@@@\t%s\n", irpname[type]));

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	//KdPrint(("------Leave DispatchAny!------\n"));
	test(__FUNCTION__"++\n");
	tnsoftndisDebugPrint(__FUNCTION__"++\n");
	return STATUS_SUCCESS;
}



#define PROJECTNAME		"tnsoftndis - "
#define PRJNAMELEN		(strlen( PROJECTNAME ) )

int tnsoftndisDebugPrint( const char * fmt, ... )
{
	char buf[260] = { PROJECTNAME };
    va_list va;
    int err, max = sizeof buf;
    char * p = buf;
    va_start(va, fmt);    

		return 0;
    p += PRJNAMELEN; max -= PRJNAMELEN;     
    err = _vsnprintf(p, max, fmt, va);
    if( err > 0 ) 
    	DbgPrint(buf);    	
	va_end(va);
	
	return 0;
}

int test( const char * fmt, ... )
{
    int a=0;
    a+=100;
    a+=100;

	char buf[260] = { PROJECTNAME };
    va_list va;
    int err, max = sizeof buf;
    char * p = buf;
    va_start(va, fmt);    

		return 0;
    p += PRJNAMELEN; max -= PRJNAMELEN;     
    err = _vsnprintf(p, max, fmt, va);
    if( err > 0 ) 
    	DbgPrint(buf);    	
	va_end(va);
    return 0;
}
