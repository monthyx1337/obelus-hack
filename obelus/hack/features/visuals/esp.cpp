#include "../features.hpp"

// todo: team, enemy esp (seperated)
void visuals::players::Run()
{
	if (!g::pLocalPlayer || !interfaces::engine->is_in_game())
		return;

	for (auto entity : entity_cache::Groups[CGroupType::PLAYERS_ENEMIES])
	{
		if (!entity)
			continue;

		if (!entity->IsAlive() || entity == g::pLocalPlayer)
			continue;

		const int index = entity->Index();
		if (index > 64)
			continue;

		bool enemy = entity->Enemy(g::pLocalPlayer);
		bool dormant = entity->Dormant();

		// engine radar
		if (config.radar && enemy && !dormant)
			entity->Spotted() = true;

		// handle big dormancy fading 
		float max = config.dormancy;
		float fade = max / 2.f;
		Color color;
		float& opacity = flOpacities[index - 1];
		bool& draw = bDraw[index - 1];
		constexpr int frequency = 1.f / 0.3f;
		float step = frequency * interfaces::globals->frame_time;

		if (!dormant)
			draw = true;

		if (!draw)
			continue;

		dormant ? opacity -= step : opacity += step;

		// we don't need more dormant time for our teammates.
		bool dormant_esp = enemy && config.dormant;

		math::clamp(opacity, 0.f, 1.f);

		if (!opacity && !dormant_esp)
			continue;

		float dt = interfaces::globals->cur_time - entity->SimulationTime();
		if (dormant && dt > max)
			continue;

		// create main opacity variables.
		int alpha = (int)(225.f * opacity);
		int low_alpha = (int)(179.f * opacity);

		if (dormant && dormant_esp)
		{
			alpha = 112;
			low_alpha = 80;

			if (dt > fade)
			{
				float faded = (dt - fade);
				float scale = 1.f - (faded / fade);

				alpha *= scale;
				low_alpha *= scale;
			}

			color = { 112, 112, 112 };
		}

		// apply final opacity.
		color.a = alpha;

		// draw player esp.
		visuals::players::ESP(entity, color, alpha, low_alpha);

	}
}

