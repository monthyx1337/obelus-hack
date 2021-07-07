#include "../../features/features.hpp"


int	__fastcall	hooks::hkDoPostScreenEffects(void* thisptr, int edx, void* pSetup)
{
	static auto oDoPostScreenEffects = detour::post_screen_effects.GetOriginal<decltype(&hkDoPostScreenEffects)>();

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return oDoPostScreenEffects(thisptr, edx, pSetup);

	if (g::pLocalPlayer && interfaces::glow_manager && config.glow)
		visuals::Glow();

	return oDoPostScreenEffects(thisptr, edx, pSetup);
}
