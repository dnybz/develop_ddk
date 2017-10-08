;
; Testcap.INF  -- This file contains installation information for Testcap,
;                 A sample WDM capture driver that requires no hardware
;               
;                 Copyright (c) 1997-2001 Microsoft Corporation
;
;

[Version]
signature="$CHICAGO$"
Class=Media
ClassGUID={4d36e96c-e325-11ce-bfc1-08002be10318}
Provider=%ProviderName%
DriverVer=3/1/2001,1.0
CatalogFile=testcap.cat

[ControlFlags]
;ExcludeFromSelect=*

[Manufacturer]
%MsftMfg%=Microsoft

[DestinationDirs]
Testcap.CopyDrivers=10,system32\drivers

[SourceDisksNames]
1=%TESTCAP.Disk1%,disk1

[SourceDisksFiles]
testcap.sys=1

;---------------------------------------------------------------
;              M I C R O S O F T
;
;                 T E S T C A P 
;---------------------------------------------------------------

[Microsoft]
%TESTCAP.DeviceDesc%=Testcap.Device,TESTCAP

[Testcap.Device]
Include=    ks.inf,kscaptur.inf
Needs=      Stream.Registration,KS.Registration,KSCAPTUR.Registration
AddReg=     Testcap.AddReg
CopyFiles=  Testcap.CopyDrivers
KnownFiles= Testcap.KnownFiles

[Testcap.Device.NT]
Include=    ks.inf,kscaptur.inf
Needs=      Stream.Registration,KS.Registration,KSCAPTUR.Registration.NT
AddReg=     Testcap.AddReg
CopyFiles=  Testcap.CopyDrivers

[Testcap.AddReg]
HKR,,DevLoader,,*ntkern
HKR,,NTMPDriver,,testcap.sys
HKR,,PageOutWhenUnopened,3,01
HKR,,DontSuspendIfStreamsAreRunning,3,01

[Testcap.CopyDrivers]
testcap.sys

[Testcap.KnownFiles]
testcap.sys=safe

[TESTCAP.Device.Interfaces]
AddInterface={65e8773d-8f56-11d0-a3b9-00a0c9223196},"GLOBAL",VideoCapture,
AddInterface={6994AD05-93EF-11D0-A3CC-00A0C9223196},"GLOBAL",VideoCapture,
AddInterface={a799a800-a46d-11d0-a18c-00a02401dcd4},"GLOBAL",VideoTvTuner,
AddInterface={a799a801-a46d-11d0-a18c-00a02401dcd4},"GLOBAL",VideoCrossbar,
AddInterface={a799a802-a46d-11d0-a18c-00a02401dcd4},"GLOBAL",VideoTvAudio,

[TESTCAP.Device.NT.Interfaces]
AddInterface={65e8773d-8f56-11d0-a3b9-00a0c9223196},"GLOBAL",VideoCapture,
AddInterface={6994AD05-93EF-11D0-A3CC-00A0C9223196},"GLOBAL",VideoCapture,
AddInterface={a799a800-a46d-11d0-a18c-00a02401dcd4},"GLOBAL",VideoTvTuner,
AddInterface={a799a801-a46d-11d0-a18c-00a02401dcd4},"GLOBAL",VideoCrossbar,
AddInterface={a799a802-a46d-11d0-a18c-00a02401dcd4},"GLOBAL",VideoTvAudio,

[VideoCapture]
AddReg=VideoCapture.AddReg

[VideoTvTuner]
AddReg=VideoTvTuner.AddReg

[VideoCrossbar]
AddReg=VideoXBar.AddReg

[VideoTvAudio]
AddReg=VideoTvAudio.AddReg


