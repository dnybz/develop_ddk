#include "[%Name%]WDM.h"

/************************************************************************
* ��������:DriverEntry
* ��������:��ʼ���������򣬶�λ������Ӳ����Դ�������ں˶���
* �����б�:
      pDriverObject:��I/O�������д���������������
      pRegistryPath:����������ע�����е�·��
* ���� ֵ:���س�ʼ������״̬
*************************************************************************/
#pragma INITCODE 
extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject,
								IN PUNICODE_STRING pRegistryPath)
{
	KdPrint(("Enter DriverEntry\n"));

	pDriverObject->DriverExtension->AddDevice = [%Name%]WDMAddDevice;
	pDriverObject->MajorFunction[IRP_MJ_PNP] = [%Name%]WDMPnp;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = 
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = 
	pDriverObject->MajorFunction[IRP_MJ_READ] = 
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = [%Name%]WDMDispatchRoutine;
	pDriverObject->DriverUnload = [%Name%]WDMUnload;

	KdPrint(("Leave DriverEntry\n"));
	return STATUS_SUCCESS;
}

#pragma PAGEDCODE
void Dump(IN PDRIVER_OBJECT	pDriverObject)
{
	KdPrint(("----------------------------------------------\n"));
	KdPrint(("Begin Dump...\n"));
	KdPrint(("Driver Address:0X%08X\n",pDriverObject));
	KdPrint(("Driver name:%wZ\n",&pDriverObject->DriverName));
	KdPrint(("Driver HardwareDatabase:%wZ\n",pDriverObject->HardwareDatabase));
	KdPrint(("Driver first device:0X%08X\n",pDriverObject->DeviceObject));
	
	PDEVICE_OBJECT pDevice = pDriverObject->DeviceObject;
	int i=1;
	for (;pDevice!=NULL;pDevice = pDevice->NextDevice)
	{
		KdPrint(("The %d device\n",i++));
		KdPrint(("Device AttachedDevice:0X%08X\n",pDevice->AttachedDevice));
		KdPrint(("Device NextDevice:0X%08X\n",pDevice->NextDevice));
		KdPrint(("Device StackSize:%d\n",pDevice->StackSize));
		KdPrint(("Device's DriverObject:0X%08X\n",pDevice->DriverObject));
	}
	
	KdPrint(("Dump over!\n"));
	KdPrint(("----------------------------------------------\n"));
}

#pragma PAGEDCODE
void DumpDeviceStack(IN PDEVICE_OBJECT	pdo)
{
	KdPrint(("----------------------------------------------\n"));

	KdPrint(("Begin Dump device stack...\n"));
	
	PDEVICE_OBJECT pDevice = pdo;
	int i=0;
	for (;pDevice!=NULL;pDevice=pDevice->AttachedDevice)
	{
		KdPrint(("The %d device in device stack\n",i++));
		KdPrint(("Device AttachedDevice:0X%08X\n",pDevice->AttachedDevice));
		KdPrint(("Device NextDevice:0X%08X\n",pDevice->NextDevice));
		KdPrint(("Device StackSize:%d\n",pDevice->StackSize));
		KdPrint(("Device's DriverObject:0X%08X\n",pDevice->DriverObject));
	}

	KdPrint(("Dump over!\n"));
	KdPrint(("----------------------------------------------\n"));
}

