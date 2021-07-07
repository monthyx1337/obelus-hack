#include "../../features/features.hpp"

void __fastcall hooks::player::hkBuildTransformations(BaseEntity* player, uint32_t xe, c_studio_hdr* hdr, vec3_t* pos, quaternion* q, const matrix_t& transform, int32_t mask, byte* computed)
{
	if (g::pCmd == nullptr)
		oBuildTransformations(player, hdr, pos, q, transform, mask, computed);

	const auto backup = hdr->bone_flags;

	for (auto index = 0; index < hdr->bone_flags.Count(); index++)
	{
		hdr->bone_flags.Element(index) &= ~bone_always_procedural;
	}

	oBuildTransformations(player, hdr, pos, q, transform, mask, computed);
	hdr->bone_flags = backup;
}