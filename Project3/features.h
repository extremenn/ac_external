#pragma once

#include "memory.h"
#include "structs.h"

void noRecoil(Memory& memory, bool state);
void infAmmo(Memory& memory, bool state);
void fireRate(Memory& memory, bool state);
void noGunAnimation(Memory& memory, bool state);
void noGunKnockback(Memory& memory, bool state);
bool isTeamGame(Memory& memory);