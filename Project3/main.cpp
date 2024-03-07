#include <dwmapi.h>
#include <d3d11.h>

#include "memory.h"
#include "options.h"
#include "offsets.h"
#include "features.h"
#include "visuals.h"
#include "gui.h"
#include "graphics.h"
#include "aimbot.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int wWidth{};
int wHeight{};
RECT windowPos{};

void setWindowSize() {
	HWND gameWindow = FindWindow(nullptr, L"AssaultCube");
	if (gameWindow != nullptr) {
		RECT targetWindow{};
		GetWindowRect(gameWindow, &targetWindow);
		wWidth = targetWindow.right - targetWindow.left;
		wHeight = targetWindow.bottom - targetWindow.top;

		GetWindowRect(gameWindow, &windowPos);
	}
}

void updateWindow(const HWND &window) {
	HWND gameWindow = FindWindow(nullptr, L"AssaultCube");
	RECT targetWindow{};
	GetWindowRect(gameWindow, &targetWindow);
	wWidth = targetWindow.right - targetWindow.left; // FIX 
	wHeight = targetWindow.bottom - targetWindow.top;
	MoveWindow(window, targetWindow.left, targetWindow.top, wWidth, wHeight, true);
}

void features(Memory memory)
{
	uintptr_t localPlayer{ memory.ReadMemory<uintptr_t>(memory.moduleBase + Offsets::localPlayer) };
	uintptr_t entityList{ memory.ReadMemory<uintptr_t>(memory.moduleBase + Offsets::entityList) };

	if (menu::noRecoil && !toggled::noRecoil)
	{
		noRecoil(memory, true);
		toggled::noRecoil = !toggled::noRecoil;
	}
	else if (!menu::noRecoil && toggled::noRecoil){
		noRecoil(memory, false);
		toggled::noRecoil = !toggled::noRecoil;
	}

	if (menu::infAmmo)
	{
		infAmmo(memory, true);
		toggled::ammoHack = true;
	}
	else if (!menu::infAmmo && toggled::ammoHack) {
		infAmmo(memory, false);
		toggled::ammoHack = false;
	}

	if (menu::fireRate && !toggled::noFireRate)
	{
		fireRate(memory, true);
		toggled::noFireRate = !toggled::noFireRate;
	}
	else if (!menu::fireRate && toggled::noFireRate){
		fireRate(memory, false);
		toggled::noFireRate = !toggled::noFireRate;
	}

	if (menu::noGunAnimation && !toggled::noGunAnimation)
	{
		noGunAnimation(memory, true);
		toggled::noGunAnimation = !toggled::noGunAnimation;
	}
	else if(!menu::noGunAnimation && toggled::noGunAnimation){
		noGunAnimation(memory, false);
		toggled::noGunAnimation = !toggled::noGunAnimation;
	}

	if (menu::noGunKnockback && !toggled::noGunKnockback)
	{
		noGunKnockback(memory, true);
		toggled::noGunKnockback = !toggled::noGunKnockback;
	}
	else if (!menu::noGunKnockback && toggled::noGunKnockback){
		noGunKnockback(memory, false);
		toggled::noGunKnockback = !toggled::noGunKnockback;
	}

	if (menu::playerEsp)
	{
		Visuals::Draw(memory, localPlayer, entityList);
	}

	if (menu::aimbot) {
		if (GetAsyncKeyState(options::aimbotKey)) {
			Aimbot(memory, localPlayer, entityList);
		}
	}

	if (menu::fov) {
		drawFovCircle();
	}
}

LRESULT CALLBACK WindowProc(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(window, uMsg, wParam, lParam)) {
		return 0L;
	}

	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0L;
	}
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0L;
	}
	return DefWindowProc(window, uMsg, wParam, lParam);
}

void openConsole() {
	AllocConsole();

	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmdShow) {

	openConsole();
	Memory memory;
	if (memory.initialize()) {
		std::cout << "Process Found!";
		FreeConsole();
	}
	else {
		std::cout << "Process Not Found";
		std::cin.get();
		return 0;
	}

	WNDCLASSEXW wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = L"External Overlay";

	RegisterClassExW(&wc);
	setWindowSize();

	const HWND window = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
		wc.lpszClassName,
		L"Overlay",
		WS_POPUP,
		0,
		0,
		wWidth,
		wHeight,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);
	{
		RECT clientArea{};
		GetClientRect(window, &clientArea);

		RECT windowArea{};
		GetWindowRect(window, &windowArea);

		POINT diff{};
		ClientToScreen(window, &diff);

		const MARGINS margins{
			windowArea.left + (diff.x - windowArea.left),
			windowArea.top + (diff.y - windowArea.top),
			clientArea.right,
			clientArea.bottom
		};

		DwmExtendFrameIntoClientArea(window, &margins);
	}

	Graphics render(window);
	ShowWindow(window, cmdShow);
	UpdateWindow(window);
	render.initImGui(window);

	bool running = true;
	while (running) {
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				running = false;
			}

		}

		if (!running) {
			break;
		}

		updateWindow(window);
		render.createNewFrame();

		if (menu::menu) {
			drawMenu();
		}
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			menu::menu = !menu::menu;

			long style = GetWindowLongPtr(window, GWL_EXSTYLE);
			style = menu::menu ? (style & ~WS_EX_LAYERED) : (style | WS_EX_LAYERED);
			SetWindowLongPtr(window, GWL_EXSTYLE, style);

			SetForegroundWindow(menu::menu ? window : FindWindow(nullptr, L"AssaultCube"));
		}
		features(memory);		
		render.RenderTarget();
		render.swapChain->Present(0U, 0U);
	}

	DestroyWindow(window);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);

	return 0;
}