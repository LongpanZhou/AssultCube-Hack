#pragma once

#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;

class Mem
{
private:
	DWORD processId;
	HANDLE process;
public:
	Mem(const wchar_t* processName);
	~Mem();

	DWORD GetProcessId();

	HANDLE GetProcessHandle();

	uintptr_t GetModuleAddress(const wchar_t* modName);

	uintptr_t FindDynamicAddress(uintptr_t ptr, vector<unsigned int> offsets);

	bool InForeground();

	template<typename T>
	T Read(uintptr_t address)
	{
		T value;
		ReadProcessMemory(this->process, (LPCVOID)address, &value, sizeof(T), nullptr);
		return value;
	}

	template<typename T>
	bool Write(uintptr_t address, T value)
	{
		return WriteProcessMemory(this->process, (LPVOID)address, &value, sizeof(T), nullptr);
	}
};