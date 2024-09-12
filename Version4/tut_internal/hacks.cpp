#include "hacks.h"
#include "constants.h"

void* MapAddrTarget = (void*)(exeBaseAddress + 0x5D2C4);
void* MapAddrPatchTo = (void*)(exeBaseAddress + 0x5D3E6);

void* RadarAddrTarget = (void*)(exeBaseAddress + 0x5C4D6);
void* RadarAddrPatchTo = (void*)(exeBaseAddress + 0x5C606);

byte originalMapBytes[6] = { 0 };
byte originalRadarBytes[6] = { 0 };

void hacks::Map()
{
	DWORD oldProtect;
	VirtualProtect(MapAddrTarget, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(originalMapBytes, MapAddrTarget, 6);
	*((BYTE*)MapAddrTarget) = 0xE9;
	*((DWORD*)((BYTE*)MapAddrTarget + 1)) = (DWORD)((BYTE*)MapAddrPatchTo - (BYTE*)MapAddrTarget) - 5;
	*((BYTE*)MapAddrTarget + 5) = 0x90;

	VirtualProtect(MapAddrTarget, 6, oldProtect, &oldProtect);
}

void hacks::unMap()
{
	DWORD oldProtect;
	VirtualProtect(MapAddrTarget, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(MapAddrTarget, originalMapBytes, 6);
	VirtualProtect(MapAddrTarget, 6, oldProtect, &oldProtect);
}

void hacks::Radar()
{
	DWORD oldProtect;
	VirtualProtect(RadarAddrTarget, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(originalRadarBytes, RadarAddrTarget, 6);
	*((BYTE*)RadarAddrTarget) = 0xE9;
	*((DWORD*)((BYTE*)RadarAddrTarget + 1)) = (DWORD)((BYTE*)RadarAddrPatchTo - (BYTE*)RadarAddrTarget) - 5;
	*((BYTE*)RadarAddrTarget + 5) = 0x90;

	VirtualProtect(RadarAddrTarget, 6, oldProtect, &oldProtect);
}

void hacks::unRadar()
{
	DWORD oldProtect;
	VirtualProtect(RadarAddrTarget, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(RadarAddrTarget, originalRadarBytes, 6);
	VirtualProtect(RadarAddrTarget, 6, oldProtect, &oldProtect);
}