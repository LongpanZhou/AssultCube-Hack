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
	int health; //0x00EC
	char pad_00F0[277]; //0x00F1
	char name[16]; //0x0205
	char pad_0215[271]; //0x0215
	bool team; //0x0324
}; //Size: 0x0325

class EntityList
{
public:
	Player* players[32];
};