//////////////////////////////////////////////////////////////////////////
// Copyright (c) 2013-2013 TnsoftTech Corporation
//
// Module Name:
//		CopyFileDriver.h
//
// Abstract:
//		This is the header file defining the functions of the kernel mode
//		driver CopyFileDriver.
// 
// Environment:
//		Kernel mode
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <wdm.h>
#ifdef __cplusplus
}
#endif

#include <stdio.h>
#include <windef.h>
#include "User.h"


#define DEVICE_NAME			L"\\Device\\CopyDevice"		// Kernel device name.
#define DEVICE_SYMLINK_NAME L"\\DosDevices\\CopyDevice"	// Device symbolic link name or \\??\\CopyDevice.
#define COPY_TAG			'YPOC'
#define MAX_READ_SIZE		(64*1024) // 64k
#define CP_BUFFER_SIZE		4096
#define arraysize(p)		(sizeof(p)/sizeof((p)[0]))

#define SOURCE_HANDLE		TRUE
#define DEST_HANDLE			FALSE

#define PAGEDCODE			code_seg("PAGE")
#define LOCKEDCODE			code_seg()
#define PAGEDDATA			data_seg("PAGE")
#define LOCKEDDATA			data_seg()
#define INITCODE			code_seg("INIT")
#define INITDATA			data_seg("INIT")


typedef struct _DEVICE_EXTENSION 
{
	PDEVICE_OBJECT fdo;
	UNICODE_STRING DeviceName;
	UNICODE_STRING SymLinkName;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;


extern "C"
NTSTATUS DriverEntry(IN PDRIVER_OBJECT	DriverObject,
					 IN PUNICODE_STRING RegistryPath);

NTSTATUS CreateDevice(IN PDRIVER_OBJECT DriverObject);

NTSTATUS DeviceControl(IN PDEVICE_OBJECT fdo,
					   IN PIRP Irp);

BOOLEAN CharToWchar(IN  PCHAR  CharBuffer, 
					OUT PWCHAR WcharBuffer, 
					IN  ULONG  WcharBufferSize);

NTSTATUS KernelOpenFile(IN PWCHAR FileName,
						IN OUT HANDLE *FileHandle);

NTSTATUS KernelReadFile(IN  HANDLE FileHandle,
						OUT PVOID Buffer,
						IN  ULONG Length,
						IN  LARGE_INTEGER ByteOffset,
						OUT LPDWORD lpBytesReturned);

NTSTATUS KernelCloseFile(IN HANDLE Handle);

NTSTATUS GetDestFileHandle(IN PWCHAR FileName,
						   IN OUT HANDLE *FileHandle);

NTSTATUS KernelGetFileSize(IN HANDLE FileHandle,
						   IN OUT LARGE_INTEGER *FileSize);

NTSTATUS KernelCopyFile(IN PWCHAR pwSourceFile,
						IN PWCHAR pwDestFile);

void Unload(IN PDRIVER_OBJECT DriverObject);

NTSTATUS DispatchAny(IN PDEVICE_OBJECT fdo,
					 IN PIRP Irp);

