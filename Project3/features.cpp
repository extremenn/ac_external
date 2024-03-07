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

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

extern int wWidth;
extern int wHeight;

void noRecoil(Memory& memory, bool state)
{
	uintptr_t recoilAddr = memory.ReadMemoryChain(memory.moduleBase + Offsets::localPlayer, { 0x364, 0xC, 0x60 });

	if (options::originalRecoil == 0)
	{
		options::originalRecoil = memory.ReadMemory<int>(recoilAddr);
	}
	memory.WriteMemory(recoilAddr, state ? 0 : options::originalRecoil);
}

void infAmmo(Memory& memory, bool state) {

	uintptr_t ammoInClipAddr = memory.ReadMemoryChain(memory.moduleBase + Offsets::localPlayer, { 0x364, Offsets::currentWeaponClass, Offsets::gunClipAmmo });
	if (options::originalClipAmmo == 0)
	{
		options::originalClipAmmo = memory.ReadMemory<short>(ammoInClipAddr);
	}
	memory.WriteMemory(ammoInClipAddr, state ? 99999 : options::originalClipAmmo);
}

void fireRate(Memory& memory, bool state) {

	uintptr_t fireRateAddr = memory.ReadMemoryChain(memory.moduleBase + Offsets::localPlayer, { 0x364, Offsets::gunInfoClass, Offsets::gunFireRate });

	if (options::originalFireRate == 0)
	{
		options::originalFireRate = memory.ReadMemory<short>(fireRateAddr);
	}
	memory.WriteMemory(fireRateAddr, state ? short{ 0 } : options::originalFireRate);
}

void noGunAnimation(Memory& memory, bool state) {

	uintptr_t noAnimationAddr = memory.ReadMemoryChain(memory.moduleBase + Offsets::localPlayer, { 0x364, Offsets::gunInfoClass, Offsets::gunAnimation });

	if (options::originalGunAnimation == 0)
	{
		options::originalGunAnimation = memory.ReadMemory<int>(noAnimationAddr);
	}
	memory.WriteMemory(noAnimationAddr, state ? short{ 0 } : options::originalGunAnimation);
}

void noGunKnockback(Memory& memory, bool state) {

	uintptr_t gunKnockBackAddr = memory.ReadMemoryChain(memory.moduleBase + Offsets::localPlayer, { 0x364, Offsets::gunInfoClass, Offsets::gunKnockBack });

	if (options::originalKnockBack == 0)
	{
		options::originalKnockBack = memory.ReadMemory<int>(gunKnockBackAddr);
	}
	memory.WriteMemory(gunKnockBackAddr, state ? short{ 0 } : options::originalKnockBack);
}

bool isTeamGame(Memory& memory) {
	int gameMode = memory.ReadMemory<int>(memory.moduleBase + Offsets::gameMode);

	enum teamGameModes {
		// Bots
		aBotTeamDeathmatch = 7,
		aBotTeamSurvivor = 20,
		aBotTeamOneShot = 21,
		// Multiplayer
		aTeamSurvivor = 4,
		aCaptureTheFlag = 5,
		aTeamOneShot = 11,
		aHuntTheFlag = 13,
		aTeamKeepTheFlag = 14,
		aTeamPistolFrenzy = 16,
		aTeamLastSwissStanding = 17,
	};

	switch (gameMode) {
	case aBotTeamDeathmatch:
	case aBotTeamSurvivor:
	case aBotTeamOneShot:
	case aTeamSurvivor:
	case aCaptureTheFlag:
	case aTeamOneShot:
	case aHuntTheFlag:
	case aTeamKeepTheFlag:
	case aTeamPistolFrenzy:
	case aTeamLastSwissStanding:
		return true;
	default:
		return false;
	}
}