#pragma once

namespace visuals
{
	bool WorldToScreen(const vec3_t&, vec3_t&);

	namespace players
	{
		void Run();
		void ESP(BaseEntity*, Color, int, int);
		void OffScreen(BaseEntity* player, int alpha);
		static std::array< bool, 64 > bDraw;
		static std::array< float, 2048 > flOpacities;
	}

	namespace world
	{
		void Run();
		void ESP(BaseEntity*);
		void PlantedBomb(BaseEntity*);
	}

	void Scope();
	void Glow();
}