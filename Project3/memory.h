#pragma once

#include <iostream>
#include <string>
#include <Windows.h>
#include "TlHelp32.h"
#include <tchar.h>
#include <vector>

class Memory
{
public:
	uintptr_t moduleBase{};
private:
	uintptr_t processId{};
	HANDLE hProcess{};
	std::wstring procName{ L"ac_client.exe" };
public:
	BOOL initialize();

	//Reads Process Memory
	template<typename T>
	T ReadMemory(uintptr_t address)
	{
		T value{};
		ReadProcessMemory(hProcess, (LPCVOID)address, &value, sizeof(value), NULL);
		return value;
	}

	//Writes Process Memory
	template<typename T>
	void WriteMemory(uintptr_t address, T value)
	{
		WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(value), NULL);
	}

	uintptr_t ReadMemoryChain(uintptr_t address, std::vector<int> offsets);

private:

	//Takes a snapshot of the currenty processlist and loops through it till it finds the passed in process
	DWORD getProcId();
	DWORD GetModuleBase();
};