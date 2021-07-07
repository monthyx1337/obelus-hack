#include "../../features/features.hpp"

void __fastcall hooks::hkEmitSound(void* pThis, int edx, void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const void* pOrigin, const void* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int something)
{
	static auto ohkEmitSound = detour::emit_sound.GetOriginal<decltype(&hkEmitSound)>();
	if (config.auto_accept && !interfaces::engine->is_connected())
	{
		static auto set_local_ready = 
			reinterpret_cast<bool(__stdcall*)(const char*)>(
				pattern::Scan(XOR("client.dll"), XOR("55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12")));

		if (set_local_ready)
			set_local_ready("");
	}


	ohkEmitSound(pThis, edx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, something);
}