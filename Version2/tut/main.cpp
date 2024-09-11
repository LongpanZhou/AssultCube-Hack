#include "mem.h"
#include "Classes.h"
#include <algorithm>
#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

int main()
{
	auto mem = Mem::Mem(L"ac_client.exe");
	auto baseAddress = mem.GetModuleAddress(L"ac_client.exe");
	Entity localPlayer, tmp;
	vector<Entity> entities;
	methods m;
	
	while (true)
	{
		localPlayer = m.ReadEntity(mem.FindDynamicAddress(baseAddress, { Offsets::LocalPlayer }), mem);

		for (unsigned int i = 0; i < 31; i++)
		{
			tmp = m.ReadEntity(mem.FindDynamicAddress(baseAddress, { Offsets::EntityList, 0x4 * i }), mem);
			tmp.CalcMag(localPlayer);
		
			if (tmp.health > 0 && tmp.health <= 100)
				entities.push_back(tmp);
		}

		std::sort(entities.begin(), entities.end(), [](const Entity& a, const Entity& b) {return a.mag < b.mag;});
		std::this_thread::sleep_for(std::chrono::milliseconds(20));

		if (GetKeyState(VK_SHIFT) & 0x8000)
			if (entities.size() > 0)
				for (Entity e : entities)
					if (e.team != localPlayer.team)
					{
						Vector2 angle = localPlayer.CalcAngles(e);
						mem.Write(mem.FindDynamicAddress(baseAddress, { Offsets::LocalPlayer }) + 0x0034, angle);
						break;
					}

		entities.clear();
	}

	getchar();
	return 0;
}