#pragma once
#include <Windows.h>
#include "structures.h"

extern HMODULE hModule;
extern uintptr_t exeBaseAddress;
extern Player* localPlayerPtr;
extern EntityList* entityList;
extern int* numPlayers;