#pragma once
#include <imgui.h>

namespace Visuals {
	void Draw(Memory& memory, uintptr_t& localPlayer, uintptr_t& entityList);
	void drawBox(ImVec2& headPos, ImVec2& feetPos);
	void drawHealth(ImVec2& headPos, ImVec2& feetPos, int pHealth);
	void drawWeapon(ImVec2& headPos, ImVec2& feetPos, int gunId);
	void drawName(ImVec2& headPos, ImVec2& feetPos, std::string& name);
}