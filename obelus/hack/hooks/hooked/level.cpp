#include "../../features/features.hpp"

void __fastcall hooks::hkLevelInitPostEntity(void* pThis, void* edx)
{
	static auto oLevelInitPostEntity = detour::post_entity.GetOriginal<decltype(&hkLevelInitPostEntity)>();

	client::OnMapload();

	oLevelInitPostEntity(pThis, edx);
}

void __fastcall hooks::hkLevelShutdown(void* pThis, void* edx)
{
	static auto oLevelShutdown = detour::shutdown.GetOriginal<decltype(&hkLevelShutdown)>();

	entity_cache::Clear();
	g::pLocalPlayer = nullptr;
	g::pLocalWeapon = nullptr;
	g::pLocalWeaponData = nullptr;

	oLevelShutdown(pThis, edx);
}