/************************************************************************
* ��������:[%Name%]WDMAddDevice
* ��������:������豸
* �����б�:
      DriverObject:��I/O�������д���������������
      PhysicalDeviceObject:��I/O�������д������������豸����
* ���� ֵ:����������豸״̬
*************************************************************************/
#pragma PAGEDCODE
NTSTATUS [%Name%]WDMAddDevice(IN PDRIVER_OBJECT DriverObject,
                           IN PDEVICE_OBJECT PhysicalDeviceObject)
{ 
	PAGED_CODE();
	KdPrint(("Enter [%Name%]WDMAddDevice\n"));

	NTSTATUS status;
	PDEVICE_OBJECT fdo;
	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName,L"\\Device\\[%Name%]WDMDevice");
	status = IoCreateDevice(
		DriverObject,
		sizeof(DEVICE_EXTENSION),
		&(UNICODE_STRING)devName,
		FILE_DEVICE_UNKNOWN,
		0,
		FALSE,
		&fdo);
	if( !NT_SUCCESS(status))
		return status;
	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION)fdo->DeviceExtension;
	pdx->fdo = fdo;
	pdx->NextStackDevice = IoAttachDeviceToDeviceStack(fdo, PhysicalDeviceObject);
	UNICODE_STRING symLinkName;
	RtlInitUnicodeString(&symLinkName,L"\\DosDevices\\[%Name%]WDM");

	pdx->ustrDeviceName = devName;
	pdx->ustrSymLinkName = symLinkName;
	status = IoCreateSymbolicLink(&(UNICODE_STRING)symLinkName,&(UNICODE_STRING)devName);

	if( !NT_SUCCESS(status))
	{
		IoDeleteSymbolicLink(&pdx->ustrSymLinkName);
		status = IoCreateSymbolicLink(&symLinkName,&devName);
		if( !NT_SUCCESS(status))
		{
			return status;
		}
	}

	fdo->Flags |= DO_BUFFERED_IO | DO_POWER_PAGABLE;
	fdo->Flags &= ~DO_DEVICE_INITIALIZING;


	Dump(DriverObject);
	DumpDeviceStack(PhysicalDeviceObject);

	KdPrint(("Leave [%Name%]WDMAddDevice\n"));
	return STATUS_SUCCESS;
}

/************************************************************************
* ��������:DefaultPnpHandler
* ��������:��PNP IRP����ȱʡ����
* �����б�:
      pdx:�豸�������չ
      Irp:��IO�����
* ���� ֵ:����״̬
*************************************************************************/ 
#pragma PAGEDCODE
NTSTATUS DefaultPnpHandler(PDEVICE_EXTENSION pdx, PIRP Irp)
{
	PAGED_CODE();
	KdPrint(("Enter DefaultPnpHandler\n"));
	IoSkipCurrentIrpStackLocation(Irp);
	KdPrint(("Leave DefaultPnpHandler\n"));
	return IoCallDriver(pdx->NextStackDevice, Irp);
}

/************************************************************************
* ��������:HandleRemoveDevice
* ��������:��IRP_MN_REMOVE_DEVICE IRP���д���
* �����б�:
      fdo:�����豸����
      Irp:��IO�����
* ���� ֵ:����״̬
*************************************************************************/
#pragma PAGEDCODE
NTSTATUS HandleRemoveDevice(PDEVICE_EXTENSION pdx, PIRP Irp)
{
	PAGED_CODE();
	KdPrint(("Enter HandleRemoveDevice\n"));

	Irp->IoStatus.Status = STATUS_SUCCESS;
	NTSTATUS status = DefaultPnpHandler(pdx, Irp);
	IoDeleteSymbolicLink(&(UNICODE_STRING)pdx->ustrSymLinkName);

    //����IoDetachDevice()��fdo���豸ջ���ѿ���
    if (pdx->NextStackDevice)
        IoDetachDevice(pdx->NextStackDevice);
	
    //ɾ��fdo��
    IoDeleteDevice(pdx->fdo);
	KdPrint(("Leave HandleRemoveDevice\n"));
	return status;
}

