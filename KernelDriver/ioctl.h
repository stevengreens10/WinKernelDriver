#pragma once

#define TEST_DEVICE 0x8000

#define IOCTL_SETTHREADPRIORITY CTL_CODE(TEST_DEVICE, 0x800, METHOD_NEITHER, FILE_ANY_ACCESS)

typedef struct ThreadData
{
	unsigned long threadId;
	unsigned short priority;
} ThreadData;