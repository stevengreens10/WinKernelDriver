#pragma once
#include <ntddk.h>

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);

void DriverUnload(PDRIVER_OBJECT DriverObject);