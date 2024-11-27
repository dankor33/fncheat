#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
uintptr_t virtualaddy;
uintptr_t cr3;

#define CODE_READ CTL_CODE(FILE_DEVICE_UNKNOWN, 0x134576, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_BASE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x548767, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_GET_GUARDED_REGION CTL_CODE(FILE_DEVICE_UNKNOWN, 0x65877, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_GET_DIR_BASE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x464876, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_SECURITY 0x9ae52c5


typedef struct _ReadWrite1 {
	INT32 security;
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
	BOOLEAN write;
} rw, * prw;

typedef struct _Base1 {
	INT32 security;
	INT32 process_id;
	ULONGLONG* address;
} ba, * pba;

typedef struct _Guarded_Region1 {
	INT32 security;
	ULONGLONG* address;
} ga, * pga;

typedef struct _MEMORY_OPERATION_DATA {
	uint32_t        pid;
	ULONGLONG* cr3;
} MEMORY_OPERATION_DATA, * PMEMORY_OPERATION_DATA;

namespace mem {
	HANDLE driver_handle;
	INT32 process_id;

	bool find_driver() {
		driver_handle = CreateFileW((L"\\\\.\\NvidiaCore"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		if (!driver_handle || (driver_handle == INVALID_HANDLE_VALUE))
			return false;

		return true;
	}

	void read_physical(PVOID address, PVOID buffer, DWORD size) {
		_ReadWrite1 arguments = { 0 };

		arguments.security = CODE_SECURITY;
		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = process_id;
		arguments.write = FALSE;

		DeviceIoControl(driver_handle, CODE_READ, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}

	void write_physical(PVOID address, PVOID buffer, DWORD size) {
		_ReadWrite1 arguments = { 0 };

		arguments.security = CODE_SECURITY;
		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = process_id;
		arguments.write = TRUE;

		DeviceIoControl(driver_handle, CODE_READ, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}

	uintptr_t fetch_cr3() {
		uintptr_t cr3 = NULL;
		_MEMORY_OPERATION_DATA arguments = { 0 };

		arguments.pid = process_id;
		arguments.cr3 = (ULONGLONG*)&cr3;

		DeviceIoControl(driver_handle, CODE_GET_DIR_BASE, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return cr3;
	}

	uintptr_t find_image() {
		uintptr_t image_address = { NULL };
		_Base1 arguments = { NULL };

		arguments.security = CODE_SECURITY;
		arguments.process_id = process_id;
		arguments.address = (ULONGLONG*)&image_address;

		DeviceIoControl(driver_handle, CODE_BASE, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return image_address;
	}

	uintptr_t get_guarded_region() {
		uintptr_t guarded_region_address = { NULL };
		_Guarded_Region1 arguments = { NULL };

		arguments.security = CODE_SECURITY;
		arguments.address = (ULONGLONG*)&guarded_region_address;

		DeviceIoControl(driver_handle, CODE_GET_GUARDED_REGION, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return guarded_region_address;
	}

	INT32 find_process(LPCTSTR process_name) {
		PROCESSENTRY32 pt;
		HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		pt.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hsnap, &pt)) {
			do {
				if (!lstrcmpi(pt.szExeFile, process_name)) {
					CloseHandle(hsnap);
					process_id = pt.th32ProcessID;
					return pt.th32ProcessID;
				}
			} while (Process32Next(hsnap, &pt));
		}
		CloseHandle(hsnap);

		return { NULL };
	}
}

template <typename T>
T read(uint64_t address) {
	T buffer{ };
	mem::read_physical((PVOID)address, &buffer, sizeof(T));
	return buffer;
}

template <typename T>
T write(uint64_t address, T buffer) {

	mem::write_physical((PVOID)address, &buffer, sizeof(T));
	return buffer;
}
