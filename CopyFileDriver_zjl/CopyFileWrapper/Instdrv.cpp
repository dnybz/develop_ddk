
#include <Windows.h>
#include <WinSvc.h>
#include <assert.h>
//#include <WinIoCtl.h>
#include "..\CopyFileDriver\User.h"
// #include "User.h"

#include "CopyFileWrapper.h"
#include <stdio.h>


BOOL __stdcall InstallService(IN LPCSTR DriverName, IN LPCSTR DriverPath);
BOOL __stdcall StartDriver(IN LPCSTR DriverName);

BOOL __stdcall StopDriver(IN LPCSTR DriverName);
BOOL __stdcall RemoveDriver(IN LPCSTR DriverName);

BOOL __stdcall OpenDevice(IN LPCSTR DriverName, OUT HANDLE *lphDevice);


//////////////////////////////////////////////////////////////////////////
// Add new by David at 20110518.

//------------------------------------------------------------------------
// Custom Function OutputDebugString, used to print debug information.
//------------------------------------------------------------------------
int My_OutputDebugString(CHAR * Format, ...)
{
	va_list		pArgList = NULL;	
	int			Length = 0;
	CHAR		Buffer[2048] = {0}; 
	size_t		MaxCount = sizeof(Buffer);
	size_t		SizeInBytes = MaxCount/sizeof(CHAR);

	if (Format == NULL) 
		return -1;

	va_start(pArgList, Format); // pArgList point to first parameter after Format.

	//Length = _vsnprintf_s(Buffer, SizeInBytes, MaxCount, Format, pArgList);

	//Length = _vsnprintf_s(Buffer, SizeInBytes, MaxCount, Format, pArgList);

	//cb = _vsnprintf(buffer, sizeof(buffer), Format, arglist);
	Length = _vsnprintf(Buffer, SizeInBytes,  Format, pArgList);

	va_end(pArgList);

	if (Length <= 0) 
		return Length;

	OutputDebugString(Buffer);
	return Length;
}


DWORD __stdcall IsDriverInstalled(IN LPCSTR DriverName, OUT BOOL *pInstall)
{
	SC_HANDLE	hSCManager = NULL;
	SC_HANDLE	hService = NULL;
	DWORD		dwError = ERROR_SUCCESS;

	if (DriverName == NULL || pInstall == NULL)
		return ERROR_INVALID_PARAMETER;

	do 
	{
		hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCManager == NULL)
		{
			dwError = GetLastError();
			break;
		}

		hService = OpenService(hSCManager, DriverName, SERVICE_ALL_ACCESS);
		if (hService == NULL)
		{
			dwError = GetLastError();
			if (dwError == ERROR_SERVICE_DOES_NOT_EXIST)
			{
				dwError = ERROR_SUCCESS;
				*pInstall = FALSE; // no install
			}
		} 
		else
		{
			dwError = GetLastError();
			*pInstall = TRUE; // install
		}

	} while (0);

	if (hService != NULL)
		CloseServiceHandle(hService);

	if (hSCManager != NULL)
		CloseServiceHandle(hSCManager);

	return dwError;
}

BOOL __stdcall InstallInfFile(IN LPCSTR InfFilePath)
{
	UINT	uiRet = 0;
	TCHAR	CmdLineBuffer[MAX_PATH] = {0};

	wsprintf(CmdLineBuffer, TEXT("rundll32.exe setupapi,InstallHinfSection DefaultInstall 132 %s"), InfFilePath); 
	uiRet = WinExec(CmdLineBuffer, SW_HIDE);

	if (uiRet > 31)
		return TRUE;
	else 
		return FALSE;
}

BOOL __stdcall InstallDriver(IN LPCSTR DriverName, IN LPCSTR DriverPath)
{
	BOOL	bRet = FALSE;
	DWORD	dwError = 0;
	BOOL	bInstall = FALSE;

	dwError = IsDriverInstalled(DriverName, &bInstall);
	if (dwError != ERROR_SUCCESS)
		return FALSE;
	
	if (!bInstall)
	{
		//
		// No install, install service first.
		//
		bRet = InstallService(DriverName, DriverPath);
		if (!bRet)
			return FALSE;
	}

	bRet = StartDriver(DriverName);
	if (!bRet)
	{
		dwError = GetLastError();
		if (dwError == ERROR_SERVICE_ALREADY_RUNNING)
			bRet = TRUE;
	}

	return bRet;
}