[VideoCapture.AddReg]
HKR,,CLSID,,{17CCA71B-ECD7-11D0-B908-00A0C9223196}
HKR,,FriendlyName,,%TESTCAP.Capture%
; IAMVideoCompression installed as static interface on both output pins
HKR,PinFactory\0\Interfaces\{C6E13343-30AC-11d0-A18C-00A0C9118956},,,{C6E13343-30AC-11d0-A18C-00A0C9118956}
HKR,PinFactory\1\Interfaces\{C6E13343-30AC-11d0-A18C-00A0C9118956},,,{C6E13343-30AC-11d0-A18C-00A0C9118956}
HKLM,Software\CLASSES\CLSID\%TESTCAP.AMcatID%\Instance\%TESTCAP.VBIcatID%
HKLM,Software\CLASSES\CLSID\%TESTCAP.AMcatID%\Instance\%TESTCAP.VBIcatID%,,,%TESTCAP.WDMcodecs%
HKLM,Software\CLASSES\CLSID\%TESTCAP.AMcatID%\Instance\%TESTCAP.VBIcatID%,FriendlyName,,%TESTCAP.WDMcodecs%
HKLM,Software\CLASSES\CLSID\%TESTCAP.AMcatID%\Instance\%TESTCAP.VBIcatID%,CLSID,,"%TESTCAP.VBIcatID%"
HKLM,System\CurrentControlSet\Control\MediaCategories\%TESTCAP.CCpinID%
HKLM,System\CurrentControlSet\Control\MediaCategories\%TESTCAP.CCpinID%,Name,,%TESTCAP.CCpinName%
HKLM,System\CurrentControlSet\Control\MediaCategories\%TESTCAP.NABpinID%
HKLM,System\CurrentControlSet\Control\MediaCategories\%TESTCAP.NABpinID%,Name,,%TESTCAP.NABpinName%


[VideoTvTuner.AddReg]
HKR,,CLSID,,{266EEE40-6C63-11cf-8A03-00AA006ECB65}
HKR,,FriendlyName,,%TESTCAP.TvTuner%
    
[VideoXBar.AddReg]
HKR,,CLSID,,{71F96460-78F3-11d0-A18C-00A0C9118956}
HKR,,FriendlyName,,%TESTCAP.Crossbar%

[VideoTvAudio.AddReg]
HKR,,CLSID,,{71F96462-78F3-11d0-A18C-00A0C9118956}
HKR,,FriendlyName,,%TESTCAP.TvAudio%

[Testcap.Device.NT.Services]
AddService = TESTCAP,0x00000002,Testcap.ServiceInstall

[Testcap.ServiceInstall]
DisplayName   = %TESTCAP.DeviceDesc%
ServiceType   = %SERVICE_KERNEL_DRIVER%
StartType     = %SERVICE_AUTO_START%
ErrorControl  = %SERVICE_ERROR_NORMAL%
ServiceBinary = %12%\testcap.sys

;---------------------------------------------------------------
;                S t r i n g s
;---------------------------------------------------------------

[Strings]

;----------------
; Non-Localizable
;----------------

SERVICE_KERNEL_DRIVER=1
SERVICE_AUTO_START=2
SERVICE_ERROR_NORMAL=1


;----------------
; Localizable
;----------------

ProviderName=       "YourNameHere"
MsftMfg=            "Microsoft"
TESTCAP.Disk1=      "Install Disk"
TESTCAP.DeviceDesc= "Testcap, WDM Video Capture"
TESTCAP.Capture=    "TestCap Capture"
TESTCAP.TvTuner=    "TestCap TvTuner"
TESTCAP.Crossbar=   "TestCap Crossbar"
TESTCAP.TvAudio=    "TestCap TvAudio"
TESTCAP.VBIcatID=   "{07DAD660-22F1-11d1-A9F4-00C04FBBDE8F}"
TESTCAP.AMcatID=    "{DA4E3DA0-D07D-11d0-BD50-00A0C911CE86}"
TESTCAP.WDMcodecs=  "WDM Streaming VBI Codecs"
TESTCAP.CCpinID=    "{1AAD8061-012D-11d2-B4B1-00A0D102CFBE}"
TESTCAP.CCpinName=  "HWCC"
TESTCAP.NABpinID=   "{29703660-498A-11d2-B4B1-00A0D102CFBE}"
TESTCAP.NABpinName= "HWNABTS"

