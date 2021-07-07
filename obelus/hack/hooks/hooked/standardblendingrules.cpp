#include "../../features/features.hpp"


_declspec(noinline)void hkStandardBlendingRulesDT(BaseEntity* player, int i, studio_hdr_t* hdr, vec3_t* pos, quaternion* q, float curtime, int boneMask)
{
	static auto original = reinterpret_cast<bool(__thiscall*)(BaseEntity * player, int i, studio_hdr_t * hdr, vec3_t * pos, quaternion * q, float curtime, int boneMask)>(hooks::player::oStandardBlendingRules);

	if (!player || (player->Index() - 1) > 63)
		 original(player, i, hdr, pos, q, curtime, boneMask);
		return;

	// disable interpolation.
	if (!(player->Effects() & EF_NOINTERP))
		player->Effects() |= EF_NOINTERP;

	original(player, i, hdr, pos, q, curtime, boneMask);

	// restore interpolation.
	player->Effects() &= ~EF_NOINTERP;
}

void __fastcall hooks::player::hkStandardBlendingRules(BaseEntity* player, int i, studio_hdr_t* hdr, vec3_t* pos, quaternion* q, float curtime, int boneMask)
{
	return hkStandardBlendingRulesDT(player, i, hdr, pos, q, curtime, boneMask);
}