#include <windows.h>

#include <ntverp.h>

#define VER_FILETYPE                VFT_DRV
#define VER_FILESUBTYPE             VFT2_DRV_SYSTEM
#define VER_FILEDESCRIPTION_STR     "BulkUsb Driver"
#define VER_INTERNALNAME_STR        "bulkusb.sys"
#define VER_ORIGINALFILENAME_STR    "bulkusb.sys"

#include "common.ver"

MofResourceName MOFDATA bulkusb.bmf

