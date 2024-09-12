#pragma once

class ImColor;

namespace Settings{
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
		extern float smoothingAmout;
		extern bool checkInFov;
		extern float fov;
		extern bool drawFovCircle;
	}
}