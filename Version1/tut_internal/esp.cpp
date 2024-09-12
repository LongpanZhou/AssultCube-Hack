#include "constants.h"
#include "esp.h"
const float FOV = 90;

bool isInFov(Player* owner, Vec3 looking)
{
	Vec3 angle = CalcAngle(owner->FeetPosition, looking);
	Vec3 playerAngel{ owner->ViewAngle.x + 180, owner->ViewAngle.y, 0 };
	playerAngel = playerAngel.Normalize();

	Vec3 angleDiff = playerAngel - angle;
	angleDiff = angleDiff.Normalize();

	return (fabs(angleDiff.x) <= FOV / 2 && fabs(angleDiff.y <= FOV / 2));
}

Player* esp::getNearestPlayer()
{
	Player* nearestPlayer = nullptr;
	float nearestDistance = 99999.0f;
	entityList = *(EntityList**)(exeBaseAddress + 0x191FCC);

	for (int i = 0; i < *numPlayers-1; i++)
	{
		Player* player = entityList->players[i];
		if (player->team!=localPlayerPtr->team || player->health > 100 || player->health <= 0 || isInFov(localPlayerPtr, player->HeadPosition))
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

void esp::aimbot()
{
	if (!GetAsyncKeyState(VK_SHIFT))
		return;
	Player* target = getNearestPlayer();
	if (!target)
		return;
	Vec3 angle = CalcAngle(localPlayerPtr->FeetPosition, target->FeetPosition);

	localPlayerPtr->ViewAngle.x = angle.x + 180.f;
	localPlayerPtr->ViewAngle.y = angle.y;
}