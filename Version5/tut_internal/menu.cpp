#include "constants.h"
#include "menu.h"
#include "esp.h"
#include <gl/GL.h>
#include <iostream>
#include "settings.h"
#include "hacks.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "Include/imgui/imgui.h"
#include "Include/imgui/imgui_impl_win32.h"
#include "Include/imgui/imgui_impl_opengl2.h"
#include <float.h>

bool showMenu, initialized, contextCreated, insert_pressed;

HWND gameWindow;
HGLRC myContext;
HGLRC gameContext;
HDC gHDC;
WNDPROC originalWndProc;
RECT rect;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK newWNDProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (showMenu)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;
		ImGuiIO& io = ImGui::GetIO();

		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_MOUSEWHEEL:
		case WM_MOUSEMOVE:
			io.MousePos.x = (signed short)(lParam);
			io.MousePos.y = (signed short)(lParam >> 16);
			return 0;
		}
	}
	GetWindowRect(gameWindow, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	return CallWindowProc(originalWndProc, hWnd, uMsg, wParam, lParam);
}

void espSettings()
{
	if (!ImGui::BeginTabItem("ESP"))
		return;
	ImGui::Checkbox("Enabled", &Settings::ESP::enabled);
	ImGui::Checkbox("Draw Team", &Settings::ESP::drawTeam);
	ImGui::Checkbox("draw Lines", &Settings::ESP::drawLines);
	ImGui::Checkbox("Show Name", &Settings::ESP::showName);
	ImGui::ColorEdit4("Team Color", (float*)&Settings::ESP::teamColor->Value);
	ImGui::ColorEdit4("Enemy Color", (float*)&Settings::ESP::enemyColor->Value);
	ImGui::EndTabItem();
}

void aimbotSettings()
{
	if (!ImGui::BeginTabItem("Aimbot"))
		return;
	ImGui::Checkbox("Enabled", &Settings::Aimbot::enabled);
	ImGui::Checkbox("FOV circle", &Settings::Aimbot::drawFovCircle);
	ImGui::Checkbox("Check FOV", &Settings::Aimbot::checkInFov);
	ImGui::InputFloat("Aimbot FOV", (float*)&Settings::Aimbot::fov);
	ImGui::Checkbox("Smooth Aim", &Settings::Aimbot::smoothing);
	ImGui::InputFloat("Team Color", (float*)&Settings::Aimbot::smoothingAmount);
	ImGui::EndTabItem();
}

void HacksSettings()
{
	if (!ImGui::BeginTabItem("Hacks"))
		return;
	if (ImGui::Checkbox("Radar Enabled", &Settings::Hacks::RadarHack))
	{
		if (Settings::Hacks::RadarHack)
		{
			hacks::Radar();
		}
		else
		{
			hacks::unRadar();
		}
			
	}
	if (ImGui::Checkbox("Map Enabled", &Settings::Hacks::MapHack))
	{
		if (Settings::Hacks::MapHack)
		{
			hacks::Map();
		}
		else
		{
			hacks::unMap();
		}
	}
	if (ImGui::Checkbox("NoClip Enabled", &Settings::Hacks::NoClip))
	{
		if (Settings::Hacks::NoClip)
		{
			hacks::noClip();
		}
		else
		{
			hacks::unnoClip();
		}
	}
	ImGui::SliderFloat("Speed", &Settings::Hacks::Speed, 0.0f, 10.0f);
	ImGui::EndTabItem();
}

