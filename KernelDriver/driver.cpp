#include "driver.h"
#include "globals.h"
#include "debug.h"
#include "device.h"

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	UNREFERENCED_PARAMETER(pRegistryPath);
	DbgMsg("[TST] Driver loaded! PDO: %p\n", pDriverObject);

	pDriverObject->MajorFunction[IRP_MJ_CREATE] = OnCreate;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = OnClose;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = OnDeviceControl;

	
	IoCreateDevice(pDriverObject, 0, &deviceName, FILE_DEVICE_UNKNOWN, 0, false, &deviceObj);
	DbgMsg("[TST] Device created! PDO: %p\n", deviceObj);
	IoCreateSymbolicLink(&symLinkName, &deviceName);

	pDriverObject->DriverUnload = DriverUnload;

	return STATUS_SUCCESS;
}

void DriverUnload(PDRIVER_OBJECT DriverObject) {
	UNREFERENCED_PARAMETER(DriverObject);
	IoDeleteDevice(deviceObj);
	IoDeleteSymbolicLink(&symLinkName);
	DbgMsg("[TST] Driver unloaded!\n");
}