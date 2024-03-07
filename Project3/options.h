#pragma once


namespace options
{
	inline float colorEnemy[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	inline float colorTeam[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	inline float smoothness{ 1 };
	inline float fovCircle{ 0 };
	inline bool noRecoilApplied{ false };

	inline int originalRecoil{ 0 };
	inline short originalClipAmmo{ 0 };
	inline short originalFireRate{ 0 };
	inline int originalGunAnimation{ 0 };
	inline int originalKnockBack{ 0 };

	inline int aimbotKey{ 0 };
}

namespace menu
{
	inline bool menu{ false };
	inline bool noRecoil{ false };
	inline bool infAmmo{ false };
	inline bool fireRate{ false };
	inline bool noGunAnimation{ false };
	inline bool noGunKnockback{ false };
	inline bool noSpread{ false };
	inline bool flyHack{ false };
	inline bool playerEsp{ false };
	inline bool enemyEsp{ false };
	inline bool teamEsp{ false };
	inline bool healthEsp{ false };
	inline bool weaponEsp{ false };
	inline bool nameEsp{ false };
	inline bool aimbot{ false };
	inline bool smooth{ false };
	inline bool fov{ false };
}

namespace toggled {
	inline bool noRecoil{ false };
	inline bool ammoHack{ false };
	inline bool noFireRate{ false };
	inline bool noGunAnimation{ false };
	inline bool noGunKnockback{ false };
}