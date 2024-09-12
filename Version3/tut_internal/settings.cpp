#include "settings.h"
#include "Include/imgui/imgui.h"

namespace Settings {
	namespace ESP
	{
		extern bool enabled = true;
		extern bool drawLines = true;
		extern bool drawTeam = false;
		extern bool showName = true;
		extern ImColor* teamColor = new ImColor(0, 0, 255, 255);
		extern ImColor* enemyColor = new ImColor(255, 0, 0, 255);
	}
	namespace Aimbot
	{
		extern bool enabled = true;
		extern bool smoothing = false;
		extern float smoothingAmout = 1.0f;
		extern bool checkInFov = true;
		extern float fov = 180;
		extern bool drawFovCircle = true;
	}
}