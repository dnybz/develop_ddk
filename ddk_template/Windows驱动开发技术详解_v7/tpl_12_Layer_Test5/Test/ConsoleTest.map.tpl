 ConsoleTest

 Timestamp is 4e671c04 (Wed Sep 07 15:23:48 2011)

 Preferred load address is 00400000

 Start         Length     Name                   Class
 0001:00000000 00010000H .textbss                DATA
 0002:00000000 00002a3dH .text                   CODE
 0003:00000000 000008c9H .rdata                  DATA
 0003:000008cc 00000180H .rdata$debug            DATA
 0003:00000a4c 00000104H .rtc$IAA                DATA
 0003:00000b50 00000104H .rtc$IMZ                DATA
 0003:00000c54 00000104H .rtc$IZZ                DATA
 0003:00000d58 00000104H .rtc$TAA                DATA
 0003:00000e5c 00000104H .rtc$TMZ                DATA
 0003:00000f60 00000104H .rtc$TZZ                DATA
 0003:00001064 00000000H .edata                  DATA
 0004:00000000 00000104H .CRT$XCA                DATA
 0004:00000104 00000104H .CRT$XCZ                DATA
 0004:00000208 00000104H .CRT$XIA                DATA
 0004:0000030c 00000104H .CRT$XIZ                DATA
 0004:00000410 0000014aH .data                   DATA
 0004:0000055c 000000b4H .bss                    DATA
 0005:00000000 00000028H .idata$2                DATA
 0005:00000028 00000014H .idata$3                DATA
 0005:0000003c 00000104H .idata$4                DATA
 0005:00000140 00000104H .idata$5                DATA
 0005:00000244 00000365H .idata$6                DATA

  Address         Publics by Value              Rva+Base     Lib:Object

 0000:00000000       ___safe_se_handler_table   00000000     <absolute>
 0000:00000000       ___safe_se_handler_count   00000000     <absolute>
 0001:00000000       __enc$textbss$begin        00401000     <linker-defined>
 0001:00010000       __enc$textbss$end          00411000     <linker-defined>
 0002:00000260       _main                      00411260 f   ConsoleTest_main.obj
 0002:000003ba       _printf                    004113ba f   MSVCRT:MSVCR71.dll
 0002:000003c0       __RTC_CheckEsp             004113c0 f   MSVCRT:stack.obj
 0002:000003eb       @_RTC_CheckStackVars@8     004113eb f   MSVCRT:stack.obj
 0002:00000460       __chkstk                   00411460 f   MSVCRT:chkstk.obj
 0002:00000460       __alloca_probe             00411460     MSVCRT:chkstk.obj
 0002:000004ac       __RTC_InitBase             004114ac f   MSVCRT:init.obj
 0002:000004db       __RTC_Shutdown             004114db f   MSVCRT:init.obj
 0002:0000055c       _mainCRTStartup            0041155c f   MSVCRT:crtexe.obj
 0002:00000aae       ?_RTC_Failure@@YAXPAXH@Z   00411aae f   MSVCRT:error.obj
 0002:00000afb       ?_RTC_MemFailure@@YAXPAXHPBX@Z 00411afb f   MSVCRT:error.obj
 0002:00000cf4       ?_RTC_StackFailure@@YAXPAXPBD@Z 00411cf4 f   MSVCRT:error.obj
 0002:00000da3       __RTC_UninitUse            00411da3 f   MSVCRT:error.obj
 0002:00000e72       __RTC_NumErrors            00411e72 f   MSVCRT:userapi.obj
 0002:00000e76       __RTC_GetErrDesc           00411e76 f   MSVCRT:userapi.obj
 0002:00000e94       __RTC_SetErrorType         00411e94 f   MSVCRT:userapi.obj
 0002:00000ebd       __RTC_SetErrorFunc         00411ebd f   MSVCRT:userapi.obj
 0002:00000ed1       ?_RTC_GetErrorFunc@@YAP6AHHPBDH00ZZPBX@Z 00411ed1 f   MSVCRT:userapi.obj
 0002:00000ed8       __CRT_RTC_INIT             00411ed8 f   MSVCRT:MSVCR71.dll
 0002:00000ede       __c_exit                   00411ede f   MSVCRT:MSVCR71.dll
 0002:00000ee4       __exit                     00411ee4 f   MSVCRT:MSVCR71.dll
 0002:00000eea       __XcptFilter               00411eea f   MSVCRT:MSVCR71.dll
 0002:00000ef0       __cexit                    00411ef0 f   MSVCRT:MSVCR71.dll
 0002:00000ef6       _exit                      00411ef6 f   MSVCRT:MSVCR71.dll
 0002:00000efc       ___p___initenv             00411efc f   MSVCRT:MSVCR71.dll
 0002:00000f02       __amsg_exit                00411f02 f   MSVCRT:MSVCR71.dll
 0002:00000f08       ___getmainargs             00411f08 f   MSVCRT:MSVCR71.dll
 0002:00000f0e       __onexit                   00411f0e f   MSVCRT:atonexit.obj
 0002:00000f3d       _atexit                    00411f3d f   MSVCRT:atonexit.obj
 0002:00000f53       __RTC_Initialize           00411f53 f   MSVCRT:initsect.obj
 0002:00000fa8       __RTC_Terminate            00411fa8 f   MSVCRT:initsect.obj
 0002:00000ffe       __initterm                 00411ffe f   MSVCRT:MSVCR71.dll
 0002:00001004       __setdefaultprecision      00412004 f   MSVCRT:fp8.obj
 0002:0000101a       ___setusermatherr          0041201a f   MSVCRT:MSVCR71.dll
 0002:00001020       __matherr                  00412020 f   MSVCRT:merr.obj
 0002:00001023       __setargv                  00412023 f   MSVCRT:dllargv.obj
 0002:00001026       ___p__commode              00412026 f   MSVCRT:MSVCR71.dll
 0002:0000102c       ___p__fmode                0041202c f   MSVCRT:MSVCR71.dll
 0002:00001032       ___set_app_type            00412032 f   MSVCRT:MSVCR71.dll
 0002:00001038       __SEH_prolog               00412038 f   MSVCRT:sehprolg.obj
 0002:00001073       __SEH_epilog               00412073 f   MSVCRT:sehprolg.obj
 0002:00001098       __except_handler3          00412098 f   MSVCRT:MSVCR71.dll
 0002:00001649       ?_RTC_GetSrcLine@@YAHKPADHPAHPAPAD@Z 00412649 f   MSVCRT:pdblkup.obj
 0002:00001a6c       ___dllonexit               00412a6c f   MSVCRT:MSVCR71.dll
 0002:00001a72       __controlfp                00412a72 f   MSVCRT:MSVCR71.dll
 0002:00001a78       _CloseHandle@4             00412a78 f   kernel32:KERNEL32.dll
 0002:00001a7e       _ReadFile@20               00412a7e f   kernel32:KERNEL32.dll
 0002:00001a84       _GetLastError@0            00412a84 f   kernel32:KERNEL32.dll
 0002:00001a8a       _CreateFileA@28            00412a8a f   kernel32:KERNEL32.dll
 0002:00001a90       _GetModuleHandleA@4        00412a90 f   kernel32:KERNEL32.dll
 0002:00001a96       _RaiseException@16         00412a96 f   kernel32:KERNEL32.dll
 0002:00001a9c       _GetVersionExA@4           00412a9c f   kernel32:KERNEL32.dll
 0002:00001aa2       _GetProcAddress@8          00412aa2 f   kernel32:KERNEL32.dll
 0002:00001aa8       _LoadLibraryA@4            00412aa8 f   kernel32:KERNEL32.dll
 0002:00001aae       _DebugBreak@0              00412aae f   kernel32:KERNEL32.dll
 0002:00001ab4       _GetModuleFileNameA@12     00412ab4 f   kernel32:KERNEL32.dll
 0002:00001aba       _HeapAlloc@12              00412aba f   kernel32:KERNEL32.dll
 0002:00001ac0       _GetCurrentProcess@0       00412ac0 f   kernel32:KERNEL32.dll
 0002:00001ac6       _HeapFree@12               00412ac6 f   kernel32:KERNEL32.dll
 0002:00001acc       _GetProcessHeap@0          00412acc f   kernel32:KERNEL32.dll
 0002:00001ad2       _FreeLibrary@4             00412ad2 f   kernel32:KERNEL32.dll
 0003:00000020       ??_C@_0EC@FLCPNAEB@Failed?5to?5obtain?5file?5handle?5to?5@ 00414020     ConsoleTest_main.obj
 0003:00000070       ??_C@_0O@LGOPJFNE@?2?2?4?2HelloDDKA?$AA@ 00414070     ConsoleTest_main.obj
 0003:000000a8       ??_C@_0BO@GNIAFIKK@Unknown?5Runtime?5Check?5Error?6?$AN?$AA@ 004140a8     MSVCRT:error.obj
 0003:000000cc       ??_C@_0DG@HKJMLLLP@A?5local?5variable?5was?5used?5before@ 004140cc     MSVCRT:error.obj
 0003:0000010c       ??_C@_0BN@FFOINMNJ@Stack?5memory?5was?5corrupted?6?$AN?$AA@ 0041410c     MSVCRT:error.obj
 0003:00000130       ??_C@_0BBN@GPMLNJCF@A?5cast?5to?5a?5smaller?5data?5type?5ha@ 00414130     MSVCRT:error.obj
 0003:00000288       ??_C@_0NN@NGPKDKPD@The?5value?5of?5ESP?5was?5not?5properl@ 00414288     MSVCRT:error.obj
 0003:000003b8       ??_C@_0BC@KNBMEBEO@IsDebuggerPresent?$AA@ 004143b8     MSVCRT:error.obj
 0003:000003d0       ??_C@_0N@HAOHNMDE@Kernel32?4dll?$AA@ 004143d0     MSVCRT:error.obj
 0003:000003e0       ??_C@_0CA@IODNCDPG@Run?9Time?5Check?5Failure?5?$CD?$CFd?5?9?5?$CFs?$AA@ 004143e0     MSVCRT:error.obj
 0003:00000408       ??_C@_0L@CJCNHJJG@?6?$ANModule?3?7?$AA@ 00414408     MSVCRT:error.obj
 0003:00000418       ??_C@_08GJEJCAJD@?6?$ANLine?3?7?$AA@ 00414418     MSVCRT:error.obj
 0003:00000424       ??_C@_0CI@INCLAPNB@Invalid?5pointer?5was?5assigned?5at?6@ 00414424     MSVCRT:error.obj
 0003:00000454       ??_C@_0BB@JEGFEAAM@?8?5was?5corrupted?4?$AA@ 00414454     MSVCRT:error.obj
 0003:00000468       ??_C@_0BM@FGFAMDLF@Stack?5around?5the?5variable?5?8?$AA@ 00414468     MSVCRT:error.obj
 0003:0000048c       ??_C@_0DA@BEGFPCGG@A?5variable?5is?5being?5used?5without@ 0041448c     MSVCRT:error.obj
 0003:000004c8       ??_C@_0CH@LOIIHGKM@?8?5is?5being?5used?5without?5being?5de@ 004144c8     MSVCRT:error.obj
 0003:000004f8       ??_C@_0P@HLKMHJCG@The?5variable?5?8?$AA@ 004144f8     MSVCRT:error.obj
 0003:0000050c       ??_C@_0CK@CNLNOEPB@Local?5variable?5used?5before?5initi@ 0041450c     MSVCRT:userapi.obj
 0003:00000540       ??_C@_0BI@CIGMDCBH@Stack?5memory?5corruption?$AA@ 00414540     MSVCRT:userapi.obj
 0003:0000055c       ??_C@_0CK@FEGOIOPB@Cast?5to?5smaller?5type?5causing?5los@ 0041455c     MSVCRT:userapi.obj
 0003:00000590       ??_C@_0BJ@HEGAHDFO@Stack?5pointer?5corruption?$AA@ 00414590     MSVCRT:userapi.obj
 0003:000005d0       ??_C@_0M@HHEBEAAO@MSPDB71?4DLL?$AA@ 004145d0     MSVCRT:pdblkup.obj
 0003:000005e0       ??_C@_0N@KIGOLBLC@Module32Next?$AA@ 004145e0     MSVCRT:pdblkup.obj
 0003:000005f0       ??_C@_0O@CNMFJJGO@Module32First?$AA@ 004145f0     MSVCRT:pdblkup.obj
 0003:00000600       ??_C@_0BF@OCLFCNKN@GetModuleInformation?$AA@ 00414600     MSVCRT:pdblkup.obj
 0003:0000061c       ??_C@_0BD@FIEJBLME@EnumProcessModules?$AA@ 0041461c     MSVCRT:pdblkup.obj
 0003:00000634       ??_C@_09EPMADGLA@PSAPI?4DLL?$AA@ 00414634     MSVCRT:pdblkup.obj
 0003:00000640       ??_C@_0BJ@JEFKDHMD@CreateToolhelp32Snapshot?$AA@ 00414640     MSVCRT:pdblkup.obj
 0003:00000660       ??_C@_0O@CHDGJGFP@ImageNtHeader?$AA@ 00414660     MSVCRT:pdblkup.obj
 0003:00000670       ??_C@_0N@KBGMOMJO@IMAGEHLP?4DLL?$AA@ 00414670     MSVCRT:pdblkup.obj
 0003:00000680       ??_C@_0N@NHGLKGEC@KERNEL32?4DLL?$AA@ 00414680     MSVCRT:pdblkup.obj
 0003:00000690       ??_C@_0BF@BMIHBHCA@EnvironmentDirectory?$AA@ 00414690     MSVCRT:pdblkup.obj
 0003:000006ac       ??_C@_0CN@KGLAKABH@SOFTWARE?2Microsoft?2VisualStudio?2@ 004146ac     MSVCRT:pdblkup.obj
 0003:000006e4       ??_C@_0M@HLOHPNFA@RegCloseKey?$AA@ 004146e4     MSVCRT:pdblkup.obj
 0003:000006f4       ??_C@_0BB@HHEIPGME@RegQueryValueExA?$AA@ 004146f4     MSVCRT:pdblkup.obj
 0003:00000708       ??_C@_0O@DCOGKJGG@RegOpenKeyExA?$AA@ 00414708     MSVCRT:pdblkup.obj
 0003:00000718       ??_C@_0N@JJPEKNFO@ADVAPI32?4DLL?$AA@ 00414718     MSVCRT:pdblkup.obj
 0003:00000728       ??_C@_01KDCPPGHE@r?$AA@    00414728     MSVCRT:pdblkup.obj
 0003:0000072a       ??_C@_00CNPNBAHC@?$AA@     0041472a     MSVCRT:pdblkup.obj
 0003:0000072c       ??_C@_08ONJBEOFA@PDBClose?$AA@ 0041472c     MSVCRT:pdblkup.obj
 0003:00000738       ??_C@_08NINFAAFA@DBIClose?$AA@ 00414738     MSVCRT:pdblkup.obj
 0003:00000744       ??_C@_08LIGPJJNA@ModClose?$AA@ 00414744     MSVCRT:pdblkup.obj
 0003:00000750       ??_C@_0O@IPLOKOKN@ModQueryLines?$AA@ 00414750     MSVCRT:pdblkup.obj
 0003:00000760       ??_C@_0BE@GDNPNLGH@DBIQueryModFromAddr?$AA@ 00414760     MSVCRT:pdblkup.obj
 0003:00000778       ??_C@_0L@KPMOCKEL@PDBOpenDBI?$AA@ 00414778     MSVCRT:pdblkup.obj
 0003:00000788       ??_C@_0BB@PENKIMFE@PDBOpenValidate3?$AA@ 00414788     MSVCRT:pdblkup.obj
 0003:00000a4c       ___rtc_iaa                 00414a4c     MSVCRT:initsect.obj
 0003:00000c54       ___rtc_izz                 00414c54     MSVCRT:initsect.obj
 0003:00000d58       ___rtc_taa                 00414d58     MSVCRT:initsect.obj
 0003:00000f60       ___rtc_tzz                 00414f60     MSVCRT:initsect.obj
 0004:00000000       ___xc_a                    00416000     MSVCRT:cinitexe.obj
 0004:00000104       ___xc_z                    00416104     MSVCRT:cinitexe.obj
 0004:00000208       ___xi_a                    00416208     MSVCRT:cinitexe.obj
 0004:0000030c       ___xi_z                    0041630c     MSVCRT:cinitexe.obj
 0004:00000410       ?_RTC_ErrorLevels@@3PAHA   00416410     MSVCRT:error.obj
 0004:00000450       ___defaultmatherr          00416450     MSVCRT:merr.obj
 0004:00000580       __dowildcard               00416580     MSVCRT:wildcard.obj
 0004:00000584       __newmode                  00416584     MSVCRT:_newmode.obj
 0004:00000588       __commode                  00416588     MSVCRT:xncommod.obj
 0004:0000058c       __fmode                    0041658c     MSVCRT:xtxtmode.obj
 0004:000005f0       __adjust_fdiv              004165f0     <common>
 0004:000005fc       ___onexitend               004165fc     <common>
 0004:0000060c       ___onexitbegin             0041660c     <common>
 0005:00000000       __IMPORT_DESCRIPTOR_MSVCR71 00417000     MSVCRT:MSVCR71.dll
 0005:00000014       __IMPORT_DESCRIPTOR_KERNEL32 00417014     kernel32:KERNEL32.dll
 0005:00000028       __NULL_IMPORT_DESCRIPTOR   00417028     MSVCRT:MSVCR71.dll
 0005:00000140       __imp__GetProcessHeap@0    00417140     kernel32:KERNEL32.dll
 0005:00000144       __imp__HeapFree@12         00417144     kernel32:KERNEL32.dll
 0005:00000148       __imp__GetCurrentProcess@0 00417148     kernel32:KERNEL32.dll
 0005:0000014c       __imp__HeapAlloc@12        0041714c     kernel32:KERNEL32.dll
 0005:00000150       __imp__GetModuleFileNameA@12 00417150     kernel32:KERNEL32.dll
 0005:00000154       __imp__DebugBreak@0        00417154     kernel32:KERNEL32.dll
 0005:00000158       __imp__LoadLibraryA@4      00417158     kernel32:KERNEL32.dll
 0005:0000015c       __imp__GetProcAddress@8    0041715c     kernel32:KERNEL32.dll
 0005:00000160       __imp__GetVersionExA@4     00417160     kernel32:KERNEL32.dll
 0005:00000164       __imp__RaiseException@16   00417164     kernel32:KERNEL32.dll
 0005:00000168       __imp__GetModuleHandleA@4  00417168     kernel32:KERNEL32.dll
 0005:0000016c       __imp__CreateFileA@28      0041716c     kernel32:KERNEL32.dll
 0005:00000170       __imp__GetLastError@0      00417170     kernel32:KERNEL32.dll
 0005:00000174       __imp__ReadFile@20         00417174     kernel32:KERNEL32.dll
 0005:00000178       __imp__CloseHandle@4       00417178     kernel32:KERNEL32.dll
 0005:0000017c       __imp__FreeLibrary@4       0041717c     kernel32:KERNEL32.dll
 0005:00000180       \177KERNEL32_NULL_THUNK_DATA 00417180     kernel32:KERNEL32.dll
 0005:000001b8       __imp___except_handler3    004171b8     MSVCRT:MSVCR71.dll
 0005:000001bc       __imp___onexit             004171bc     MSVCRT:MSVCR71.dll
 0005:000001c0       __imp___controlfp          004171c0     MSVCRT:MSVCR71.dll
 0005:000001c4       __imp____set_app_type      004171c4     MSVCRT:MSVCR71.dll
 0005:000001c8       __imp____p__fmode          004171c8     MSVCRT:MSVCR71.dll
 0005:000001cc       __imp____p__commode        004171cc     MSVCRT:MSVCR71.dll
 0005:000001d0       __imp___adjust_fdiv        004171d0     MSVCRT:MSVCR71.dll
 0005:000001d4       __imp____setusermatherr    004171d4     MSVCRT:MSVCR71.dll
 0005:000001d8       __imp___initterm           004171d8     MSVCRT:MSVCR71.dll
 0005:000001dc       __imp____getmainargs       004171dc     MSVCRT:MSVCR71.dll
 0005:000001e0       __imp___amsg_exit          004171e0     MSVCRT:MSVCR71.dll
 0005:000001e4       __imp____p___initenv       004171e4     MSVCRT:MSVCR71.dll
 0005:000001e8       __imp__exit                004171e8     MSVCRT:MSVCR71.dll
 0005:000001ec       __imp___cexit              004171ec     MSVCRT:MSVCR71.dll
 0005:000001f0       __imp___XcptFilter         004171f0     MSVCRT:MSVCR71.dll
 0005:000001f4       __imp___exit               004171f4     MSVCRT:MSVCR71.dll
 0005:000001f8       __imp___c_exit             004171f8     MSVCRT:MSVCR71.dll
 0005:000001fc       __imp___CRT_RTC_INIT       004171fc     MSVCRT:MSVCR71.dll
 0005:00000200       __imp____dllonexit         00417200     MSVCRT:MSVCR71.dll
 0005:00000204       __imp__printf              00417204     MSVCRT:MSVCR71.dll
 0005:00000208       \177MSVCR71_NULL_THUNK_DATA 00417208     MSVCRT:MSVCR71.dll

 entry point at        0002:000000c8

