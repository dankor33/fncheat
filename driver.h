#pragma once

#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <tlhelp32.h>

#include <dwmapi.h>
#include <fstream>
#include <string>
#include <cstdio>
#include <random>

#include "Driver/UTILS/skCrypt.h"
#include "Driver/UTILS/lazy_importer.hpp"
#include "Driver/Callstack-Spoofer.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <d2d1.h>
#include <d3d9types.h>
#include <d3d11.h>
#include "Lib/imgui/imgui.h"
#include "Lib/imgui/imgui_impl_dx11.h"
#include "Lib/imgui/imgui_impl_win32.h"
#include "Lib/imgui/imgui_internal.h"

#define CodeRead CTL_CODE(FILE_DEVICE_UNKNOWN, 0x54AF491, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CodeElevateProcess CTL_CODE(FILE_DEVICE_UNKNOWN, 0xC2F3745, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CodeModuleAddress CTL_CODE(FILE_DEVICE_UNKNOWN, 0x712615, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CodeWrite CTL_CODE(FILE_DEVICE_UNKNOWN, 0xAF1624, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CodeBaseAddress CTL_CODE(FILE_DEVICE_UNKNOWN, 0x61361, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CodeCR3 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x4F4125, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CodeMoveMouse CTL_CODE(FILE_DEVICE_UNKNOWN, 0x54DAF, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CodeHideProcess CTL_CODE(FILE_DEVICE_UNKNOWN, 0x416, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CodeRestartDriver CTL_CODE(FILE_DEVICE_UNKNOWN, 0x641, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct SReadWrite
{
	signed int ProcessID;
	uintptr_t Address;
	uintptr_t Buffer;
	uintptr_t Size;
} ReadWrite, * PReadWrite;

typedef struct SBaseAddress
{
	signed int ProcessID;
	uintptr_t* Address;
} BassAddress, * PBaseAddress;

typedef struct SModuleAddress
{
	signed int ProcessID;
	const char* ModuleName;
	uintptr_t* Address;
} ModdAddress, * PModuleAddress;

typedef struct SCR3
{
	signed int ProcessID;
} CR3, * PCR3;

typedef struct SZeroCR3
{
	bool ZeroCR3;
} ZeroCR3, * PZeroCR3;

typedef struct SElevateProcess
{
	signed int ProcessID;
} ElevateProcess, * PElevateProcess;

typedef struct SMoveMouse
{
	long X;
	long Y;
	unsigned short ButtonFlags;
} MoveMouse, * PMoveMouse;

typedef struct _hf {
	INT32 process_id;
} hidefile, * HideFileStruct;

class _Driver
{

public:
	static uintptr_t	BaseAddress;
	static HANDLE		DriverHandle;
	static int			ProcessID;
	static uintptr_t	ClientDLL;



	static bool			Init();
	static bool			Install(const char* certContent);
	static bool			Load(std::vector<std::uint8_t> Bytes);
	static int			FindProcessID(std::string ProcessName);
	static void			ReadProcessMemory(PVOID Destination, PVOID Buffer, DWORD Size);
	static void			ReadProcessArray(PVOID Destination, PVOID Buffer, DWORD Size);
	static void			WriteProcessMemory(PVOID Destination, PVOID Buffer, DWORD Size);
	static void			CacheCR3();
	static uintptr_t	FindBaseAddress();
	static uintptr_t	FindModuleAddress(const char* module_name);
	static void			ElevateProcess(int PID);
	static void			MoveMouse(int X, int Y);
	static void			HideProcess(int PiD);

	template <typename T>
	inline T RPM(uint64_t address)
	{
		SPOOF_FUNC;

		T buffer{ };
		ReadProcessMemory((PVOID)address, &buffer, sizeof(T));
		return buffer;
	}

	template <typename T>
	inline T WPM(uint64_t address, T buffer)
	{
		SPOOF_FUNC;

		WriteProcessMemory((PVOID)address, &buffer, sizeof(T));
		return buffer;
	}

	template <typename T>
	void readArray(uint64_t address, T* buffer, size_t size)
	{
		SPOOF_FUNC;

		ReadProcessMemory((PVOID)address, (PVOID)buffer, (sizeof(T) * size));
	}
};

inline _Driver* Driver;
