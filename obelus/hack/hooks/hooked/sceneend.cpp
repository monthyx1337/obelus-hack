#include "../../features/features.hpp"

void __fastcall hooks::hkSceneEnd(void* thisptr, void* edx) {
	static auto ohkSceneEnd = detour::scene_end.GetOriginal<decltype(&hkSceneEnd)>();
	ohkSceneEnd(thisptr, edx);
}
