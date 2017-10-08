[Dynamic, Provider("WMIProv"),
 WMI,
 Description("Bulk Usb driver for Intel 82930"),
 guid("{BBA21300-6DD3-11d2-B844-00C04FAD5171}"),
 locale("MS\\0x409")]
class BulkDeviceInformation
{
    [key, read]
     string InstanceName;

    [read] boolean Active;

    [WmiDataId(1),
     read,
     write,
     Description("The DebugPrintLevel property indicates the debug output level of the generic client driver.")
     ]
    uint32 DebugPrintLevel;
	
    [WmiDataId(2),
     read,
     Description("ModelName")
     ]
    string ModelName;

};
