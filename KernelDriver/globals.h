#pragma once
#include <ntddk.h>

UNICODE_STRING deviceName = RTL_CONSTANT_STRING(L"\\Device\\TEST");
UNICODE_STRING symLinkName = RTL_CONSTANT_STRING(L"\\??\\TEST");

PDEVICE_OBJECT deviceObj;