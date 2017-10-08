//========================================================================== 
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <NTDDK.h>
#ifdef __cplusplus
}
#endif 

 int test_ZwLoadDriver() 
{ 
//PUNICODE_STRING RegistryPath=R;
	ZwLoadDriver(NULL);
	return 1;
}

#pragma INITCODE
extern "C" NTSTATUS DriverEntry ( IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath	) 
{
	NTSTATUS status=0;
	KdPrint(("Enter DriverEntry\n"));

	//注册其他驱动调用函数入口
	//riverObject->DriverUnload = [%Name%]DDKUnload;
	//riverObject->MajorFunction[IRP_MJ_CREATE] = [%Name%]DDKDispatchRoutine;
	//riverObject->MajorFunction[IRP_MJ_CLOSE] = [%Name%]DDKDispatchRoutine;
	//riverObject->MajorFunction[IRP_MJ_WRITE] = [%Name%]DDKDispatchRoutine;
	//riverObject->MajorFunction[IRP_MJ_READ] = [%Name%]DDKDispatchRoutine;
	
	//创建驱动设备对象
	//status = CreateDevice(pDriverObject);
 test_ZwLoadDriver() ;
	KdPrint(("DriverEntry end\n"));
	return status;
}