void profileSettings()
{
	if (!ImGui::BeginTabItem("Profile"))
		return;
	static char newProfileName[128] = "";
	static std::string selectedProfile = "";
	static std::vector<std::string> profiles = Settings::getSaveFiles();

	ImGui::Text("Load Existing Profile");
	if (ImGui::BeginListBox("##profiles", ImVec2(-FLT_MIN, 4 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (auto& profile : profiles)
		{
			std::string profileName = Settings::stripProfileName(profile);
			bool isSelected = (selectedProfile == profile);
			if (ImGui::Selectable(profileName.c_str(), isSelected))
				selectedProfile = profile;
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	if (ImGui::Button("Load Profile") && !selectedProfile.empty())
		Settings::loadSettings(selectedProfile);

	if (ImGui::Button("Delete Profile") && !selectedProfile.empty())
	{
		if (std::remove(selectedProfile.c_str()) == 0)
		{
			profiles = Settings::getSaveFiles();
			selectedProfile.clear();
		}
	}

	ImGui::Text("Save Profile");
	ImGui::InputText("##profileName", newProfileName, IM_ARRAYSIZE(newProfileName));
	if (ImGui::Button("Save"))
	{
		Settings::saveSettings((std::string(newProfileName) + ".json"));
		profiles = Settings::getSaveFiles();
	}

	ImGui::TextColored(ImVec4(1, 0, 0, 1), "Warning: (might) need admin permission!");
	ImGui::EndTabItem();
}

void gunStatSeetings()
{
	if (!ImGui::BeginTabItem("GunStats"))
		return;

	if (!localPlayerPtr || !localPlayerPtr->CurrWeapon || !localPlayerPtr->CurrWeapon->WeaponStats)
	{
		ImGui::Text("No weapon found");
	}
	else
	{
		WeaponStats* info = localPlayerPtr->CurrWeapon->WeaponStats;
		ImGui::Text("Name: %s", info->Name);
		ImGui::Text("Description: %s", info->Description);

		const char* labels[] = { "Recoil", "RecoilPushBack", "Damage", "AttackDelay", "PushBack", "Spread", "MagSize", "KonckBack", "ReloadingTime"};
		uint16_t* uint16Values[] = { &info->BaseRecoil, &info->RecoilPushBack, &info->Damage, &info->AttackDelay, &info->PushBack, &info->Spread, &info->MagSize, &info->KonckBack, &info->ReloadingTime };
		uint16_t* outputs[] = { (uint16_t*)&info->BaseRecoil, (uint16_t*)&info->RecoilPushBack, (uint16_t*)&info->Damage, (uint16_t*)&info->AttackDelay, (uint16_t*)&info->PushBack, (uint16_t*)&info->Spread, (uint16_t*)&info->MagSize, (uint16_t*)&info->KonckBack, (uint16_t*)&info->ReloadingTime };

		for (int i = 0; i < 9; ++i) {
			int tmp = static_cast<int>(*uint16Values[i]);
			if (ImGui::SliderInt(labels[i], &tmp, 0, 1000)) {
				*outputs[i] = (uint16_t)(tmp);
			}
		}

	}	
	ImGui::EndTabItem();
}

void menu::toggleMenu()
{
	showMenu = !showMenu;
	ImGuiIO& io = ImGui::GetIO();
	io.WantCaptureMouse = showMenu;
	io.WantCaptureKeyboard = showMenu;
	io.MouseDrawCursor = showMenu;
}

void menu::init()
{
	gameWindow = FindWindowA(NULL, "AssaultCube");
	originalWndProc = (WNDPROC)SetWindowLongPtr(gameWindow, GWL_WNDPROC, (LONG_PTR)newWNDProc);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

	ImGui_ImplWin32_Init(gameWindow);
	ImGui_ImplOpenGL2_Init();
	ImGui::StyleColorsDark();

	ImGui::SetCurrentContext(ImGui::GetCurrentContext());
	initialized = true;
}

void menu::startRender()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void menu::render()
{
	if (GetAsyncKeyState(VK_INSERT) & 0x8000)
	{
		if (!insert_pressed)
		{
			toggleMenu();
			insert_pressed = true;
		}
	}
	else
	{
		insert_pressed = false;
	}
	if (showMenu)
	{
		ImGui::SetNextWindowSize(ImVec2(400, 400));
		ImGui::Begin("Menu", &showMenu);
		if (ImGui::BeginTabBar("Tab bar"))
		{
			espSettings();
			aimbotSettings();
			HacksSettings();
			profileSettings();
			gunStatSeetings();
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}

void menu::endRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void SetupContext(HDC hdc)
{
	myContext = wglCreateContext(hdc);

	wglMakeCurrent(hdc, myContext);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	contextCreated = true;
}

void menu::shutdown()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	SetWindowLongPtr(FindWindowA(NULL, "AssaultCube"), GWLP_WNDPROC, (LONG_PTR)originalWndProc);
}

BOOL __stdcall menu::newSwapBuffers(HDC hdc)
{
	gameContext = wglGetCurrentContext();
	if (!contextCreated)
		SetupContext(hdc);

	wglMakeCurrent(hdc, myContext);
	menu::startRender();
	menu::render();
	esp::esp();
	esp::aimbot();
	esp::NoClip();
	menu::endRender();

	wglMakeCurrent(hdc, gameContext);
	return end_originalSwapBuffers(hdc);
}