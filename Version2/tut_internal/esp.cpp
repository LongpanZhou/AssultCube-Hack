#include "esp.h"
#include "Include/imgui/imgui.h"
#include <iostream>

const float FOV = 90;
extern int width = 1920, height = 1080;

Player* esp::getNearestPlayer()
{
	Player* nearestPlayer = nullptr;
	float nearestDistance = 99999.0f;

	for (int i = 0; i < *numPlayers - 1; i++)
	{
		Player* player = entityList->players[i];
		if (player->team == localPlayerPtr->team || player->health > 100 || player->health <= 0)
			continue;
		float distance = localPlayerPtr->FeetPosition.Distance(player->FeetPosition);
		if (distance < nearestDistance)
		{
			nearestDistance = distance;
			nearestPlayer = player;
		}
	}
	return nearestPlayer;
}

Player* esp::getNearestEntityAngle()
{
	Player* nearestPlayer = nullptr;
	float smallestAngle = 99999.0f;

	for (int i = 0; i < *numPlayers - 1; i++)
	{
		Player* player = entityList->players[i];
		if (player->team == localPlayerPtr->team || player->health > 100 || player->health <= 0)
			continue;
		Vec3 targetAngle = CalcAngle(localPlayerPtr->HeadPosition, player->HeadPosition);
		//normalize 
		float xDiff = localPlayerPtr->ViewAngle.x - targetAngle.x;
		if (xDiff <= 0)
			xDiff += 360;
		xDiff = min(360 - xDiff, xDiff - 180);

		float yDiff = min(abs(localPlayerPtr->ViewAngle.y - targetAngle.y), abs(targetAngle.y - localPlayerPtr->ViewAngle.y));

		float angleMagnitude = sqrt(pow(xDiff, 2) + pow(yDiff, 2));
		if (angleMagnitude < smallestAngle)
		{
			smallestAngle = angleMagnitude;
			nearestPlayer = player;
		}
	}
	return nearestPlayer;
}

Player* esp::getNearestEntityAngleWts()
{
	Player* nearestPlayer = nullptr;
	float nearestDistance = 99999.0f;

	for (int i = 0; i < *numPlayers - 1; i++)
	{
		Player* player = entityList->players[i];
		if (player->team == localPlayerPtr->team || player->health > 100 || player->health <= 0)
			continue;

		Vec3 AimPosition;
		WorldToScreen(player->HeadPosition, AimPosition, viewMatrix, width, height);
		float distance = abs(AimPosition.Distance(Vec3{ width / 2.0f,height / 2.0f,0 }));

		if (AimPosition.z < 0)
			continue;

		if (distance < nearestDistance)
		{
			nearestDistance = distance;
			nearestPlayer = player;
		}
	}
	return nearestPlayer;
}