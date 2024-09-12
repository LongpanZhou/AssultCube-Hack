#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "esp.h"

void injected_thread(HMODULE h)
{
	AllocConsole();
	FILE* file;
	freopen_s(&file, "CONOUT$", "w", stdout);
	freopen_s(&file, "CONIN$", "r", stdin);
	std::cout << "Injected successfully" << std::hex << std::endl;

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		esp::aimbot();
		//for (int key = 0x08; key <= 0xFE; ++key) {
		//	if (GetAsyncKeyState(key) & 0x8000) {
		//		switch (key) {
		//		case 'W':
		//			localPlayerPtr->Velocity.x += 0.5;
		//			break;
		//		case 'S':
		//			localPlayerPtr->Velocity.x -= 0.5;
		//			break;
		//		case 'A':
		//			localPlayerPtr->Velocity.z -= 0.5;
		//			break;
		//		case 'D':
		//			localPlayerPtr->Velocity.z += 0.5;
		//			break;
		//		case VK_SPACE:
		//			localPlayerPtr->Velocity.y += 0.5;
		//			break;
		//		case VK_LCONTROL:
		//			localPlayerPtr->Velocity.y -= 0.5;
		//			break;
		//		}
		//	}
		//}
	}

	fclose(file);
	FreeConsole();
	FreeLibraryAndExitThread(h, 0);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)injected_thread, nullptr, 0, nullptr);
	case DLL_THREAD_ATTACH:

	case DLL_THREAD_DETACH:

	case DLL_PROCESS_DETACH:
		break;
	}
	return true;
}