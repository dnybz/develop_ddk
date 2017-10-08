//////////////////////////////////////////////////////////////////////////
// Copyright (c) 2013-2013 TnsoftTech Corporation
// Module Name:
//		User.h
// 
// Abstract:
//		Header file which contains the structures, type definitions,
//		and constants that are shared between the kernel mode driver,
//		CopyFileDriver.sys, and the user mode executable, CopyFileTest.exe.
// 
// Environment:
//     Kernel and user mode
//////////////////////////////////////////////////////////////////////////

#ifndef __USER_H__
#define __USER_H__

#ifndef CTL_CODE
#pragma message("CTL_CODE undefined. Include winioctl.h or wdm.h")
#endif

#pragma warning(disable: 4200) // nonstandard extension used : zero-sized array in struct/union


#define COPYDEVICE			"CopyFileDriver"
#define COPYDEVICE_SYS		"CopyFileDriver.sys"
#define DRIVERS 			"drivers"
#define SYS					"sys"
#define COPYDEVICE_NAME		"\\\\.\\CopyDevice"

#define IOCTL_OPEN_FILE CTL_CODE(\
	FILE_DEVICE_UNKNOWN,\
	0x800+0x88,\
	METHOD_BUFFERED,\
	FILE_ANY_ACCESS)

#define IOCTL_READ_FILE CTL_CODE(\
	FILE_DEVICE_UNKNOWN,\
	0x800+0x89,\
	METHOD_BUFFERED,\
	FILE_ANY_ACCESS)

#define IOCTL_CLOSE_FILE CTL_CODE(\
	FILE_DEVICE_UNKNOWN,\
	0x800+0x8A,\
	METHOD_BUFFERED,\
	FILE_ANY_ACCESS)

#define IOCTL_COPY_FILE CTL_CODE(\
	FILE_DEVICE_UNKNOWN,\
	0x800+0x8B,\
	METHOD_BUFFERED,\
	FILE_ANY_ACCESS)

#define IOCTL_GET_FILE_SIZE CTL_CODE(\
	FILE_DEVICE_UNKNOWN,\
	0x800+0x8C,\
	METHOD_BUFFERED,\
	FILE_ANY_ACCESS)


typedef struct _READ_BUFFER
{
	ULONG			ulFileHandle;
	LARGE_INTEGER	ByteOffset;
} READ_BUFFER, *PREAD_BUFFER;

typedef struct _SEND_DATA
{
	ULONG	Size;
	CHAR	szSourceFile[MAX_PATH];
	ULONG	SourceFileLen;
	CHAR	szDestFile[MAX_PATH];
	ULONG	DestFileLen;
} SEND_DATA, *PSEND_DATA;


#endif

