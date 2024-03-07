#include "memory.h"

BOOL Memory::initialize()
{
	if (processId = getProcId()) {
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
		moduleBase = GetModuleBase();
		return true;
	}
	else {
		return false;
	}
}

uintptr_t Memory::ReadMemoryChain(uintptr_t address, std::vector<int> offsets) {
	uintptr_t newAddress{ address };

	for (int i{ 0 }; i < (offsets.size()); ++i)
	{
		ReadProcessMemory(hProcess, (LPCVOID)(newAddress), &newAddress, sizeof(newAddress), NULL);
		newAddress += offsets[i];
	}

	return newAddress;
}

DWORD Memory::getProcId() {

	DWORD pidHighestThreadCount = 0;
	DWORD highestThreadCount = 0;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnap, &pe);

	while (Process32Next(hSnap, &pe))
	{
		if ((_tcscmp(pe.szExeFile, procName.c_str()) == 0))
		{
			highestThreadCount = pe.cntThreads;
			pidHighestThreadCount = pe.th32ProcessID;
		}
	}

	CloseHandle(hSnap);

	return pidHighestThreadCount;
}

DWORD Memory::GetModuleBase() {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);
	MODULEENTRY32 moduleEntry32;
	moduleEntry32.dwSize = sizeof(MODULEENTRY32);
	Module32First(hSnap, &moduleEntry32);
	do {
		if (_wcsicmp(moduleEntry32.szModule, procName.c_str()) == 0) {
			moduleBase = (DWORD)moduleEntry32.modBaseAddr;
			break;
		}
	} while (Module32Next(hSnap, &moduleEntry32));

	CloseHandle(hSnap);

	return moduleBase;
}