void visuals::players::ESP(BaseEntity* entity, Color color, int alpha, int low_alpha)
{
	bbox_t box;
	player_info_t info;
	bool enemy = entity->Enemy(g::pLocalPlayer);

	if (!interfaces::engine->get_player_info(entity->Index(), &info))
		return;

	if (!math::GetBoundingBox(entity, box) || !enemy)
		return;

	/* box esp */
	if (config.box)
	{
		// set the fucking colors.
		if (low_alpha == 179)
			color.a = std::clamp(g_menu.ConvertColor(config.box_col).a, 0, 0xb4);
		else
			color.a = std::clamp(low_alpha, 0, g_menu.ConvertColor(config.box_col).a);

		color.r = g_menu.ConvertColor(config.box_col).r;
		color.g = g_menu.ConvertColor(config.box_col).g;
		color.b = g_menu.ConvertColor(config.box_col).b;

		// render the box.
		render::rect_outlined(box.x, box.y, box.w, box.h, color, { 10, 10, 10, low_alpha });
	}

	/* name esp */
	if (config.name)
	{
		std::string name{ std::string(info.name).substr(0, 24) };
		Color clr;

		// set the fucking colors again.
		if (low_alpha == 179)
			clr.a = std::clamp(g_menu.ConvertColor(config.name_col).a, 0, 0xb4);
		else
			clr.a = std::clamp(low_alpha, 0, std::clamp(g_menu.ConvertColor(config.name_col).a, 0, 0xb4));

		clr.r = g_menu.ConvertColor(config.name_col).r;
		clr.g = g_menu.ConvertColor(config.name_col).g;
		clr.b = g_menu.ConvertColor(config.name_col).b;

		// render the name.
		render::esp.string(box.x + box.w / 2, box.y - render::esp.m_size.iHeight, clr, name, render::ALIGN_CENTER);
	}

	/* health bar */
	if (config.health)
	{
		int y = box.y + 1;
		int h = box.h - 2;

		// fix for above 100 hp shit.
		int hp = std::min(100, entity->Health());

		// finalize the bar color.
		int r = std::min((510 * (100 - hp)) / 100, 255);
		int g = std::min((510 * hp) / 100, 255);

		// calculate the height.
		int fill = (int)std::round(hp * h / 100.f);

		// render time!
		render::rect_filled(box.x - 6, y - 1, 4, h + 2, { 10, 10, 10, low_alpha });
		render::rect(box.x - 5, y + h - fill, 2, fill, Color(r, g, 0, alpha));

		// draw the little hp text just like skeet.
		if (hp < 100)
			render::pixel.string(box.x - 5, y + (h - fill) - 5, { 255, 255, 255, low_alpha }, std::to_string(hp), render::ALIGN_CENTER);
	}

	/* flags */
	if (config.flags)
	{
		std::vector< std::pair< std::string, Color > > flags;

		// hp & ammo info.
		if (config.playerflags[5])
		{
			int hp = std::min(100, entity->Health());
			int r = std::min((510 * (100 - hp)) / 100, 255);
			int g = std::min((510 * hp) / 100, 255);
			flags.push_back({ tfm::format(XOR("HEALTH: %i"), entity->Health()), Color(r, g, 0, low_alpha) });

			if (entity->Armor() > 0)
				flags.push_back({ tfm::format(XOR("ARMOR: %i"), entity->Armor()), { 60, 180, 225, low_alpha } });
		}

		// bomb.
		if (entity->HasC4())
			flags.push_back({ XOR("B"), { 255, 0, 0, low_alpha } });

		// money.
		if (config.playerflags[0])
			flags.push_back({ tfm::format(XOR("$%i"), entity->Money()), { 150, 200, 60, low_alpha } });

		// armor.
		if (entity->Armor() > 0 && config.playerflags[1])
			flags.push_back({ entity->HasHelmet() ? XOR("HK") : XOR("K"), { 255, 255, 255, low_alpha } });

		// reload.
		if (entity->Reloading() && config.playerflags[4])
			flags.push_back({ XOR("R"), { 60, 180, 225, low_alpha } });

		// scoped.
		if (entity->IsScoped() && config.playerflags[2])
			flags.push_back({ XOR("ZOOM"), { 60, 180, 225, low_alpha } });

		// flashed.
		if (entity->FlashDuration() > 0.f && config.playerflags[3])
			flags.push_back({ XOR("BLIND"), { 255, 255, 15, low_alpha } });

		// rescuing hostage.
		if (entity->Rescuing())
			flags.push_back({ XOR("H"), { 255, 255, 15, low_alpha } });

		// grabbing hostage.
		if (entity->Grabbing())
			flags.push_back({ XOR("G"), { 255, 255, 15, low_alpha } });

		// iterate flags.
		for (size_t i{ }; i < flags.size(); ++i)
		{
			const auto& f = flags[i];
			int offset = i * (render::pixel.m_size.iHeight - 1);
			render::pixel.string(box.x + box.w + 3, box.y + offset, f.second, f.first);
		}
	}

	/* weapon esp + ammor bar */
	if (config.weapon || config.weapon_icon)
	{
		BaseWeapon* weapon = entity->ActiveWeapon();
		if (weapon)
		{
			CWeaponData* data = weapon->GetWpnData();

			if (data)
			{
				int  offset{ 3 }, bar;
				float scale;

				// the maxclip1 in the weaponinfo
				int max = data->weapon_max_clip;
				int current = weapon->Clip1Count();

				AnimationLayer* animlayer = &entity->AnimOverlays()[1];

				// ammo bar.
				if (max != -1 && config.ammo)
				{
					offset += 5;
					scale = (float)current / max;

					if (entity->Reloading())
						bar = (animlayer->flCycle * box.w) / 1.f;
					else
						bar = (int)(box.w - 2) * scale;

					int iAlpha;
					if (low_alpha == 179)
						iAlpha = std::clamp(g_menu.ConvertColor(config.ammo_col).a, 0, 0xb4);
					else
						iAlpha = std::clamp(low_alpha, 0, std::clamp(g_menu.ConvertColor(config.ammo_col).a, 0, 0xb4));

					render::rect_filled(box.x, box.y + box.h + 2, box.w, 4, { 10, 10, 10, iAlpha });

					// set the fucking colors.
					Color clr;
					if (low_alpha == 179)
						clr.a = std::clamp(g_menu.ConvertColor(config.ammo_col).a, 0, 0xb4);
					else
						clr.a = std::clamp(low_alpha, 0, std::clamp(g_menu.ConvertColor(config.ammo_col).a, 0, 0xb4));
					clr.r = g_menu.ConvertColor(config.ammo_col).r;
					clr.g = g_menu.ConvertColor(config.ammo_col).g;
					clr.b = g_menu.ConvertColor(config.ammo_col).b;

					render::rect(box.x + 1, box.y + box.h + 3, bar, 2, clr);

					// less then a 5th of the bullets left.
					if (current <= (int)std::round(max / 5) && !entity->Reloading())
						render::pixel.string(box.x + bar, box.y + box.h, { 255, 255, 255, low_alpha }, std::to_string(current), render::ALIGN_CENTER);
					else if (entity->Reloading())
						render::pixel.string(box.x + bar, box.y + box.h, { 255, 255, 255, low_alpha }, tfm::format(XOR("%1.0f%%"), std::to_string(static_cast<int>(animlayer->flCycle * 100))), render::ALIGN_CENTER);


				}

				// set the fucking colors again.
				Color weapon_clr;

				if (low_alpha == 179)
					weapon_clr.a = std::clamp(g_menu.ConvertColor(config.weapon_col).a, 0, 0xb4);
				else
					weapon_clr.a = std::clamp(low_alpha, 0, std::clamp(g_menu.ConvertColor(config.weapon_col).a, 0, 0xb4));
				weapon_clr.r = g_menu.ConvertColor(config.weapon_col).r;
				weapon_clr.g = g_menu.ConvertColor(config.weapon_col).g;
				weapon_clr.b = g_menu.ConvertColor(config.weapon_col).b;

				Color weapon_icon_clr;

				if (low_alpha == 179)
					weapon_icon_clr.a = std::clamp(g_menu.ConvertColor(config.weapon_icon_col).a, 0, 0xb4);
				else
					weapon_icon_clr.a = std::clamp(low_alpha, 0, std::clamp(g_menu.ConvertColor(config.weapon_icon_col).a, 0, 230));
				weapon_icon_clr.r = g_menu.ConvertColor(config.weapon_icon_col).r;
				weapon_icon_clr.g = g_menu.ConvertColor(config.weapon_icon_col).g;
				weapon_icon_clr.b = g_menu.ConvertColor(config.weapon_icon_col).b;

				// construct std::string instance of localized weapon name.
				std::string name{ weapon->GetLocalizedName() };
				std::transform(name.begin(), name.end(), name.begin(), ::toupper);

				// weapon esp
				if (config.weapon)
					render::pixel.string(box.x + box.w / 2, box.y + box.h + offset, weapon_clr, name, render::ALIGN_CENTER);

				// weapon icon esp
				if (config.weapon_icon)
				{
					if (weapon->ItemDefinitionIndex() == WEAPON_KNIFE || weapon->ItemDefinitionIndex() == WEAPON_KNIFE_T)
						render::icon2.string(box.x + box.w / 2, box.y + box.h + (offset + (config.weapon ? 10 : 0)) - 4, weapon_icon_clr, "J", render::ALIGN_CENTER);
					else
						render::icon.string(box.x + box.w / 2, box.y + box.h + offset + (config.weapon ? 10 : 0), weapon_icon_clr, weapon->GetWeaponIcon(), render::ALIGN_CENTER);
				}
			}
		}
	}
}

