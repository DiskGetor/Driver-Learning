#include <ntifs.h>
#define DEV_DEVICE L"\\Device\\FirstDevice"
#define LINK_NAME L"\\??\\FirstDevice"


NTSTATUS CreateDevice(PDRIVER_OBJECT pDriverObject)
{
	NTSTATUS status;
	PDEVICE_OBJECT pDevObj;
	UNICODE_STRING usDevName;
	UNICODE_STRING usSymName;


	RtlInitUnicodeString(&usDevName, DEV_DEVICE);
	status = IoCreateDevice(pDriverObject, 0, &usDevName, FILE_DEVICE_UNKNOWN, 0, TRUE, &pDevObj);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	pDevObj->Flags |= DO_BUFFERED_IO;
	RtlInitUnicodeString(&usSymName, LINK_NAME);
	status = IoCreateSymbolicLink(&usSymName, &usDevName);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(pDevObj);
		return status;
	}
	return STATUS_SUCCESS;
}

VOID OnTimer(
	_In_ struct _DEVICE_OBJECT *DeviceObject,
	_In_opt_ PVOID Context
)
{
	static count = 3;
	if (count==0)
	{
		KdPrint(("OnTimer!"));
		count = 3;
	}
	count--;
}


VOID CreateIoTimer(PDEVICE_OBJECT pDeviceObject)
{
	NTSTATUS status;
	status = IoInitializeTimer(pDeviceObject, OnTimer, NULL);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("IoInitializeTimer Fail:%X！",status));
		return;
	}
	IoStartTimer(pDeviceObject);
}

VOID DeleteIoTimer(PDEVICE_OBJECT pDeviceObject)
{
	IoStopTimer(pDeviceObject);
}

NTSTATUS DriverUnLoad(PDRIVER_OBJECT pDriverObject)
{
	UNICODE_STRING usSymName;
	RtlInitUnicodeString(&usSymName, LINK_NAME);
	if (pDriverObject->DeviceObject != NULL)
	{
		DeleteIoTimer(pDriverObject->DeviceObject);
		IoDeleteSymbolicLink(&usSymName);
		IoDeleteDevice(pDriverObject->DeviceObject);
		KdPrint(("驱动卸载成功！"));
	}
	return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath)
{
	NTSTATUS  status;
	status = CreateDevice(pDriverObject);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("CreateDevice Error:%X",status));
	}
	else
	{
		KdPrint(("CreateDevice Success---DeviceObject:%X", pDriverObject->DeviceObject));
		CreateIoTimer(pDriverObject->DeviceObject);
	}
	pDriverObject->DriverUnload = DriverUnLoad;
	KdPrint(("驱动加载成功！"));
	return STATUS_SUCCESS;
}