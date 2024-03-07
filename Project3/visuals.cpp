#include <d3d11.h>
#include <algorithm>
#include <DirectXMath.h>
#include <cmath>

#include "memory.h"
#include "options.h"
#include "math.h"
#include "structs.h"
#include "features.h"
#include "offsets.h"
#include "visuals.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

extern int wWidth;
extern int wHeight;

ImVec4 color{};

namespace Visuals{

	void Draw(Memory& memory, uintptr_t& localPlayer, uintptr_t& entityList) {
		Offsets::Player lPlayer{ memory.ReadMemory<Offsets::Player>(localPlayer) };
		int playerCount{ memory.ReadMemory<int>((memory.moduleBase + Offsets::entityList) + 0x8) };
		viewMatrix matrix{ memory.ReadMemory<viewMatrix>(memory.moduleBase + Offsets::viewMatrix) };
		bool isTeamMode{ isTeamGame(memory) };

		vec2 screenHead{};
		vec2 screenFeet{};

		for (int i{ 0 }; i < playerCount; ++i)
		{
			uintptr_t entityAdr{ memory.ReadMemory<uintptr_t>(entityList + (0x4 * i)) };
			Offsets::Player entity{ memory.ReadMemory<Offsets::Player>(entityAdr) };
			bool isTeam{ entity.pTeam == lPlayer.pTeam };

			if (!isTeamMode) {
				isTeam = false;
			}
	
			if (!entityAdr) {
				continue;
			}
			if (entity.isDead) {
				continue;
			}

			if (isTeam) {
				color = { (options::colorTeam[0] * 255.0f), (options::colorTeam[1] * 255.0f), (options::colorTeam[2] * 255.0f), (options::colorTeam[3] * 255.0f) };
			}
			else
			{
				color = { (options::colorEnemy[0] * 255.0f), (options::colorEnemy[1] * 255.0f), (options::colorEnemy[2] * 255.0f), (options::colorEnemy[3] * 255.0f) };
			}

			if (!(worldToScreen(entity.hPos, screenHead, matrix, wWidth, wHeight) && worldToScreen(entity.pPos, screenFeet, matrix, wWidth, wHeight))) {
				continue;
			}
			ImVec2 headPos(screenHead.x, screenHead.y);
			ImVec2 feetPos(screenFeet.x, screenFeet.y);

			if (menu::enemyEsp && !isTeam) {
				drawBox(headPos, feetPos);
			}
			else if (menu::teamEsp && isTeam) {
				drawBox(headPos, feetPos);
			}

			if (menu::healthEsp && menu::enemyEsp && !isTeam) {
				drawHealth(headPos, feetPos, entity.pHealth);
			}
			else if (menu::healthEsp && menu::teamEsp && isTeam) {
				drawHealth(headPos, feetPos, entity.pHealth);
			}

			if (menu::nameEsp) {
				std::string playerName(entity.pName, sizeof(entity.pName));
				drawName(headPos, feetPos, playerName);
			}

			if (menu::weaponEsp) {
				uintptr_t gunIdAdr{ memory.ReadMemory<uintptr_t>(entityAdr + 0x364) };
				int gunId{ memory.ReadMemory<int>(gunIdAdr + 0x4) };
				drawWeapon(headPos, feetPos, gunId);
			}
		}

	}

	void drawBox(ImVec2& headPos, ImVec2& feetPos) {

		float boxHeight = headPos.y - feetPos.y;
		float boxWidth = boxHeight / 2.6f;

		float halfBoxWidth = boxWidth / 2.0f;

		ImVec2 startBoxPos(headPos.x - halfBoxWidth, headPos.y);
		ImVec2 endBoxPos(feetPos.x + halfBoxWidth, feetPos.y);

		ImGui::GetBackgroundDrawList()->AddRect(startBoxPos, endBoxPos, IM_COL32(color.x, color.y, color.z, color.w), 0.0f, NULL, 1.5f);
		
	}

	void drawHealth(ImVec2& headPos, ImVec2& feetPos, int pHealth) {
		float distFromtEnt = 1.5f;
		float barWidth = 2.0f;

		float height = feetPos.y - headPos.y;
		float playerWidth = height / 5;

		float top = headPos.y + height - (height * pHealth / 100.f);
		float left = feetPos.x - playerWidth - distFromtEnt - barWidth;
		float right = feetPos.x - playerWidth - distFromtEnt;
		float bottom = feetPos.y;

		ImVec2 start(left, top );
		ImVec2 stop(right, bottom);

		ImGui::GetBackgroundDrawList()->AddRectFilled(start, stop, ImColor(0, 255, 0));
		ImGui::GetBackgroundDrawList()->AddText(stop, ImColor(0, 255, 0), std::to_string(pHealth).c_str());
	}

	void drawWeapon(ImVec2& headPos, ImVec2& feetPos, int gunId){
		enum {
			KNIFE, PISTOL, CARBINE, SHOTGUN, SUBGUN, SNIPER, ASSAULT, GRENADE
		};

		float height = feetPos.y - headPos.y;
		float playerWidth = height / 5;

		ImVec2 drawPosition{ headPos.x + playerWidth + 1.5f, headPos.y };

		std::string gunName;
		switch (gunId) {
		case KNIFE:
			gunName = "KNIFE";
			break;
		case PISTOL:
			gunName = "PISTOL";
			break;
		case CARBINE:
			gunName = "CARBINE";
			break;
		case SHOTGUN:
			gunName = "SHOTGUN";
			break;
		case SUBGUN:
			gunName = "SUBGUN";
			break;
		case SNIPER:
			gunName = "SNIPER";
			break;
		case ASSAULT:
			gunName = "ASSAULT";
			break;
		case GRENADE:
			gunName = "GRENADE";
			break;
		default:
			gunName = "UNKNOWN";
			break;
		}

		ImGui::GetBackgroundDrawList()->AddText(drawPosition, ImColor(255, 255, 255), gunName.c_str());
	}

	void drawName(ImVec2& headPos, ImVec2& feetPos, std::string& name){

		float height = feetPos.y - headPos.y;
		float playerWidth = height / 5;
		ImVec2 start(headPos.x + playerWidth + 1.5f, headPos.y - 15.0f);

		ImGui::GetBackgroundDrawList()->AddText(start, ImColor(0, 0, 0), name.c_str());;
	}


}