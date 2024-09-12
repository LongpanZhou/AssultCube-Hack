#pragma once

namespace menu {
	void toggleMenu();
	void init();
	void startRender();
	void render();
	void endRender();
	void shutdown();
	BOOL __stdcall newSwapBuffers(HDC hdc);
}