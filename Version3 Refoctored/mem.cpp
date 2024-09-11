#include "mem.h"

Mem::Mem(std::wstring_view processName)
{
	const auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapShot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);

		while (Process32Next(snapShot, &entry))
			if (std::wstring_view(entry.szExeFile).contains(processName))
			{
				this->processId = entry.th32ProcessID;
				this->process = OpenProcess(PROCESS_ALL_ACCESS, NULL, processId);
				break;
			}
	}
	CloseHandle(snapShot);
}

Mem::~Mem()
{
	if (this->process)
		CloseHandle(this->process);
}

DWORD Mem::GetProcessId()
{
	return this->processId;
}

HANDLE Mem::GetProcessHandle()
{
	return this->process;
}

uintptr_t Mem::GetModuleAddress(std::wstring_view processName)
{
	uintptr_t modBaseAddr;
	const auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->processId);

	if (snapShot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		while (Module32Next(snapShot, &modEntry))
			if (std::wstring_view(modEntry.szModule).contains(processName))
			{
				modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
				break;
			}
	}
	CloseHandle(snapShot);
	return modBaseAddr;
}

bool Mem::InForeground()
{
	HWND tmp_hwnd = GetForegroundWindow();
	if (tmp_hwnd == nullptr) return false;

	DWORD foregroundPID;
	if (GetWindowThreadProcessId(tmp_hwnd, &foregroundPID) == 0) return false;
	return (foregroundPID == this->processId);
}

uintptr_t Mem::FindDynamicAddress(uintptr_t ptr, vector<unsigned int> offsets)
{
	auto addr = static_cast<uint32_t>(ptr); //32 bits
	//cout << "----------------------------------------" << endl;
	//cout << hex << addr << dec << " " << sizeof(addr) << endl;

	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		//cout << hex << addr << " + " << offsets[i] << " = " << " " << addr + offsets[i] << dec << " " << sizeof(addr) << endl;
		addr += offsets[i];
		ReadProcessMemory(this->process, (BYTE*)addr, &addr, sizeof(addr), nullptr);
	}
	//cout << hex << addr << endl;
	return addr;
}