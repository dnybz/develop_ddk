

#include "logger_ddk.h"


//=========================================释放DLL用到的文件操作函数===========================================================
/**//**
	* 创建或打开文件
	* @param lpFileHandle 返回打开的文件句柄指针
	* @param usFileName 需要打开的文件路径，使用对象路径，如\\??\\c:\test.txt
	* @param dwDesiredAccess 申请权限，可以用|（或）组合以下操作
	*/
NTSTATUS MyCreateFile(OUT PHANDLE lpFileHandle,
					  IN PUNICODE_STRING usFileName,
					  IN ULONG dwDesiredAccess,
					  IN ULONG dwShareAccess,
					  IN ULONG dwCreateDisposition,
					  IN ULONG dwCreateOptions)
{
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	OBJECT_ATTRIBUTES oaName;
	IO_STATUS_BLOCK iosBlock;
	if (lpFileHandle != NULL && usFileName != NULL && usFileName->Buffer != NULL)
	{
		if (PASSIVE_LEVEL != KeGetCurrentIrql()) 
		{
			return ntStatus;
		}
		InitializeObjectAttributes(&oaName,
			usFileName,
			OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
			NULL,
			NULL);
		ntStatus = ZwCreateFile(lpFileHandle,
			dwDesiredAccess,
			&oaName,
			&iosBlock,
			NULL,
			FILE_ATTRIBUTE_NORMAL,
			dwShareAccess,
			dwCreateDisposition,
			dwCreateOptions,
			NULL,
			0);
		if (!NT_SUCCESS(ntStatus))
		{    
			return ntStatus;
		}
	}
	return ntStatus;
}

/**//**
	* 向文件写入内容
	* @param hFile 文件句柄
	* @param pBuffer 缓冲区
	* @param ulBufferSize 缓冲区大小
	* @param pulBytesWrite 实际写入的大小
	* @return 读取成功： STATUS_SUCCESS；读取失败：NTSTATUS error code
	*/
NTSTATUS MyWriteFile(IN HANDLE hFile,
					 IN PVOID pBuffer,
					 IN ULONG ulBufferSize,
					 OUT PULONG pulBytesWrite)
{
	IO_STATUS_BLOCK    iosBlock;
	NTSTATUS        ntStatus = STATUS_UNSUCCESSFUL;

	if (hFile == NULL || pBuffer == NULL)
	{
		return ntStatus;
	}

	if (PASSIVE_LEVEL !=  KeGetCurrentIrql()) 
	{
		return ntStatus;
	}

	*pulBytesWrite = 0;

	ntStatus = ZwWriteFile(hFile,
		NULL,
		NULL,
		NULL,
		&iosBlock,
		pBuffer,
		ulBufferSize,
		NULL,
		NULL);

	if (NT_SUCCESS(ntStatus))
	{
		*pulBytesWrite = (ULONG)iosBlock.Information;
	}

	return ntStatus;
}

/**//**
	* 关闭打开的文件句柄
	* @param hFile 文件句柄
	* @return 读取成功： STATUS_SUCCESS；读取失败：NTSTATUS error code
	*/
NTSTATUS MyCloseFile(IN HANDLE hFile)
{
	return ZwClose(hFile);
}


//#include <ntifs.h>
//#include <wdm.h>
//#include <ntstrsafe.h>
//写日志文件
NTSTATUS logger(char* buf,ULONG len)
{
	NTSTATUS  ntStatus = STATUS_UNSUCCESSFUL;
	UNICODE_STRING LogFilePath;
	ULONG pulBytesWrite = 0;
	HANDLE hFile = NULL;
	RtlInitUnicodeString(&LogFilePath,L"\\??\\C:\\1.log");
	ntStatus = MyCreateFile(&hFile,&LogFilePath,FILE_APPEND_DATA,FILE_SHARE_WRITE,FILE_OPEN_IF,FILE_NON_DIRECTORY_FILE|FILE_SYNCHRONOUS_IO_NONALERT);
	if (NT_SUCCESS(ntStatus))
	{
		ntStatus = MyWriteFile(hFile,buf,len,&pulBytesWrite);
		if(NT_SUCCESS(ntStatus))
		{
			return MyCloseFile(hFile);
		}
		MyCloseFile(hFile);
	}
	return ntStatus;
}

