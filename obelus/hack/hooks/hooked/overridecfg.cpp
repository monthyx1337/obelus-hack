#include "../../features/features.hpp"


bool __fastcall hooks::hkOverrideConfig(void* pThis, void* edx, MaterialSystemConfig_t* pConfig, bool bUpdate)
{
	static auto oOverrideConfig = detour::override_config.GetOriginal<decltype(&hkOverrideConfig)>();

	if (config.fullbright)
		pConfig->uFullbright = true;

	static auto bUpdateNight = false, bToggle = false;
	static auto flBrightness = 100.0f;

	static auto fl1 = config.sky_col[0];
	static auto fl2 = config.sky_col[1];
	static auto fl3 = config.sky_col[2];

	if (bToggle != config.nightmode ||
		flBrightness != config.brightness
		|| fl1 != config.sky_col[0]
		|| fl2 != config.sky_col[1]
		|| fl3 != config.sky_col[2])
	{
		bToggle = config.nightmode;
		flBrightness = config.brightness;
		bUpdateNight = true;
	}

	if (bUpdateNight)
	{
		bUpdateNight = false;
		visuals::ModulateWorld();
	}

	return oOverrideConfig(pThis, edx, pConfig, bUpdate);
}

int __fastcall hooks::vars::hkCsmShadowGetInt() {
	// fuck the blue nigger shit on props with nightmode
	// :nauseated_face:
	return 0;
}