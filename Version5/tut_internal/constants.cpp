#include "constants.h"

HMODULE hModule = GetModuleHandle(nullptr);
uintptr_t exeBaseAddress = (uintptr_t)GetModuleHandle(nullptr);

extern Player* localPlayerPtr = *(Player**)(exeBaseAddress + 0x17E0A8);
extern EntityList* entityList = *(EntityList**)(exeBaseAddress + 0x191FCC);
extern float* viewMatrix = (float*)(exeBaseAddress + 0x17DFD0);

extern int* numPlayers = (int*)(exeBaseAddress + 0x18AC0C);
_wglSwapBuffers originalSwapBuffers = (_wglSwapBuffers)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
_wglSwapBuffers end_originalSwapBuffers = originalSwapBuffers;