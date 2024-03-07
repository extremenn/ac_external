#pragma once
#include "structs.h"

namespace Offsets {
	constexpr uintptr_t localPlayer{ 0x17E0A8 };
	constexpr uintptr_t entityList{ 0x18AC04 };
	constexpr uintptr_t viewMatrix{ 0x17DFD0 };
	constexpr uintptr_t gameMode{ 0x18ABF8 };

	//Game classes
	constexpr int gunInfoClass = 0xC;
	constexpr int currentWeaponClass = 0x14;

	//Weapon
	constexpr int gunClipAmmo = 0x0;
	constexpr int gunRecoil = 0x60;
	constexpr int gunFireRate = 0x48;
	constexpr int gunKnockBack = 0x54;
	constexpr int gunAnimation = 0x5A;

	struct Player
	{
		char pad_0000[4]; //0x0000
		vec3 hPos; //0x0004
		char pad_0010[24]; //0x0010
		vec3 pPos; //0x0028
		float pYaw; //0x0034
		float pPitch; //0x0038
		char pad_003C[176]; //0x003C
		int32_t pHealth; //0x00EC
		char pad_00F0[277]; //0x00F0
		char pName[15]; //0x0205
		char pad_0214[248]; //0x0214
		int32_t pTeam; //0x030C
		char pad_0310[8]; //0x0310
		int32_t isDead; //0x0318
		char pad_031C[392]; //0x031C
	};

}