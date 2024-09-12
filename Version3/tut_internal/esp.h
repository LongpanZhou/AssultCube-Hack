#pragma once
#include "structures.h"
#include "constants.h"
 
extern int width, height;
namespace esp{
	Player* getNearestPlayer();
	Player* getNearestEntityAngle();
	Player* getNearestEntityAngleWts();
	void aimbot();
	void esp();
}