BOOL __stdcall UninstallDriver(IN LPCSTR DriverName)
{
	BOOL bRet = FALSE;

	bRet = StopDriver(DriverName);
	if (!bRet)
		return FALSE;

	bRet = RemoveDriver(DriverName);

	return bRet;
}

BOOL __stdcall InstallService(IN LPCSTR DriverName, IN LPCSTR DriverPath)
{
	SC_HANDLE	hSCManager = NULL;
	SC_HANDLE	hService = NULL;
	BOOL		bRet = TRUE;
	DWORD		dwError = 0;

	do 
	{
		hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCManager == NULL)
		{
			bRet = FALSE;
			break;
		}

		hService = CreateService(hSCManager,				// SCM handle
								 DriverName,				// name of service in register
								 DriverName,				// name to display
								 SERVICE_ALL_ACCESS,		// access authority
								 SERVICE_KERNEL_DRIVER,		// driver service
								 SERVICE_DEMAND_START,		// Start
								 SERVICE_ERROR_NORMAL,		// ErrorControl
								 DriverPath,				// ImagePath
								 NULL,						// no load ordering group
								 NULL,						// no tag identifier
								 NULL,						// no dependencies
								 NULL,						// LocalSystem account
								 NULL);						// no password

		if (hService == NULL)
		{
			dwError = GetLastError();
			if (dwError == ERROR_IO_PENDING || dwError == ERROR_SERVICE_EXISTS)
				bRet = TRUE;
			else
				bRet = FALSE;
		}

	} while (0);

	if (hService != NULL) 
		CloseServiceHandle(hService);

	if (hSCManager != NULL) 
		CloseServiceHandle(hSCManager);

	return bRet;
}

BOOL __stdcall StartDriver(IN LPCSTR DriverName)
{
	SC_HANDLE	hSCManager = NULL;
	SC_HANDLE	hService = NULL;
	BOOL		bRet = FALSE;
	DWORD		dwError = 0;

	do 
	{
		hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCManager == NULL)
		{
			bRet = FALSE;
			break;
		}

		hService = OpenService(hSCManager, DriverName, SERVICE_ALL_ACCESS);
		if (hService == NULL)
		{
			bRet = FALSE;
			break;
		}

		bRet = StartService(hService, 0, NULL);
		if (!bRet)
		{
			dwError = GetLastError();
			if (dwError == ERROR_SERVICE_ALREADY_RUNNING || dwError == ERROR_SERVICE_DISABLED)
				bRet = TRUE;
		}

	} while (0);
	
	if (hService != NULL) 
		CloseServiceHandle(hService);

	if (hSCManager != NULL)
		CloseServiceHandle(hSCManager);

	return bRet;
}

BOOL __stdcall StopDriver(IN LPCSTR DriverName)
{
	SC_HANDLE		hSCManager = NULL; 
	SC_HANDLE		hService = NULL;
	BOOL			bRet = FALSE;
	DWORD			dwError = 0;
	SERVICE_STATUS	ServiceStatus = {0};

	do 
	{
		hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCManager == NULL)
		{
			bRet = FALSE;
			break;
		}

		hService = OpenService(hSCManager, DriverName, SERVICE_ALL_ACCESS);
		if (hService == NULL)
		{
			bRet = FALSE;
			break;
		}

		bRet = ControlService(hService, SERVICE_CONTROL_STOP, &ServiceStatus);
		if (!bRet)
		{
			dwError = GetLastError(); 
			break;
		}

	} while (0);

	if (hService != NULL)
		CloseServiceHandle(hService);

	if (hSCManager != NULL) 
		CloseServiceHandle(hSCManager);

	return bRet;
}

BOOL __stdcall RemoveDriver(IN LPCSTR DriverName)
{
	SC_HANDLE	hSCManager = NULL;
	SC_HANDLE	hService = NULL;
	BOOL		bRet = FALSE;
	DWORD		dwError = 0;

	do 
	{
		hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCManager == NULL)
		{
			bRet = FALSE;
			break;
		}

		hService = OpenService(hSCManager, DriverName, SERVICE_ALL_ACCESS);
		if (hService == NULL)
		{
			dwError = GetLastError();
			if ( dwError != ERROR_SUCCESS )
			{
				//AfxMessageBox( _T("OpenService error!\n") );
				My_OutputDebugString( "OpenService error, dwError = %ld\n", dwError );
			}

			bRet = FALSE;
			break;
		}

		bRet = DeleteService(hService);
		if (!bRet)
		{
			dwError = GetLastError(); 
			break;
		}

	} while (0);

	if (hService != NULL)
		CloseServiceHandle(hService);

	if (hSCManager != NULL)
		CloseServiceHandle(hSCManager);

	return bRet;
}

