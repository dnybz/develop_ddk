/************************************************************************
* 文件名称:Driver.cpp                                                 
* 作    者:RichTech
* 完成日期:2007-11-1
*************************************************************************/

#include "Driver.h"

/************************************************************************
* 函数名称:DriverEntry
* 功能描述:初始化驱动程序，定位和申请硬件资源，创建内核对象
* 参数列表:
      pDriverObject:从I/O管理器中传进来的驱动对象
      pRegistryPath:驱动程序在注册表的中的路径
* 返回 值:返回初始化驱动状态
*************************************************************************/
#pragma INITCODE
extern "C" NTSTATUS DriverEntry (
			IN PDRIVER_OBJECT pDriverObject,
			IN PUNICODE_STRING pRegistryPath	) 
{
	NTSTATUS status;
	KdPrint(("Enter DriverEntry\n"));

	//设置卸载函数
	pDriverObject->DriverUnload = HelloDDKUnload;

	//设置派遣函数
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = HelloDDKDispatchRoutin;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = HelloDDKDispatchRoutin;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = HelloDDKDispatchRoutin;
	pDriverObject->MajorFunction[IRP_MJ_READ] = HelloDDKDispatchRoutin;
	pDriverObject->MajorFunction[IRP_MJ_CLEANUP] = HelloDDKDispatchRoutin;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = HelloDDKDispatchRoutin;
	pDriverObject->MajorFunction[IRP_MJ_SET_INFORMATION] = HelloDDKDispatchRoutin;
	pDriverObject->MajorFunction[IRP_MJ_SHUTDOWN] = HelloDDKDispatchRoutin;
	pDriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = HelloDDKDispatchRoutin;
	
	//创建驱动设备对象
	status = CreateDevice(pDriverObject);

	KdPrint(("Leave DriverEntry\n"));
	return status;
}

/************************************************************************
* 函数名称:CreateDevice
* 功能描述:初始化设备对象
* 参数列表:
      pDriverObject:从I/O管理器中传进来的驱动对象
* 返回 值:返回初始化状态
*************************************************************************/
#pragma INITCODE
NTSTATUS CreateDevice (
		IN PDRIVER_OBJECT	pDriverObject) 
{
	NTSTATUS status;
	PDEVICE_OBJECT pDevObj;
	PDEVICE_EXTENSION pDevExt;
	
	//创建设备名称
	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName,L"\\Device\\[%Name%]");
	
	//创建设备
	status = IoCreateDevice( pDriverObject,
						sizeof(DEVICE_EXTENSION),
						&(UNICODE_STRING)devName,
						FILE_DEVICE_UNKNOWN,
						0, TRUE,
						&pDevObj );
	if (!NT_SUCCESS(status))
		return status;

	pDevObj->Flags |= DO_BUFFERED_IO;
	pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	pDevExt->pDevice = pDevObj;
	pDevExt->ustrDeviceName = devName;
	//创建符号链接
	UNICODE_STRING symLinkName;
	RtlInitUnicodeString(&symLinkName,L"\\??\\[%Name%]");
	pDevExt->ustrSymLinkName = symLinkName;
	status = IoCreateSymbolicLink( &symLinkName,&devName );
	if (!NT_SUCCESS(status)) 
	{
		IoDeleteDevice( pDevObj );
		return status;
	}
	return STATUS_SUCCESS;
}

/************************************************************************
* 函数名称:HelloDDKUnload
* 功能描述:负责驱动程序的卸载操作
* 参数列表:
      pDriverObject:驱动对象
* 返回 值:返回状态
*************************************************************************/
#pragma PAGEDCODE
VOID HelloDDKUnload (IN PDRIVER_OBJECT pDriverObject) 
{
	PDEVICE_OBJECT	pNextObj;
	KdPrint(("Enter DriverUnload\n"));
	pNextObj = pDriverObject->DeviceObject;
	while (pNextObj != NULL) 
	{
		PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)
			pNextObj->DeviceExtension;

		//删除符号链接
		UNICODE_STRING pLinkName = pDevExt->ustrSymLinkName;
		IoDeleteSymbolicLink(&pLinkName);
		pNextObj = pNextObj->NextDevice;
		IoDeleteDevice( pDevExt->pDevice );
	}
}

/************************************************************************
* 函数名称:HelloDDKDispatchRoutin
* 功能描述:对读IRP进行处理
* 参数列表:
      pDevObj:功能设备对象
      pIrp:从IO请求包
* 返回 值:返回状态
*************************************************************************/
#pragma PAGEDCODE
NTSTATUS HelloDDKDispatchRoutin(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp) 
{
	KdPrint(("Enter HelloDDKDispatchRoutin\n"));

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	//建立一个字符串数组与IRP类型对应起来
	static char* irpname[] = 
	{
		"IRP_MJ_CREATE",
		"IRP_MJ_CREATE_NAMED_PIPE",
		"IRP_MJ_CLOSE",
		"IRP_MJ_READ",
		"IRP_MJ_WRITE",
		"IRP_MJ_QUERY_INFORMATION",
		"IRP_MJ_SET_INFORMATION",
		"IRP_MJ_QUERY_EA",
		"IRP_MJ_SET_EA",
		"IRP_MJ_FLUSH_BUFFERS",
		"IRP_MJ_QUERY_VOLUME_INFORMATION",
		"IRP_MJ_SET_VOLUME_INFORMATION",
		"IRP_MJ_DIRECTORY_CONTROL",
		"IRP_MJ_FILE_SYSTEM_CONTROL",
		"IRP_MJ_DEVICE_CONTROL",
		"IRP_MJ_INTERNAL_DEVICE_CONTROL",
		"IRP_MJ_SHUTDOWN",
		"IRP_MJ_LOCK_CONTROL",
		"IRP_MJ_CLEANUP",
		"IRP_MJ_CREATE_MAILSLOT",
		"IRP_MJ_QUERY_SECURITY",
		"IRP_MJ_SET_SECURITY",
		"IRP_MJ_POWER",
		"IRP_MJ_SYSTEM_CONTROL",
		"IRP_MJ_DEVICE_CHANGE",
		"IRP_MJ_QUERY_QUOTA",
		"IRP_MJ_SET_QUOTA",
		"IRP_MJ_PNP",
	};

	UCHAR type = stack->MajorFunction;
	if (type >= arraysize(irpname))
		KdPrint((" - Unknown IRP, major type %X\n", type));
	else
		KdPrint(("\t%s\n", irpname[type]));


	//对一般IRP的简单操作，后面会介绍对IRP更复杂的操作
	NTSTATUS status = STATUS_SUCCESS;
	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );

	KdPrint(("Leave HelloDDKDispatchRoutin\n"));

	return status;
}

