#==========================================================================;
#
#  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
#  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
#  PURPOSE.
#
#  Copyright (c) 1992 - 1999  Microsoft Corporation.  All Rights Reserved.
#
#==========================================================================;
TARGETNAME=TestCap
TARGETTYPE=DRIVER
DRIVERTYPE=WDM
TARGETPATH=obj

C_DEFINES=-DPNP_POWER
MSC_WARNING_LEVEL=/W3 /WX

TARGETLIBS=$(DDK_LIB_PATH)\stream.lib \
           $(DDK_LIB_PATH)\ksguid.lib \
           $(DDK_LIB_PATH)\dxapi.lib

SOURCES=                \
        capmain.c       \
        capirq.c        \
        capvideo.c      \
        capxfer.c       \
        capprop.c       \
        ddraw.c         \
        testcap.rc