BOOL __stdcall OpenDevice(IN LPCSTR DriverName, OUT HANDLE *lphDevice)
{
	TCHAR	szDeviceName[64];
	HANDLE	hDevice = NULL;

	if ((GetVersion() & 0xFF) >= 5)
		wsprintf(szDeviceName, TEXT("\\\\.\\Global\\%s"), DriverName);
	else
		wsprintf(szDeviceName, TEXT("\\\\.\\%s"), DriverName);

	hDevice = CreateFile(szDeviceName,
						 GENERIC_READ | GENERIC_WRITE,
						 0,
						 NULL,
						 OPEN_EXISTING,
						 FILE_ATTRIBUTE_NORMAL,
						 NULL);

	if (hDevice == INVALID_HANDLE_VALUE)
		return FALSE;
	
	if (lphDevice != NULL)
		*lphDevice = hDevice;
	else
		CloseHandle(hDevice);

	return TRUE;
}

void CheckDevice(LPCSTR lpFileName)
{
	HANDLE	hDevice = INVALID_HANDLE_VALUE;
	DWORD	dwError = 0;

	hDevice = CreateFile(lpFileName,
						 GENERIC_READ | GENERIC_WRITE,
						 FILE_SHARE_READ,
						 NULL,
						 OPEN_ALWAYS,
						 FILE_ATTRIBUTE_NORMAL,
						 NULL);

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		dwError = GetLastError();
		return;
	}

	CloseHandle(hDevice);
}

DWORD GetDeviceHandle(HANDLE *hDevice, LPCSTR lpFileName)
{
	DWORD	dwError = 0;

	*hDevice = CreateFile(lpFileName,
						  GENERIC_READ|GENERIC_WRITE ,
						  FILE_SHARE_READ ,
						  NULL,
						  OPEN_ALWAYS ,
						  FILE_ATTRIBUTE_NORMAL,
						  NULL);

	if (hDevice == INVALID_HANDLE_VALUE)
		dwError = GetLastError();

	return dwError;
}

DWORD SendCommandToDriver(IN  LPCSTR	DriverName, 
						  IN  DWORD		IoctlCode, 
						  IN  LPVOID	InBuffer, 
						  IN  DWORD		InBufferSize,
						  OUT PVOID		OutBuffer,
						  IN  DWORD		OutBufferSize,
						  OUT LPDWORD	lpBytesReturned)
{
	BOOL	bResult = FALSE;
	HANDLE	hDevice = INVALID_HANDLE_VALUE;
	DWORD	dwReturn = 0;

	hDevice = CreateFile(DriverName ,
						 GENERIC_READ|GENERIC_WRITE ,
						 FILE_SHARE_READ ,
						 NULL,
						 OPEN_ALWAYS ,
						 FILE_ATTRIBUTE_NORMAL,
						 NULL);

	if ( hDevice == INVALID_HANDLE_VALUE )
	{
		return GetLastError();
	}
	bResult = DeviceIoControl(hDevice,
							  IoctlCode ,
							  InBuffer ,
							  InBufferSize,
							  OutBuffer,
							  OutBufferSize,
							  lpBytesReturned,
							  NULL);	

	if (!bResult)
		dwReturn = GetLastError();

	CloseHandle(hDevice);

	return dwReturn;
}

BOOL __stdcall KernelCopyFile(IN LPCSTR pszSourceFile,
							  IN LPCSTR pszDestFile)
{
	BOOL		bOk = FALSE;
	DWORD		dwReturn = 0;
	SEND_DATA	SendData = {0};
	ULONG		InputBufferSize = 0;
	DWORD		BytesReturned = 0;

	ZeroMemory(&SendData, sizeof(SEND_DATA));
	SendData.Size = sizeof(SEND_DATA);
	InputBufferSize = SendData.Size;

	SendData.SourceFileLen = strlen(pszSourceFile) + 1;
	CopyMemory(SendData.szSourceFile, pszSourceFile, SendData.SourceFileLen);

	SendData.DestFileLen = strlen(pszDestFile) + 1;
	CopyMemory(SendData.szDestFile, pszDestFile, SendData.DestFileLen);

	dwReturn = SendCommandToDriver(COPYDEVICE_NAME,
								   IOCTL_COPY_FILE,
								   &SendData,
								   sizeof(SendData),
								   NULL, 0, 
								   &BytesReturned);

	if (dwReturn == 0)
	{
		bOk = TRUE;
	} 
	else
	{
		bOk = FALSE;
	}

	return bOk;
}

