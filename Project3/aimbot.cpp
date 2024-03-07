#include "aimbot.h"
#include "structs.h"
#include "memory.h"
#include "offsets.h"
#include "math.h"
#include "options.h"
#include "features.h"

#include <imgui.h>

#define M_PI 3.14159265358979323846

extern int wWidth;
extern int wHeight;

void Aimbot(Memory& memory, uintptr_t& localPlayer, uintptr_t& entityList) {

	Offsets::Player lPlayer{ memory.ReadMemory<Offsets::Player>(localPlayer) };
	int playerCount{ memory.ReadMemory<int>((memory.moduleBase + Offsets::entityList) + 0x8) };
	bool isTeamMode{ isTeamGame(memory) };

	for (int i{ 0 }; i < playerCount; ++i)
	{
		uintptr_t entityAdr{ getClosestTargetToCrosshair(memory, entityList, lPlayer.pTeam, playerCount, isTeamMode) };
		Offsets::Player entity{ memory.ReadMemory<Offsets::Player>(entityAdr) };
		bool isTeam{ entity.pTeam == lPlayer.pTeam };

		vec3 resultVector = entity.hPos - lPlayer.hPos;

		float yaw = -atan2(resultVector.x, resultVector.y) * (180 / M_PI) + 180;
		float pitch = atan2(resultVector.z, sqrt(resultVector.x * resultVector.x + resultVector.y * resultVector.y)) * (180 / M_PI);

		if (!isTeamMode) {
			isTeam = false;
		}

		if (!entity.isDead && !isTeam && entityAdr != NULL) {

			std::string resultX = std::to_string(resultVector.x);
			std::string resultY = std::to_string(resultVector.y);
			std::string resultZ = std::to_string(resultVector.z);

			if (menu::smooth) {
				vec2 calcAngles{ yaw - lPlayer.pYaw, pitch - lPlayer.pPitch };
				yaw = lPlayer.pYaw + calcAngles.x / options::smoothness;
				pitch = lPlayer.pPitch + calcAngles.y / options::smoothness;
			}

			std::string resultText = "Result: (" + resultX + ", " + resultY + ", " + resultZ + ")";
			ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(200, 200), IM_COL32(255, 255, 255, 255), resultText.c_str());
			
			memory.WriteMemory<float>(localPlayer + offsetof(Offsets::Player, pYaw), yaw);
			memory.WriteMemory<float>(localPlayer + offsetof(Offsets::Player, pPitch), pitch);

		}

	}
}

uintptr_t getClosestTargetToCrosshair(Memory& memory, uintptr_t& entityList, int& playerTeam, int& playerCount, bool isTeamMode)
{
	vec2 centerScreen{ wWidth / 2, wHeight / 2 };
	vec2 entityHeadPos2D;

	uintptr_t bestTarget{};

	int closestToCrosshair{ 999999 };
	float fov{300};

	for (int i{ 0 }; i < playerCount; ++i)
	{
		uintptr_t entityAdr{ memory.ReadMemory<uintptr_t>(entityList + (0x4 * i)) };
		Offsets::Player entity{ memory.ReadMemory<Offsets::Player>(entityAdr) };
		bool isTeam{};

		if (isTeamMode) {
			isTeam = entity.pTeam == playerTeam;
		}
		else {
			isTeam = false;
		}

		if (!entity.isDead && !isTeam && entityAdr != NULL) {

			viewMatrix matrix{ memory.ReadMemory<viewMatrix>(memory.moduleBase + Offsets::viewMatrix) };
			worldToScreen(entity.hPos, entityHeadPos2D, matrix, wWidth, wHeight);

			float distance = sqrt((centerScreen.x - entityHeadPos2D.x) * (centerScreen.x - entityHeadPos2D.x) + (centerScreen.y - entityHeadPos2D.y) * (centerScreen.y - entityHeadPos2D.y));

			if (menu::fov) {
				if (distance < options::fovCircle && distance < closestToCrosshair)
				{
					bestTarget = entityAdr;
					closestToCrosshair = distance;
				}
			}
			else {
				if (distance < fov)
				{
					fov = distance;
					bestTarget = entityAdr;
				}
			}
		}

	}

	return bestTarget;
}

void drawFovCircle() {
	ImVec2 center{ (float)wWidth / 2, (float)wHeight / 2 + 12 };
	ImGui::GetBackgroundDrawList()->AddCircle(center, options::fovCircle, IM_COL32(255, 255, 255, 255), 0, 0.001f);
}