/************************************************************************
* ��������:[%Name%]WDMPnp
* ��������:�Լ��弴��IRP���д���
* �����б�:
      fdo:�����豸����
      Irp:��IO�����
* ���� ֵ:����״̬
*************************************************************************/
#pragma PAGEDCODE
NTSTATUS [%Name%]WDMPnp(IN PDEVICE_OBJECT fdo,
                        IN PIRP Irp)
{
	PAGED_CODE();

	KdPrint(("Enter [%Name%]WDMPnp\n"));
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	static NTSTATUS (*fcntab[])(PDEVICE_EXTENSION pdx, PIRP Irp) = 
	{
		DefaultPnpHandler,		// IRP_MN_START_DEVICE
		DefaultPnpHandler,		// IRP_MN_QUERY_REMOVE_DEVICE
		HandleRemoveDevice,		// IRP_MN_REMOVE_DEVICE
		DefaultPnpHandler,		// IRP_MN_CANCEL_REMOVE_DEVICE
		DefaultPnpHandler,		// IRP_MN_STOP_DEVICE
		DefaultPnpHandler,		// IRP_MN_QUERY_STOP_DEVICE
		DefaultPnpHandler,		// IRP_MN_CANCEL_STOP_DEVICE
		DefaultPnpHandler,		// IRP_MN_QUERY_DEVICE_RELATIONS
		DefaultPnpHandler,		// IRP_MN_QUERY_INTERFACE
		DefaultPnpHandler,		// IRP_MN_QUERY_CAPABILITIES
		DefaultPnpHandler,		// IRP_MN_QUERY_RESOURCES
		DefaultPnpHandler,		// IRP_MN_QUERY_RESOURCE_REQUIREMENTS
		DefaultPnpHandler,		// IRP_MN_QUERY_DEVICE_TEXT
		DefaultPnpHandler,		// IRP_MN_FILTER_RESOURCE_REQUIREMENTS
		DefaultPnpHandler,		// 
		DefaultPnpHandler,		// IRP_MN_READ_CONFIG
		DefaultPnpHandler,		// IRP_MN_WRITE_CONFIG
		DefaultPnpHandler,		// IRP_MN_EJECT
		DefaultPnpHandler,		// IRP_MN_SET_LOCK
		DefaultPnpHandler,		// IRP_MN_QUERY_ID
		DefaultPnpHandler,		// IRP_MN_QUERY_PNP_DEVICE_STATE
		DefaultPnpHandler,		// IRP_MN_QUERY_BUS_INFORMATION
		DefaultPnpHandler,		// IRP_MN_DEVICE_USAGE_NOTIFICATION
		DefaultPnpHandler,		// IRP_MN_SURPRISE_REMOVAL
	};

	ULONG fcn = stack->MinorFunction;
	if (fcn >= arraysize(fcntab))
	{						// unknown function
		status = DefaultPnpHandler(pdx, Irp); // some function we don't know about
		return status;
	}						// unknown function

#if DBG
	static char* fcnname[] = 
	{
		"IRP_MN_START_DEVICE",
		"IRP_MN_QUERY_REMOVE_DEVICE",
		"IRP_MN_REMOVE_DEVICE",
		"IRP_MN_CANCEL_REMOVE_DEVICE",
		"IRP_MN_STOP_DEVICE",
		"IRP_MN_QUERY_STOP_DEVICE",
		"IRP_MN_CANCEL_STOP_DEVICE",
		"IRP_MN_QUERY_DEVICE_RELATIONS",
		"IRP_MN_QUERY_INTERFACE",
		"IRP_MN_QUERY_CAPABILITIES",
		"IRP_MN_QUERY_RESOURCES",
		"IRP_MN_QUERY_RESOURCE_REQUIREMENTS",
		"IRP_MN_QUERY_DEVICE_TEXT",
		"IRP_MN_FILTER_RESOURCE_REQUIREMENTS",
		"",
		"IRP_MN_READ_CONFIG",
		"IRP_MN_WRITE_CONFIG",
		"IRP_MN_EJECT",
		"IRP_MN_SET_LOCK",
		"IRP_MN_QUERY_ID",
		"IRP_MN_QUERY_PNP_DEVICE_STATE",
		"IRP_MN_QUERY_BUS_INFORMATION",
		"IRP_MN_DEVICE_USAGE_NOTIFICATION",
		"IRP_MN_SURPRISE_REMOVAL",
	};

	KdPrint(("PNP Request (%s)\n", fcnname[fcn]));
#endif // DBG

	status = (*fcntab[fcn])(pdx, Irp);
	KdPrint(("Leave [%Name%]WDMPnp\n"));
	return status;
}

/************************************************************************
* ��������:[%Name%]WDMDispatchRoutine
* ��������:��ȱʡIRP���д���
* �����б�:
      fdo:�����豸����
      Irp:��IO�����
* ���� ֵ:����״̬
*************************************************************************/
#pragma PAGEDCODE
NTSTATUS [%Name%]WDMDispatchRoutine(IN PDEVICE_OBJECT fdo,
								 IN PIRP Irp)
{
	PAGED_CODE();
	KdPrint(("Enter [%Name%]WDMDispatchRoutine\n"));
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;	// no bytes xfered
	IoCompleteRequest( Irp, IO_NO_INCREMENT );
	KdPrint(("Leave [%Name%]WDMDispatchRoutine\n"));
	return STATUS_SUCCESS;
}

/************************************************************************
* ��������:[%Name%]WDMUnload
* ��������:�������������ж�ز���
* �����б�:
      DriverObject:��������
* ���� ֵ:����״̬
*************************************************************************/
#pragma PAGEDCODE
void [%Name%]WDMUnload(IN PDRIVER_OBJECT DriverObject)
{
	PAGED_CODE();
	KdPrint(("Enter [%Name%]WDMUnload\n"));
	KdPrint(("Leave [%Name%]WDMUnload\n"));
}
