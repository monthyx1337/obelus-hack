#include "../../client/utilities/csgo.hpp"
#include "../structs/animstate.hpp"

bool BaseEntity::HasC4() 
{
	using HasC4_t = bool(__thiscall*)(decltype(this));
	return g::aHasC4.as< HasC4_t >()(this);
}

void BaseEntity::SetAbsOrigin(const vec3_t& origin) 
{
	using SetAbsOrigin_t = void(__thiscall*)(decltype(this), const vec3_t&);
	g::SetAbsOrigin.as< SetAbsOrigin_t >()(this, origin);
}
c_studio_hdr* BaseEntity::StudioHdr() 
{
	return get< c_studio_hdr* >(g::studioHdr);
}

c_studio_hdr* BaseEntity::ModelPtr() 
{
	using LockStudioHdr_t = void(__thiscall*)(decltype(this));

	if (!StudioHdr())
		g::LockStudioHdr.as< LockStudioHdr_t >()(this);

	return StudioHdr();
}

CSGOPlayerAnimState* BaseEntity::AnimState() {
	// .text:1037A5B8 00C     E8 E3 40 E6 FF         call    C_BasePlayer__Spawn ; Call Procedure
	// .text:1037A5BD 00C     80 BE E1 39 00 00 00   cmp     byte ptr[ esi + 39E1h ], 0; Compare Two Operands
	// .text:1037A5C4 00C     74 48                  jz      short loc_1037A60E; Jump if Zero( ZF = 1 )
	// .text:1037A5C6 00C     8B 8E 74 38 00 00      mov     ecx, [ esi + 3874h ]; this
	// .text:1037A5CC 00C     85 C9                  test    ecx, ecx; Logical Compare
	// .text:1037A5CE 00C     74 3E                  jz      short loc_1037A60E; Jump if Zero( ZF = 1 )
	return get< CSGOPlayerAnimState* >(g::PlayerAnimState);
}

CBoneAccessor& BaseEntity::BoneAccessor() {
	// .text:101A9253 1C4    C7 81 A0 26 00 00 00 FF 0F 00    mov     dword ptr[ ecx + 26A0h ], 0FFF00h
	// .text:101A925D 1C4    C7 81 9C 26 00 00 00 FF 0F 00    mov     dword ptr[ ecx + 269Ch ], 0FFF00h
	// .text:101A9267 1C4    8B 10                            mov     edx, [ eax ]
	// .text:101A9269 1C4    8D 81 94 26 00 00                lea     eax, [ ecx + 2694h ]; Load Effective Address
	// .text:101A926F 1C4    50                               push    eax
	return get< CBoneAccessor >(g::BoneAccessor);
}

void entity_cache::Fill()
{
	if (!g::pLocalPlayer)
		return;

	for (int n = 1; n < interfaces::entity_list->get_highest_index(); n++)
	{
		const auto entity = interfaces::entity_list->get<BaseEntity>(n);
		if (!entity)
			continue;

		if (entity->IsPlayer())
		{
			Groups[CGroupType::PLAYERS_ALL].push_back(entity);
			Groups[entity->TeamNum() != g::pLocalPlayer->TeamNum() ? CGroupType::PLAYERS_ENEMIES : CGroupType::PLAYERS_TEAMMATES].push_back(entity);
		}
		else
		{
			if (!entity->Dormant())
				Groups[CGroupType::WORLD_ALL].push_back(entity);
		}
	}
}

void entity_cache::Clear()
{
	for (auto& group : Groups)
		group.second.clear();
}

const std::vector<BaseEntity*>& entity_cache::GetEntityGroup(const CGroupType& group)
{
	return Groups[group];
}

void entity_cache::Update()
{
	static int lastFrame;

	if (lastFrame == interfaces::globals->frame_count)
		return;

	lastFrame = interfaces::globals->frame_count;

	pInfernoData.clear();
	
	for (auto entity : entity_cache::Groups[CGroupType::WORLD_ALL])
	{
		if (!entity)
			continue;

		if (!entity->IsPlayer())
		{
			if (entity->Dormant())
				continue;

			switch (entity->GetClientClass()->m_ClassID) 
			{
			case cinferno:
				pInfernoData.emplace_back(entity);
				break;
			}
		}
	}

}

entity_cache::InfernoData::InfernoData(BaseEntity* inferno) noexcept
{
	const auto& origin = inferno->AbsOrigin();

	points.reserve(*inferno->FireXDelta());
	for (int i = 0; i < inferno->FireCount(); ++i) {
		if (inferno->FireIsBurning()[i])
			points.emplace_back(inferno->FireXDelta()[i] + origin.x, inferno->FireYDelta()[i] + origin.y, inferno->FireZDelta()[i] + origin.z);
	}
}
