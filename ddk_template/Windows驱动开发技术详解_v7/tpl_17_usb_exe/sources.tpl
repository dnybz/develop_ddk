
TARGETNAME=RwBulk
TARGETTYPE=PROGRAM

UMTYPE=console
UMBASE=0x01000000

TARGETPATH=obj

INCLUDES=..\sys


TARGETLIBS= $(DDK_LIB_PATH)\usbd.lib \
            $(SDK_LIB_PATH)\setupapi.lib

SOURCES=      \
        RwBulk.c  \
        RwBulk.rc

