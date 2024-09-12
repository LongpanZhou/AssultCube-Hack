#pragma once

#include <vector>
#include <string>
class ImColor;

namespace Settings{
	std::vector<std::string> getSaveFiles();
	std::string stripProfileName(std::string &profileName);
	void saveSettings(const std::string &filename);
	void loadSettings(const std::string& filename);

	namespace ESP
	{
		extern bool enabled;
		extern bool drawLines;
		extern bool drawTeam;
		extern bool showName;
		extern ImColor* teamColor;
		extern ImColor* enemyColor;
	}
	namespace Aimbot
	{
		extern bool enabled;
		extern bool smoothing;
		extern float smoothingAmount;
		extern bool checkInFov;
		extern float fov;
		extern bool drawFovCircle;
	}
	namespace Hacks
	{
		extern bool RadarHack;
		extern bool MapHack;
	}
}