//////////////////////////////////////////////////////////////////////////
// Return Value:
//		INVALID_HANDLE_VALUE:	fail
//////////////////////////////////////////////////////////////////////////
HANDLE __stdcall KernelOpenFile(IN LPCSTR FileName)
{
	HANDLE		FileHandle = NULL;
	ULONG		FileLen = 0;
	DWORD		dwReturn = 0;
	PVOID		InBuffer = (PVOID) FileName;
	BYTE	    OutBuffer[4] = {0};
	DWORD		OutBufferSize = sizeof(OutBuffer);
	DWORD		BytesReturned = 0;
	BOOL		bOk = FALSE;

	FileLen = strlen(FileName) + 1;

	dwReturn = SendCommandToDriver(COPYDEVICE_NAME,
								   IOCTL_OPEN_FILE,
								   InBuffer,
								   FileLen,
								   OutBuffer,
								   OutBufferSize,
								   &BytesReturned);

	if (dwReturn == 0)
	{
		bOk = TRUE;
		FileHandle = (HANDLE) (*(ULONG*) OutBuffer);
	} 
	else
	{
		bOk = FALSE;
		return INVALID_HANDLE_VALUE;
	}

	return FileHandle;
}

BOOL __stdcall KernelReadFile(IN  HANDLE	FileHandle,
							  OUT PVOID		Buffer,
							  IN  ULONG		Length,
							  IN  LONGLONG	ReadOffset,
							  OUT LPDWORD	lpBytesReturned)
{
	BOOL		bOk = FALSE;
	DWORD		dwReturn = 0;
	READ_BUFFER	InBuffer = {0};
	ULONG		InBufferSize = sizeof(InBuffer); // 16
	DWORD		BytesReturned = 0;

	InBuffer.ulFileHandle = (ULONG)FileHandle;
	InBuffer.ByteOffset.QuadPart = ReadOffset; 

	dwReturn = SendCommandToDriver(COPYDEVICE_NAME,
								   IOCTL_READ_FILE,
								   &InBuffer,
								   InBufferSize,
								   Buffer,
								   Length,
								   &BytesReturned);

	if (dwReturn == 0)
	{
		bOk = TRUE;
		if (lpBytesReturned)
			*lpBytesReturned = BytesReturned;
	} 
	else
	{
		bOk = FALSE;
	}

	return bOk;
}

BOOL __stdcall KernelCloseHandle(IN HANDLE FileHandle)
{
	BOOL		bOk = FALSE;
	DWORD		dwReturn = 0;
	ULONG 		InBuffer = 0;
	DWORD		InBufferSize = sizeof(InBuffer);
	DWORD		BytesReturned = 0;

	if (FileHandle == NULL || FileHandle == INVALID_HANDLE_VALUE)
		return FALSE;

	InBuffer = (ULONG) FileHandle;

	dwReturn = SendCommandToDriver(COPYDEVICE_NAME,
								   IOCTL_CLOSE_FILE,
								   &InBuffer,
								   InBufferSize,
								   NULL, 0, 
								   &BytesReturned);

	if (dwReturn == 0)
	{
		bOk = TRUE;
	} 
	else
	{
		bOk = FALSE;
	}

	return bOk;
}

BOOL __stdcall KernelGetFileSize(IN HANDLE FileHandle,
								 IN OUT LONGLONG *pFileSize)
{
	BOOL		bOk = FALSE;
	DWORD		dwReturn = 0;
	ULONG 		InBuffer = 0;
	DWORD		InBufferSize = sizeof(InBuffer);
	LONGLONG	OutBuffer = 0;
	DWORD		OutBufferSize = sizeof(OutBuffer);
	DWORD		BytesReturned = 0;

	if (FileHandle == NULL || FileHandle == INVALID_HANDLE_VALUE)
		return FALSE;

	InBuffer = (ULONG) FileHandle;

	dwReturn = SendCommandToDriver(COPYDEVICE_NAME,
								   IOCTL_GET_FILE_SIZE,
								   &InBuffer,
								   InBufferSize,
								   &OutBuffer,
								   OutBufferSize, 
								   &BytesReturned);

	if (dwReturn == 0 && pFileSize != NULL)
	{
		bOk = TRUE;
		*pFileSize = OutBuffer;
	} 
	else
	{
		bOk = FALSE;
	}

	return bOk;
}

