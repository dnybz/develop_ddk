// IOCTLS.H -- IOCTL code definitions for fileio driver
// Copyright (C) 1999 by Walter Oney
// All rights reserved

#ifndef IOCTLS_H
#define IOCTLS_H

#ifndef CTL_CODE
	#pragma message("CTL_CODE undefined. Include winioctl.h or wdm.h")
#endif

//读取BAR0基地址
#define IOCTL_READ_BASE_BAR0 CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x800, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

//读取BAR1基地址
#define IOCTL_READ_BASE_BAR1 CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x801, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)


//读取BAR2基地址
#define IOCTL_READ_BASE_BAR2 CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x802, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

//读取BAR0基地址
#define IOCTL_WRITE_BASE_BAR0 CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x803, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

//读取BAR1基地址
#define IOCTL_WRITE_BASE_BAR1 CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x804, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)


//读取BAR2基地址
#define IOCTL_WRITE_BASE_BAR2 CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x805, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

//读取图像
#define IOCTL_READ_IMAGE CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x806, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

//写图像
#define IOCTL_WRITE_IMAGE CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x807, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

//使能中断
#define IOCTL_ENABLE_INT CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x808, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

//关中断
#define IOCTL_DISABLE_INT CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x809, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

#endif
