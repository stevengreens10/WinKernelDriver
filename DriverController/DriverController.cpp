#include <iostream>
#include <Windows.h>

#include "KernelDriver/ioctl.h"

int main(int argc, char *argv[]) {

	HANDLE hDevice = CreateFile(L"\\\\.\\TEST", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hDevice == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Could not open device" << std::endl;
	}

	if(argc != 3)
	{
		printf("Usage: %s <threadID> <priority>", argv[0]);
		return 1;
	}

	auto threadId = (unsigned long) strtol(argv[1] ,NULL, 10);
	auto priority = (unsigned short) atoi(argv[2]);

	ThreadData data{ threadId, priority };
	DWORD bytesReturned;
	bool status = DeviceIoControl(hDevice, IOCTL_SETTHREADPRIORITY, &data, sizeof(ThreadData), nullptr, 0, &bytesReturned, nullptr);
	std::cout << "Status: " << status << std::endl;
}
