#include "esp.h"
#include "Include/imgui/imgui.h"
#include <iostream>
#include "settings.h"

#define deg2rad(x) (x * 3.14159265358979323846 / 180)
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

bool isInFovWS2(vec3& ScreenLoc)
{
	return (abs(Vec3{ width / 2.0f,height / 2.0f, 0 }.Distance(ScreenLoc) < Settings::Aimbot::fov));
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

void esp::aimbot()
{
	if (!Settings::Aimbot::enabled || !GetAsyncKeyState(VK_SHIFT))
		return;

	Player* target = esp::getNearestEntityAngle();

	if (!target)
		return;

	Vec3 AimPosition;
	WorldToScreen(target->HeadPosition, AimPosition, viewMatrix, width, height);

	if (Settings::Aimbot::checkInFov && !isInFovWS2(AimPosition))
		return;

	Vec3 angle = CalcAngle(localPlayerPtr->HeadPosition, target->HeadPosition);
	localPlayerPtr->ViewAngle.x = angle.x + 180.f;
	localPlayerPtr->ViewAngle.y = angle.y;
}

void esp::esp()
{
	if (!Settings::ESP::enabled)
		return;

	for (int i = 0; i < *numPlayers - 1; i++)
	{
		Player* player = entityList->players[i];
		if (player->team == localPlayerPtr->team && !Settings::ESP::drawTeam)
			continue;

		if (player->health > 100 || player->health <= 0 || player == localPlayerPtr)
			continue;

		bool Draw = true;
		Vec3 wtsFeet, wtsHead;
		Draw |= WorldToScreen(player->FeetPosition, wtsFeet, viewMatrix, width, height);
		Draw |= WorldToScreen(player->HeadPosition, wtsHead, viewMatrix, width, height);
		if (!Draw)
			continue;

		ImColor ESPColor = player->team == localPlayerPtr->team ? *Settings::ESP::teamColor : *Settings::ESP::enemyColor;

		if (Settings::ESP::showName)
		{
			std::string text = player->name;
			float textWidth = ImGui::CalcTextSize(text.c_str()).x;
			ImVec2 position(wtsFeet.x - textWidth / 2, wtsFeet.y);
			ImGui::GetBackgroundDrawList()->AddText(position, ImColor(255, 255, 255, 255), text.c_str());
		}

		if (Settings::Aimbot::drawFovCircle)
			ImGui::GetBackgroundDrawList()->AddCircle(ImVec2{ width / 2.0f, height / 2.0f }, Settings::Aimbot::fov, ImColor(255, 255, 255, 128), 0.5);

		if (Settings::ESP::drawLines)
			ImGui::GetForegroundDrawList()->AddLine(ImVec2(width / 2, height), { wtsFeet.x, wtsFeet.y }, ESPColor, 1.0f);

		Vec4 ESPrect = CalcRect(wtsFeet, wtsHead);
		ImGui::GetForegroundDrawList()->AddRect({ ESPrect.x,ESPrect.y }, { ESPrect.w,ESPrect.z }, ESPColor);

		ImVec2 start{ ESPrect.x + 5, wtsFeet.y };
		ImVec2 end{ ESPrect.x + 5, wtsFeet.y + (wtsHead.y - wtsFeet.y) * (player->health / 100.f) };
		ImColor color{ (int)(255 * (1 - player->health / 100.f)), (int)(255 * player->health / 100.f), 0 };
		ImGui::GetForegroundDrawList()->AddLine(start, end, color, 3.0f);
	}
}

void esp::NoClip()
{
	if (!Settings::Hacks::NoClip)
		return;

	float forwardX = (float)(Settings::Hacks::Speed)*cos(deg2rad((localPlayerPtr->ViewAngle.x - 180)));
	float forwardY = (float)(Settings::Hacks::Speed)*sin(deg2rad((localPlayerPtr->ViewAngle.x - 180)));
	float forwardZ = (float)(Settings::Hacks::Speed)*sin(deg2rad((localPlayerPtr->ViewAngle.y)));

	if (GetAsyncKeyState('A'))
	{
		localPlayerPtr->FeetPosition.x += forwardX;
		localPlayerPtr->FeetPosition.y += forwardY;
	}
	else if (GetAsyncKeyState('D'))
	{
		localPlayerPtr->FeetPosition.x -= forwardX;
		localPlayerPtr->FeetPosition.y -= forwardY;
	}
	else if (GetAsyncKeyState('S'))
	{
		localPlayerPtr->FeetPosition.x += forwardY;
		localPlayerPtr->FeetPosition.y -= forwardX;
		localPlayerPtr->FeetPosition.z -= forwardZ;
	}
	else if (GetAsyncKeyState('W'))
	{
		localPlayerPtr->FeetPosition.x -= forwardY;
		localPlayerPtr->FeetPosition.y += forwardX;
		localPlayerPtr->FeetPosition.z += forwardZ;
	}
	else if (GetAsyncKeyState(VK_SPACE))
	{
		localPlayerPtr->FeetPosition.z += Settings::Hacks::Speed;
	}
	else if (GetAsyncKeyState(VK_LSHIFT))
	{
		localPlayerPtr->FeetPosition.z -= Settings::Hacks::Speed;
	}
}