void visuals::Glow()
{

	static Color color;
	BaseEntity* player;

	if (!g::pLocalPlayer)
		return;

	if (!interfaces::glow_manager->vecGlowObjectDefinitions.Count())
		return;

	for (int i{ }; i < interfaces::glow_manager->vecGlowObjectDefinitions.Count(); ++i) 
	{
		GlowObject_t* obj = &interfaces::glow_manager->vecGlowObjectDefinitions[i];

		if (!obj->pEntity || !obj->pEntity->IsPlayer())
			continue;

		player = (BaseEntity*)obj->pEntity;

		// get reference to array variable.
		float& opacity = visuals::players::flOpacities[player->Index() - 1];

		bool enemy = player->Enemy(g::pLocalPlayer);
		bool local = player == g::pLocalPlayer;

		if (enemy)
		{
			color = g_menu.ConvertColor(config.glow_col);
			obj->bRenderWhenOccluded = true;
			obj->bRenderWhenUnoccluded = false;
			obj->bFullBloomRender = false;
			obj->vecColor = { (float)color.r / 255.f, (float)color.g / 255.f, (float)color.b / 255.f };
			obj->flAlpha = opacity * color.a / 255.f;

		}
	}
}


bool visuals::WorldToScreen(const vec3_t& origin, vec3_t& screen)
{
	//return interfaces::debug_overlay->world_to_screen(origin, screen);
	const auto screentransform = [&origin, &screen]() -> bool
	{
		static std::uintptr_t pviewmatrix;
		if (!pviewmatrix)
		{
			pviewmatrix = reinterpret_cast<std::uintptr_t>(pattern::Scan(XOR("client.dll"), XOR("0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9")));
			pviewmatrix = *reinterpret_cast<std::uintptr_t*>(pviewmatrix + 0x3u) + 0xB0u;
		}

		const matrix_t& w2smatrix = *reinterpret_cast<matrix_t*>(pviewmatrix);
		screen.x = w2smatrix.mat_val[0][0] * origin.x + w2smatrix.mat_val[0][1] * origin.y + w2smatrix.mat_val[0][2] * origin.z + w2smatrix.mat_val[0][3];
		screen.y = w2smatrix.mat_val[1][0] * origin.x + w2smatrix.mat_val[1][1] * origin.y + w2smatrix.mat_val[1][2] * origin.z + w2smatrix.mat_val[1][3];
		screen.z = 0.0f;

		float w = w2smatrix.mat_val[3][0] * origin.x + w2smatrix.mat_val[3][1] * origin.y + w2smatrix.mat_val[3][2] * origin.z + w2smatrix.mat_val[3][3];

		if (w < 0.001f)
		{
			screen.x *= 100000;
			screen.y *= 100000;
			return true;
		}

		float invw = 1.f / w;
		screen.x *= invw;
		screen.y *= invw;

		return false;
	};

	if (!screentransform())
	{
		int iscreenwidth, iscreenheight;
		interfaces::engine->GetScreenSize(iscreenwidth, iscreenheight);

		screen.x = (iscreenwidth * 0.5f) + (screen.x * iscreenwidth) * 0.5f;
		screen.y = (iscreenheight * 0.5f) - (screen.y * iscreenheight) * 0.5f;

		return true;
	}
	return false;
}