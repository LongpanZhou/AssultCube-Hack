#include "constants.h"
#include "menu.h"
#include "esp.h"
#include <gl/GL.h>
#include <iostream>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "Include/imgui/imgui.h"
#include "Include/imgui/imgui_impl_win32.h"
#include "Include/imgui/imgui_impl_opengl2.h"

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
		ImGui::ShowDemoWindow(&showMenu);
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
	menu::endRender();

	wglMakeCurrent(hdc, gameContext);
	return end_originalSwapBuffers(hdc);
}