
#ifndef __COPYFILEWRAPPER_H__
#define __COPYFILEWRAPPER_H__ 


#ifdef COPYFILEWRAPPER_EXPORTS
#define COPYFILEWRAPPER_API __declspec(dllexport)
#pragma message("+++++++++++++++++++dllexport")
#else
#define COPYFILEWRAPPER_API __declspec(dllimport)
#pragma message("-------------------dllimport")
#endif


#pragma warning(disable: 4127) // conditional expression is constant
#pragma warning(disable: 4201) // nonstandard extension used : nameless struct/union


COPYFILEWRAPPER_API DWORD __stdcall IsDriverInstalled(IN LPCSTR DriverName, OUT BOOL *pInstall);
COPYFILEWRAPPER_API BOOL __stdcall InstallInfFile(IN LPCSTR InfFilePath);
COPYFILEWRAPPER_API BOOL __stdcall InstallDriver(IN LPCSTR DriverName, IN LPCSTR DriverPath);
COPYFILEWRAPPER_API BOOL __stdcall UninstallDriver(IN LPCSTR DriverName);
COPYFILEWRAPPER_API BOOL __stdcall StartDriver(IN LPCSTR DriverName);
COPYFILEWRAPPER_API BOOL __stdcall KernelCopyFile(IN LPCSTR pszSourceFile, IN LPCSTR pszDestFile);


COPYFILEWRAPPER_API HANDLE __stdcall KernelOpenFile(IN LPCSTR FileName);
COPYFILEWRAPPER_API BOOL __stdcall KernelReadFile(IN  HANDLE	FileHandle,
												  OUT PVOID		Buffer,
												  IN  ULONG		Length,
												  IN  LONGLONG	ReadOffset,
												  OUT LPDWORD	lpBytesReturned);

COPYFILEWRAPPER_API BOOL __stdcall KernelCloseHandle(IN HANDLE FileHandle);

COPYFILEWRAPPER_API BOOL __stdcall KernelGetFileSize(IN HANDLE FileHandle,
													 IN OUT LONGLONG *pFileSize);


#endif
