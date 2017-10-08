

#include "logger_ddk.h"


//=========================================�ͷ�DLL�õ����ļ���������===========================================================
/**//**
	* ��������ļ�
	* @param lpFileHandle ���ش򿪵��ļ����ָ��
	* @param usFileName ��Ҫ�򿪵��ļ�·����ʹ�ö���·������\\??\\c:\test.txt
	* @param dwDesiredAccess ����Ȩ�ޣ�������|����������²���
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
	* ���ļ�д������
	* @param hFile �ļ����
	* @param pBuffer ������
	* @param ulBufferSize ��������С
	* @param pulBytesWrite ʵ��д��Ĵ�С
	* @return ��ȡ�ɹ��� STATUS_SUCCESS����ȡʧ�ܣ�NTSTATUS error code
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
	* �رմ򿪵��ļ����
	* @param hFile �ļ����
	* @return ��ȡ�ɹ��� STATUS_SUCCESS����ȡʧ�ܣ�NTSTATUS error code
	*/
NTSTATUS MyCloseFile(IN HANDLE hFile)
{
	return ZwClose(hFile);
}


//#include <ntifs.h>
//#include <wdm.h>
//#include <ntstrsafe.h>
//д��־�ļ�
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

