#include "gui.h"
#include "options.h"
#include "hotkey.h"

#include <chrono>

bool demoOpen{ true };
static int activeTab{};

void drawMenu() {
	ImGui::SetNextWindowSize(ImVec2(500, 500));
	ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	{
		static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

		if (ImGui::Button("Visuals"))
		{
			activeTab = 0;
		}
		ImGui::SameLine(0, -1.0f);
		if (ImGui::Button("Aim"))
		{
			activeTab = 1;
		}
		ImGui::SameLine(0, -1.0f);
		if (ImGui::Button("Misc"))
		{
			activeTab = 2;
		}

		switch (activeTab) {
		case 0:
			visuals();
			break;

		case 1:
			aimbot();
			break;

		case 2:
			misc();
			break;
		}

	}
	ImGui::End();
}

void visuals() {
	ImGui::Checkbox("ESP", &menu::playerEsp);
	
	ImGui::Checkbox("Team ESP", &menu::teamEsp);
	ImGui::SameLine();
	ImGui::ColorEdit4("Team Color", options::colorTeam, ImGuiColorEditFlags_NoInputs);

	ImGui::Checkbox("Enemy ESP", &menu::enemyEsp);
	ImGui::SameLine();
	ImGui::ColorEdit4("Enemy Color", options::colorEnemy, ImGuiColorEditFlags_NoInputs);

	ImGui::Checkbox("Health", &menu::healthEsp);
	ImGui::Checkbox("Weapon", &menu::weaponEsp);
	ImGui::Checkbox("Name", &menu::nameEsp);
}

void aimbot() {
	ImGui::Checkbox("Aimbot", &menu::aimbot);
	if (menu::aimbot) {
		ImGui::SameLine();
		hotkey(&options::aimbotKey, ImVec2(100, 20));
	}

	ImGui::Checkbox("Smoothing", &menu::smooth);
	if (menu::smooth) {
		ImGui::SliderFloat("Smoothness", &options::smoothness, 1.0f, 100.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
	}

	ImGui::Checkbox("FOV", &menu::fov);
	if (menu::fov) {
		ImGui::SliderFloat("FOV Circle", &options::fovCircle, 1.0f, 300.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
	}
}

void misc() {
	ImGui::Checkbox("No Recoil", &menu::noRecoil);
	ImGui::Checkbox("Ammo Hack(Client)", &menu::infAmmo);
	ImGui::Checkbox("No Fire Rate(Client)", &menu::fireRate);
	ImGui::Checkbox("No Gun Animation", &menu::noGunAnimation);
	ImGui::Checkbox("No Gun Knockback", &menu::noGunKnockback);
}


void hotkey(int* k, const ImVec2& size_arg)
{
	static bool waitingforkey = false;
	if (waitingforkey == false) {
		if (ImGui::Button(KeyNames[*(int*)k], size_arg))
			waitingforkey = true;
	}
	else if (waitingforkey == true) {
		ImGui::Button("....", size_arg);
		for (auto& Key : KeyCodes)
		{
			if (GetAsyncKeyState(Key)) {
				if (Key == VK_LBUTTON) {
					break;
				}
				else {
					*(int*)k = Key;
					waitingforkey = false;
				}
			}
		}
	}
}