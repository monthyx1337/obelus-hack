#include "../../features/features.hpp"

void __fastcall hooks::player::hkDoExtraBoneProcessing(void* ecx, void* edx, studio_hdr_t* hdr, vec3_t* pos, quaternion* q, const matrix_t& matrix, byte* boneComputed, void* context)
{
	static auto original = reinterpret_cast<bool(__thiscall*)(void * ecx, void* edx, studio_hdr_t * hdr, vec3_t * pos, quaternion * q, const matrix_t & matrix, byte * boneComputed, void* context)>(hooks::player::oDoExtraBoneProcessing);

	BaseEntity* player = reinterpret_cast<BaseEntity*>(ecx);

	if (!player)
		original(ecx, edx, hdr, pos, q, matrix, boneComputed, context);
	
	if (player->IsPlayer() && player->IsAlive())
	{
		int* v1 = reinterpret_cast<int*>(*reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(player) + 0x2990) + 0x30);
		for (int i = 13; i; --i)
		{
			if (reinterpret_cast<BaseEntity*>(v1) != player)
			{
				*v1 = reinterpret_cast<uintptr_t>(player);
			}

			v1 += 14;
		}
	}

	return;
}

