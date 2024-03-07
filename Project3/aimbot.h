#pragma once

#include "memory.h"

void Aimbot(Memory& memory, uintptr_t& localPlayer, uintptr_t& entityList);
uintptr_t getClosestTargetToCrosshair(Memory& memory, uintptr_t& entityList, int& playerTeam, int& playerCount, bool isTeamMode);
void drawFovCircle();