#include "mem.h"
#include "Classes.h"
#include <iostream>
#include <algorithm>
#include <windows.h>
#include <chrono>
#include <thread>
#include <GLFW/glfw3.h>

int main()
{
	RECT rect;
	HWND game = FindWindowA(nullptr, "AssaultCube");
	auto mem = Mem::Mem(L"ac_client.exe");
	auto baseAddress = mem.GetModuleAddress(L"ac_client.exe");
	int width, height;

	GetWindowRect(game, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	glfwInit();
	glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(width, height, "AssaultCube ESP", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	
	Entity localPlayer, tmp;
	vector<Entity> entities;
	methods m;

	while (true)
	{
		GetWindowRect(game, &rect);
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;

		glfwSetWindowPos(window, rect.left, rect.top);
		glfwSetWindowSize(window, width, height);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		localPlayer = m.ReadEntity(mem.FindDynamicAddress(baseAddress, { Offsets::LocalPlayer }), mem);

		for (unsigned int i = 0; i < 32; i++)
		{
			tmp = m.ReadEntity(mem.FindDynamicAddress(baseAddress, { Offsets::EntityList, 0x4 * i }), mem);
			tmp.CalcMag(localPlayer);
		
			if (tmp.health > 0 && tmp.health <= 100)
				entities.push_back(tmp);
		}

		std::sort(entities.begin(), entities.end(), [](const Entity& a, const Entity& b) {return a.mag < b.mag;});
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		ViewMatrix vm = mem.Read<ViewMatrix>(baseAddress + Offsets::ViewMatrix);

		for (const Entity &e : entities)
		{
			ImVec2 wtsFeet = m.WorldToScreen(vm, e.feet, width, height);
			ImVec2 wtsHead = m.WorldToScreen(vm, e.head, width, height);
			ImVec4 ESPrect = m.CalcRect(wtsFeet, wtsHead);

			if (wtsFeet.x > 0 && localPlayer.team != e.team)
				{
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(width / 2, height), wtsFeet, ImColor{255,255,255}, 1.0f);
					ImGui::GetForegroundDrawList()->AddRect({ ESPrect.x,ESPrect.y}, { ESPrect.w,ESPrect.z}, ImColor{ 255,255,255 });
					ImVec2 start{ ESPrect.x + 5, wtsFeet.y };
					ImVec2 end{ ESPrect.x + 5, wtsFeet.y + (wtsHead.y - wtsFeet.y) * (e.health / 100.f) };
					ImColor color{ (int)(255 * (1 - e.health / 100.f)), (int)(255 * e.health / 100.f), 0 };
					ImGui::GetForegroundDrawList()->AddLine(start, end, color, 3.0f);
				}
		}

		if (GetKeyState(VK_SHIFT) & 0x8000 && !entities.empty())
			for (const Entity &e : entities)
				if (e.team != localPlayer.team)
				{
					Vector2 angle = localPlayer.CalcAngles(e);
					mem.Write(mem.FindDynamicAddress(baseAddress, { Offsets::LocalPlayer }) + 0x0034, angle);
					break;
				}
		entities.clear();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}