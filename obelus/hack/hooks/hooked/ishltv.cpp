#include "../../features/features.hpp"

bool __fastcall hooks::hkIsHLTV(IVEngineClient* IEngineClient)
{
	static auto IsHLTV = detour::is_hl_tv.GetOriginal<decltype(&hkIsHLTV)>();

	static const auto SetupVelocity = pattern::Scan(XOR("client.dll"), XOR("84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80"));
	static const auto AccumulateLayers = pattern::Scan(XOR("client.dll"), XOR("84 C0 75 0D F6 87"));

	if (_ReturnAddress() == (uint32_t*)(AccumulateLayers) && IEngineClient->is_in_game())
		return true;

	if (_ReturnAddress() == (uint32_t*)(SetupVelocity) && g::bUpdateMatrix && IEngineClient->is_in_game())
		return true;

	return IsHLTV(IEngineClient);
}