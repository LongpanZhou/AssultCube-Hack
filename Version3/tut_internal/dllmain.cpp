#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "esp.h"
#include "menu.h"
#include "Include/minHook/MinHook.h"

void hook()
{
	if (MH_Initialize() == MH_OK)
		std::cout << "MH Initialized" << std::endl;
	if (MH_CreateHook(originalSwapBuffers, &menu::newSwapBuffers, reinterpret_cast<void**>(&end_originalSwapBuffers)) == MH_OK)
		std::cout << "MH Hooked" << std::endl;
	if (MH_EnableHook(originalSwapBuffers) == MH_OK)
		std::cout << "MH Enable" << std::endl;
}

void unhook()
{
	if (MH_DisableHook(originalSwapBuffers) == MH_OK)
		std::cout << "MH Disable" << std::endl;
	if (MH_RemoveHook(originalSwapBuffers) == MH_OK)
		std::cout << "MH Unhooked" << std::endl;
	if (MH_Uninitialize() == MH_OK)
		std::cout << "MH Uninitialize" << std::endl;
}

void injected_thread(HMODULE h)
{
	AllocConsole();
	FILE* file;
	freopen_s(&file, "CONOUT$", "w", stdout);
	freopen_s(&file, "CONIN$", "r", stdin);
	std::cout << "Injected successfully" << std::endl;
	menu::init();
	hook();

	while (true) {
		entityList = *(EntityList**)(exeBaseAddress + 0x191FCC);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		esp::aimbot();
		if (GetAsyncKeyState(VK_DELETE))
		{
			std::cout << "Please Close Window" << std::endl;
			break;
		}
	}

	unhook();
	menu::shutdown();
	fclose(file);
	FreeConsole();
	FreeLibraryAndExitThread(h, 0);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)injected_thread, hinstDLL, NULL, NULL);
	case DLL_THREAD_ATTACH:

	case DLL_THREAD_DETACH:

	case DLL_PROCESS_DETACH:
		break;
	}
	return true;
}