#pragma once
#include <Windows.h>
#include "structures.h"

extern HMODULE hModule;
extern uintptr_t exeBaseAddress;
extern Player* localPlayerPtr;
extern EntityList* entityList;
extern int* numPlayers;
extern float* viewMatrix;

typedef BOOL(__stdcall* _wglSwapBuffers)(HDC hdc);
extern _wglSwapBuffers originalSwapBuffers;
extern _wglSwapBuffers end_originalSwapBuffers;