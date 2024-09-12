#pragma once
#include "geom.h"

class Player
{
public:
	void* UnkownPtr; //0x0000
	Vector3 HeadPosition; //0x0004
	Vector3 Velocity; //0x0010
	Vector3 ZeroArray; //0x001C
	Vector3 FeetPosition; //0x0028
	Vector3 ViewAngle; //0x0034
	float Recoil; //0x0040
	char pad_0044[168]; //0x0044
	int32_t health; //0x00EC
	char pad_00F0[276]; //0x00F0
	bool placeholder; //0x0204
	char name[16]; //0x0205
	char pad_0215[271]; //0x0215
	bool team; //0x0324
	char pad_0328[59]; //0x0328
	class Weapon* PrevWeapon; //0x0360
	class Weapon* CurrWeapon; //0x0364
}; //Size: 0x0368

class EntityList
{
public:
	Player* players[32];
};

class Weapon
{
public:
	void* vtable; //0x0000
	int32_t primaryWeapon; //0x0004
	class Player* owner; //0x0008
	class WeaponStats* WeaponStats; //0x000C
	int32_t* ammo; //0x0010
	int32_t* mag; //0x0014
	int32_t* waittime; //0x0018
	int32_t shootCnt; //0x001C
	int32_t reloading; //0x0020
	char pad_0024[16]; //0x0024
};

class WeaponStats
{
public:
	char Name[23]; //0x0000
	char Description[43]; //0x0017
	uint16_t ShootingSound; //0x0042
	uint16_t ReloadingSound; //0x0044
	uint16_t ReloadingTime; //0x0046
	uint16_t AttackDelay; //0x0048
	uint16_t Damage; //0x004A
	uint16_t piercing; //0x004C
	uint16_t N00000211; //0x004E
	uint16_t N00000216; //0x0050
	uint16_t Spread; //0x0052
	uint16_t PushBack; //0x0054
	uint16_t MagSize; //0x0056
	uint16_t ShakeX; //0x0058
	uint16_t ShakeY; //0x005A
	uint16_t RecoilStep; //0x005C
	uint16_t BaseRecoil; //0x005E
	uint16_t MaxRecoil; //0x0060
	uint16_t RecoilPushBack; //0x0062
	uint16_t KonckBack; //0x0064
}; //Size: 0x0080