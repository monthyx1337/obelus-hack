#include <intrin.h>
#include "../../features/features.hpp"


void __fastcall hooks::hkDrawModel(void* thisptr, int edx, DrawModelResults_t* pResults, const DrawModelInfo_t& info, matrix_t* pBoneToWorld, float* flFlexWeights, float* flFlexDelayedWeights, const vec3_t& vecModelOrigin, int nFlags)
{
	static auto oDrawModel = detour::draw_model.GetOriginal<decltype(&hkDrawModel)>();

	bool bClearOverride = false;

	if (g::pLocalPlayer)
		bClearOverride = chams::Chams(pResults, info, pBoneToWorld, flFlexWeights, flFlexDelayedWeights, vecModelOrigin, nFlags);

	oDrawModel(thisptr, edx, pResults, info, pBoneToWorld, flFlexWeights, flFlexDelayedWeights, vecModelOrigin, nFlags);

	if (bClearOverride)
		interfaces::studio_render->forced_material_override(nullptr);
}

// disable model occlusion
int __fastcall hooks::hkListLeavesInBox(std::uintptr_t ecx, std::uintptr_t edx, vec3_t& mins, vec3_t& maxs, unsigned short* list, int list_max) {
	// CClientLeafSystem::InsertIntoTree sig "55 8B EC 83 E4 F8 81 EC ? ? ? ? 8B 45 08 53 56 8B 75"
		// [...]
		// .text:1024D4D7                   call    dword ptr[ eax + 18h ]      // called here
		// .text:1024D4DA                   mov[ esp + 930h + leaf_count ], eax // return here ( so u should sig this one... )
		// .text:1024D4DE                   jmp     short loc_1024D4E8          // :)

	static auto ohkListLeavesInBox = detour::list_leaves_in_box.GetOriginal<decltype(&hkListLeavesInBox)>();

	static auto insert_into_tree_call_list_leaves_in_box = reinterpret_cast<std::uintptr_t>(pattern::Scan(XOR("client.dll"), XOR("89 44 24 14 EB 08 C7 44 24 ? ? ? ? ? 8B 45")));

	if (reinterpret_cast<std::uintptr_t>(_ReturnAddress()) != insert_into_tree_call_list_leaves_in_box)
		return ohkListLeavesInBox(ecx, edx, mins, maxs, list, list_max);

	auto info = *reinterpret_cast<RenderableInfo_t**>(reinterpret_cast<std::uintptr_t>(_AddressOfReturnAddress()) + 0x14);

	if (!info || !info->renderable)
		return ohkListLeavesInBox(ecx, edx, mins, maxs, list, list_max);

	auto base_entity = info->renderable->GetIClientUnknown()->GetBaseEntity();

	if (!base_entity || !base_entity->IsPlayer())
		return ohkListLeavesInBox(ecx, edx, mins, maxs, list, list_max);

	info->flags &= ~0x100;
	info->flags2 |= 0xC0;

	constexpr float MAX_COORD_FLOAT = 16384.f;
	constexpr float MIN_COORD_FLOAT = -MAX_COORD_FLOAT;

	static vec3_t map_min = vec3_t(MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT);
	static vec3_t map_max = vec3_t(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);

	return ohkListLeavesInBox(ecx, edx, map_min, map_max, list, list_max);
}

void __fastcall hooks::vars::hkParticleCollectionSimulate(CParticleCollection* thisPtr, void* edx) 
{

	static auto original = reinterpret_cast<bool(__thiscall*)(CParticleCollection * thisPtr)>(oParticleCollectionSimulate);

	if (!config.changemolotov || !interfaces::engine->is_connected())
	{
		original(thisPtr);
		return;
	}

	original(thisPtr);

	CParticleCollection* root_colection = thisPtr;
	while (root_colection->m_pParent)
		root_colection = root_colection->m_pParent;

	const char* root_name = root_colection->m_pDef.m_pObject->m_Name.buffer;

	switch (fnv::hash(root_name))
	{
	case fnv::hash("molotov_groundfire"):
	case fnv::hash("molotov_groundfire_00MEDIUM"):
	case fnv::hash("molotov_groundfire_00HIGH"):
	case fnv::hash("molotov_groundfire_fallback"):
	case fnv::hash("molotov_groundfire_fallback2"):
	case fnv::hash("molotov_explosion"):
	case fnv::hash("explosion_molotov_air"):
	case fnv::hash("extinguish_fire"):
	case fnv::hash("weapon_molotov_held"):
	case fnv::hash("weapon_molotov_fp"):
	case fnv::hash("weapon_molotov_thrown"):
	case fnv::hash("incgrenade_thrown_trail"):
		switch (fnv::hash(thisPtr->m_pDef.m_pObject->m_Name.buffer))
		{
		case fnv::hash("explosion_molotov_air_smoke"):
		case fnv::hash("molotov_smoking_ground_child01"):
		case fnv::hash("molotov_smoking_ground_child02"):
		case fnv::hash("molotov_smoking_ground_child02_cheapo"):
		case fnv::hash("molotov_smoking_ground_child03"):
		case fnv::hash("molotov_smoking_ground_child03_cheapo"):
		case fnv::hash("molotov_smoke_screen"):
			if (config.nosmoke) {
				for (int i = 0; i < thisPtr->m_nActiveParticles; i++) 
				{
					float* pColor = thisPtr->m_ParticleAttributes.FloatAttributePtr(PARTICLE_ATTRIBUTE_ALPHA, i);
					*pColor = 0.f;
				}
			}
			break;
		default:
			for (int i = 0; i < thisPtr->m_nActiveParticles; i++) 
			{
				float* pColor = thisPtr->m_ParticleAttributes.FloatAttributePtr(PARTICLE_ATTRIBUTE_TINT_RGB, i);
				pColor[0] = config.molotov_col[0];
				pColor[4] = config.molotov_col[1];
				pColor[8] = config.molotov_col[2];
			}
			break;
		}
		break;
	}

}