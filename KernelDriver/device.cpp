#include "ntifs.h"
#include "device.h"

#include "debug.h"
#include "ioctl.h"

NTSTATUS OnCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	DbgMsg("[TST] On device create!\n");
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}
NTSTATUS OnClose(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	DbgMsg("[TST] On device close!\n");
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}
NTSTATUS OnDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	auto ctlCode = stack->Parameters.DeviceIoControl.IoControlCode;
	auto inBufLen = stack->Parameters.DeviceIoControl.InputBufferLength;
	DbgMsg("[TST] On device control: 0x%08x\n", ctlCode);
	NTSTATUS status = STATUS_SUCCESS;
	switch(ctlCode) {
	case IOCTL_SETTHREADPRIORITY: {
		if (inBufLen < sizeof(ThreadData)) {
			status = STATUS_INVALID_PARAMETER;
			DbgMsg("[TST] Supplied buffer is too small!\n");
			break;
		}

		if(stack->Parameters.DeviceIoControl.Type3InputBuffer == nullptr)
		{
			status = STATUS_INVALID_PARAMETER;
			DbgMsg("[TST] Supplied buffer is null!\n");
			break;
		}

		ThreadData* threadData = (ThreadData*)stack->Parameters.DeviceIoControl.Type3InputBuffer;
		PETHREAD threadObj;
		auto lookupStatus = PsLookupThreadByThreadId(ULongToHandle(threadData->threadId), &threadObj);
		if (!NT_SUCCESS(lookupStatus))
		{
			status = lookupStatus;
			DbgMsg("[TST] Lookup thread by ID failed! Status: 0x%08x\n", lookupStatus);
			break;
		}

		if (threadData->priority < 1 || threadData->priority > 31)
		{
			DbgMsg("[TST] Invalid priority! Must be between 1 and 31.\n");
			status = STATUS_INVALID_PARAMETER;
			break;
		}
		KeSetPriorityThread((PKTHREAD)threadObj, (long)threadData->priority);
		DbgMsg("[TST] Set thread priority of thread %d to %d!\n", threadData->threadId, threadData->priority);
		break;
	}
	default:
		status = STATUS_INVALID_DEVICE_REQUEST;
		break;

	}

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}