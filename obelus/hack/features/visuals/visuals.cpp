#include "../features.hpp"

void visuals::world::Run()
{
	for (auto entity : entity_cache::Groups[CGroupType::WORLD_ALL])
	{
		if (!entity)
			continue;

		// player esp.
		visuals::world::ESP(entity);

		// planted bomb esp.
		visuals::world::PlantedBomb(entity);
	}

	// big flashlight.
	visuals::RunFlashlight();

	// scope overlay.
	visuals::Scope();

	// paint movement stuff.
	visuals::PaintMovementData();

	// render grenade prediction.
	grenades::PaintNadePred();
}

void visuals::world::ESP(BaseEntity* entity)
{
	BaseWeapon* item = (BaseWeapon*)entity;

	// world items
	if (entity->IsBaseCombatWeapon())
	{
		// we only want to draw shit without owner.
		BaseEntity* owner = (BaseEntity*)interfaces::entity_list->get_client_entity_handle(item->OwnerHandle());

		vec2_t screen;
		vec3_t origin = item->AbsOrigin();

		// bruh is it even on the screen?
		if (render::WorldToScreen(origin, screen) && !owner)
		{
			CWeaponData* data = item->GetWpnData();
			if (data)
			{
				// dropped c4
				if (item->is(HASH("CC4")) && config.bomb)
				{
					render::pixel.string(screen.x, screen.y, { 150, 200, 60, 0xb4 }, XOR("BOMB"), render::ALIGN_CENTER);
				}
				// dropped weapons
				else
				{
					if (config.droppedweapons[0] || config.droppedweapons[1] && !(item->ItemDefinitionIndex() == WEAPON_TASER && item->Clip1Count() == 0))
					{
						std::string name{ item->GetLocalizedName() };
						std::transform(name.begin(), name.end(), name.begin(), ::toupper);

						Color clr;
						clr.r = g_menu.ConvertColor(config.dropped_col).r;
						clr.g = g_menu.ConvertColor(config.dropped_col).g;
						clr.b = g_menu.ConvertColor(config.dropped_col).b;
						clr.a = g_menu.ConvertColor(config.dropped_col).a;

						// note: i do be clamping the alpha cuz i wanna prevent the users from making the visuals super ugly hahhadsadshasdh

						if (config.droppedweapons[0])
						{
							// dropped text.
							render::pixel.string(screen.x, screen.y - (config.droppedweapons[1] ? 16 : 0), { clr.r,
																	   clr.g,
																	   clr.b,
																	   std::clamp(clr.a, 0, 0xb4) }, name, render::ALIGN_CENTER);
						}

						if (config.droppedweapons[1])
						{
							// dropped icon.
							render::icon.string(screen.x, screen.y - 6, { clr.r,
																		   clr.g,
																		   clr.b,
																		   std::clamp(clr.a, 0, 0xb4) }, item->GetWeaponIcon(), render::ALIGN_CENTER);
						}

						if (config.dropped_ammo && item->ItemDefinitionIndex() != 0 && item->Clip1Count() > -1)
						{
							int w = 40, h;
							//interfaces::surface->get_text_size(render::pixel.m_handle, (wchar_t*)name.c_str(), w, h);

							const auto ammo = item->Clip1Count();
							const auto max_ammo = data->weapon_max_clip;
							auto width = (w / 2 + 1) - 2;
							width *= ammo;
							width /= max_ammo;

							Color clr;
							clr.r = g_menu.ConvertColor(config.dropped_ammo_col).r;
							clr.g = g_menu.ConvertColor(config.dropped_ammo_col).g;
							clr.b = g_menu.ConvertColor(config.dropped_ammo_col).b;
							clr.a = g_menu.ConvertColor(config.dropped_ammo_col).a;

							render::rect_filled(screen.x - 11, screen.y + 10, w / 2 + 1 + 1, 4, { 10, 10, 10, 0xb4 });
							render::rect_filled(screen.x - 10, screen.y + 11, width + 1, 2, { clr.r,
																							clr.g,
																							clr.b,
																							std::clamp(clr.a, 0, 0xb4) });
							// less then a 5th of the bullets left.
							if (ammo <= (int)std::round(max_ammo / 5))
								render::pixel.string(screen.x - 10 + width, screen.y + 11 - 2 - 1, { 250, 250, 250, 0xb4 }, std::to_string(ammo), render::ALIGN_CENTER);

						}
					}
				}
			}
		}
	}

	// projectitles
	if (config.nades)
	{
		vec2_t screen;
		vec3_t origin = item->AbsOrigin();

		if (render::WorldToScreen(origin, screen))
		{
			Color col = g_menu.ConvertColor(config.nades_col);

			if (item->is(HASH("CDecoyProjectile")))
			{
				render::pixel.string(screen.x, screen.y, col, XOR("DECOY"), render::ALIGN_CENTER);
			}
			else if (item->is(HASH("CMolotovProjectile")))
			{
				render::pixel.string(screen.x, screen.y, col, XOR("FIRE"), render::ALIGN_CENTER);
			}
			else if (item->is(HASH("CBaseCSGrenadeProjectile")))
			{
				const model_t* model = item->Model();

				if (model)
				{
					std::string name{ item->Model()->name };

					if (name.find(XOR("flashbang")) != std::string::npos)
					{
						render::pixel.string(screen.x, screen.y, col, XOR("FLASH"), render::ALIGN_CENTER);
					}
					else if (name.find(XOR("fraggrenade")) != std::string::npos)
					{
						render::pixel.string(screen.x, screen.y, col, XOR("FRAG"), render::ALIGN_CENTER);
					}
				}
			}
			else if (item->is(HASH("CInferno")))
			{
				render::pixel.string(screen.x, screen.y, col, XOR("FIRE"), render::ALIGN_CENTER);

				const float flTime = 7.f - (interfaces::globals->interval_per_tick * (interfaces::globals->tick_count - item->FireEffectTickBegin()));
				if (flTime > 0) {
					render::rect(screen.x - 14, screen.y + 11 - 1, 30, 5, Color(0, 0, 0, 200));

					if (230 - 90 > 0)
						render::rect_filled(screen.x - 13, screen.y + 11, 28, 3, Color(10, 10, 10, 230 - 90));

					render::rect_filled(screen.x - 13, screen.y + 11, flTime * 4, 3, Color(240, 240, 240, 200));
				}

			}
			else if (item->is(HASH("CSmokeGrenadeProjectile")))
			{
				render::pixel.string(screen.x, screen.y, col, XOR("SMOKE"), render::ALIGN_CENTER);
			}
		}
	}
}


void visuals::world::PlantedBomb(BaseEntity* ent)
{
	
}


void visuals::Scope()
{
	if (config.noscope && g::pLocalPlayer->IsAlive() && g::pLocalPlayer->ActiveWeapon() && g::pLocalPlayer->ActiveWeapon()->IsZoomable(false) 
		&& g::pLocalPlayer->IsScoped()) 
	{
		// calculate size.
		int w = g::iWidth,
			h = g::iHeight,
			x = w / 2,
			y = h / 2,
			size = convars::cl_crosshair_sniper_width->GetInt();

		if (size > 1) 
		{
			x -= (size / 2);
			y -= (size / 2);
		}

		// draw the scope line.
		render::rect_filled(0, y, w, size, Color(0, 0, 0, 255));
		render::rect_filled(x, 0, size, h, Color(0, 0, 0, 255));
	}
}

void visuals::ModulateWorld()
{
	if (!g::pLocalPlayer || !interfaces::engine->is_in_game())
		return;

	std::vector< i_material* > vecWorld, vecProps, vecSky;

	for (
		uint16_t h = interfaces::material_system->first_material();
		h != interfaces::material_system->invalid_material_handle();
		h = interfaces::material_system->next_material(h))
	{
		i_material* pMaterial = interfaces::material_system->get_material(h);
		if (!pMaterial || pMaterial->is_error_material())
			continue;

		switch (FNV1A::HashConst(pMaterial->get_texture_group_name()))
		{
		case FNV1A::HashConst(("World textures")):
			vecWorld.push_back(pMaterial);
			break;

		case FNV1A::HashConst(("StaticProp textures")):
			vecProps.push_back(pMaterial);
			break;

		case FNV1A::HashConst(("SkyBox textures")):
			vecSky.push_back(pMaterial);
			break;
		}
	}

	if (config.nightmode) {


		const float flWorld = 1.f - (config.brightness / 100.f);
		const float flProps = 1.3f - (config.brightness / 100.f);

		for (const auto& hWorld : vecWorld)
			hWorld->color_modulate(flWorld, flWorld, flWorld);

		convars::r_drawspecificstaticprop->SetValue(0);

		for (const auto& hProps : vecProps)
			hProps->color_modulate(flProps, flProps, flProps);

		for (const auto& hSky : vecSky)
			hSky->color_modulate(config.sky_col[0], config.sky_col[1], config.sky_col[2]);
	}
	else 
	{
		for (const auto& hWorld : vecWorld)
			hWorld->color_modulate(1.f, 1.f, 1.f);
		
		convars::r_drawspecificstaticprop->SetValue(-1);

		for (const auto& hProps : vecProps)
			hProps->color_modulate(1.f, 1.f, 1.f);


		for (const auto& hSky : vecSky)
			hSky->color_modulate(1.f, 1.f, 1.f);
	}
}

CFlashLightEffect* visuals::CreateFlashlight(int nEntIndex, const char* pszTextureName, float flFov, float flFarZ, float flLinearAtten)
{
	static DWORD oConstructor = (DWORD)pattern::Scan(XOR("client.dll"), XOR("55 8B EC F3 0F 10 45 ? B8"));

	CFlashLightEffect* pFlashLight = (CFlashLightEffect*)interfaces::mem_alloc->Alloc(sizeof(CFlashLightEffect));
	if (!pFlashLight)
		return NULL;

	__asm
	{
		movss xmm3, flFov
		mov ecx, pFlashLight
		push flLinearAtten
		push flFarZ
		push pszTextureName
		push nEntIndex
		call oConstructor
	}

	return pFlashLight;
}

void visuals::DestroyFlashlight(CFlashLightEffect* pFlashlight)
{
	static DWORD oDestructor = (DWORD)pattern::Scan(XOR("client.dll"), XOR("56 8B F1 E8 ? ? ? ? 8B 4E 28"));
	__asm
	{
		mov ecx, pFlashlight
		push ecx
		call oDestructor
	}
}

void visuals::UpdateFlashlight(CFlashLightEffect* pFlashLight, const vec3_t& vecPos, const vec3_t& vecForward, const vec3_t& vecRight, const vec3_t& vecUp)
{
	typedef void(__thiscall* UpdateLight_t)(void*, int, const vec3_t&, const vec3_t&, const vec3_t&, const vec3_t&, float, float, float, bool, const char*);

	static UpdateLight_t oUpdateLight = NULL;
	if (!oUpdateLight)
	{
		DWORD callInstruction = (DWORD)pattern::Scan(XOR("client.dll"), XOR("E8 ? ? ? ? 8B 06 F3 0F 10 46")); // get the instruction address
		DWORD relativeAddress = *(DWORD*)(callInstruction + 1); // read the rel32
		DWORD nextInstruction = callInstruction + 5; // get the address of next instruction
		oUpdateLight = (UpdateLight_t)(nextInstruction + relativeAddress); // our function address will be nextInstruction + relativeAddress
	}

	oUpdateLight(pFlashLight, pFlashLight->m_nEntIndex, vecPos, vecForward, vecRight, vecUp, pFlashLight->m_flFov, pFlashLight->m_flFarZ, pFlashLight->m_flLinearAtten, pFlashLight->m_bCastsShadows, pFlashLight->m_szTextureName);
}

void visuals::RunFlashlight()
{
	//static CFlashLightEffect* pFlashLight = NULL;
	//if (!C::Get<bool>(Vars.flashlight) ||
	//	!interfaces::engine->is_in_game() ||
	//	!g::pLocalPlayer)
	//{
	//	if (pFlashLight)
	//	{
	//		DestroyFlashlight(pFlashLight);
	//		pFlashLight = NULL;
	//	}
	//	return;
	//}

	//static bool bSound = true;
	//static bool bIsOn = false;
	//static bool bToggled = false;
	//bIsOn = GUI::UTILS::KeybindMethod(C::Get<int>(Vars.flashlight_key), C::Get<int>(Vars.flashlight_key_method), &bToggled);

	//if (bIsOn)
	//{
	//	if (!pFlashLight)
	//		pFlashLight = CreateFlashlight(g::pLocalPlayer->Index(), (XOR("effects/flashlight001")), C::Get<float>(Vars.flashlight_fov), 1000, 1000);
	//}
	//else
	//{
	//	if (pFlashLight)
	//	{
	//		DestroyFlashlight(pFlashLight);
	//		pFlashLight = NULL;
	//	}
	//	return;
	//}

	////if (bIsOn && bSound)
	////{
	////	PlaySound("C:\\flashlight1.wav", NULL, SND_FILENAME | SND_ASYNC);
	////	bSound = false;
	////}
	////else if (!bIsOn && !bSound)
	////{
	////	bSound = true;
	////}


	//if (pFlashLight)
	//{
	//	vec3_t f, r, u, viewangles;

	//	interfaces::engine->get_view_angles(viewangles);
	//	math::angle_vectors(viewangles, &f, &r, &u);

	//	pFlashLight->m_bIsOn = true;
	//	pFlashLight->m_bCastsShadows = false;
	//	pFlashLight->m_flFov = C::Get<float>(Vars.flashlight_fov);
	//	UpdateFlashlight(pFlashLight, g::pLocalPlayer->EyePos(), f, r, u);
	//}
}


void visuals::GatherMovementData()
{
	if (!interfaces::engine->is_in_game() || !g::pLocalPlayer || !config.velocitygraph)
	{
		if (!vecMovementData.empty())
			vecMovementData.clear();
		return;
	}

	vecMovementData.resize(200);

	vecMovementData.insert(
		vecMovementData.begin(),
		MovementData_t{
			std::round(g::pLocalPlayer->Velocity().length_2d()),
			(bool)(g::pLocalPlayer->Flags() & FL_ONGROUND)
		}
	);
}

void visuals::PaintMovementData()
{
	if (!interfaces::engine->is_in_game() || !g::pLocalPlayer)
		return;

	static float flSpeed, flOldGroundSpeed, flLastGroundSpeed, flFrameTime;
	static int iLastFlags;

	const float flVelocity = std::round(g::pLocalPlayer->Velocity().length_2d());
	const int iFlags = g::pLocalPlayer->Flags();

	const int iIdealY = g::vScreenSize.y / 2 + 315;

	if (config.velocitygraph && g::pLocalPlayer->IsAlive() && vecMovementData.size() > 2)
	{
		for (int i = 0; i < vecMovementData.size() - 1; i++)
		{
			const MovementData_t currentData = vecMovementData[i];
			const MovementData_t nextData = vecMovementData[i + 1];

			const bool bLanded = !currentData.bOnGround && nextData.bOnGround;

			const int iCurrentSpeed = std::clamp((int)currentData.flvelocity, 0, 450);
			const int iNextSpeed = std::clamp((int)nextData.flvelocity, 0, 450);

			const RECT linePos = {
				g::vScreenSize.x / 2 + ((200 / 2) - 8) - ((signed int)i - 1) * 5 * 0.175f,
				iIdealY - 50 - (iCurrentSpeed / 3),
				g::vScreenSize.x / 2 + ((200 / 2) - 8) - (signed int)i * 5 * 0.175f,
				iIdealY - 50 - (iNextSpeed / 3)
			};

			render::line(linePos.left, linePos.top, linePos.right, linePos.bottom, Color(225, 225, 225));
		}
	}

	if (config.velocity)
	{
		if (g::pLocalPlayer->IsAlive())
		{
			if (iFlags & FL_ONGROUND && !(iLastFlags & FL_ONGROUND))
			{
				flOldGroundSpeed = flLastGroundSpeed;
				flLastGroundSpeed = flVelocity;
				flFrameTime = 1.0f;
			}

			iLastFlags = iFlags;
		}
		else
			flLastGroundSpeed = 0;

		if (flFrameTime > interfaces::globals->frame_time)
			flFrameTime -= interfaces::globals->frame_time;

		if (g::pLocalPlayer->IsAlive())
		{
			const float flDelta = flLastGroundSpeed - flOldGroundSpeed;

			Color col{ 255, 251, 0, 235 };
			if (flDelta > 0 && flFrameTime > 0.5)
				col = { 30, 250, 30, 235 };
			else if (flDelta < 0 && flFrameTime > 0.5)
				col = { 250, 30, 30, 235 };


			render::indicator.string(g::vScreenSize.x / 2, iIdealY + 24, { 250, 250, 250, 235 }, std::to_string((int)flVelocity), render::ALIGN_CENTER);
			render::indicator.string(g::vScreenSize.x / 2, iIdealY - 6, col, std::to_string((int)flLastGroundSpeed), render::ALIGN_CENTER);
		}
	}
}

void visuals::NoSmoke() {

	i_material* smoke1{};
	i_material* smoke2{};
	i_material* smoke3{};
	i_material* smoke4{};

	if (!smoke1)
		smoke1 = interfaces::material_system->find_material(XOR("particle/vistasmokev1/vistasmokev1_fire"), XOR("Other textures"));

	if (!smoke2)
		smoke2 = interfaces::material_system->find_material(XOR("particle/vistasmokev1/vistasmokev1_smokegrenade"), XOR("Other textures"));

	if (!smoke3)
		smoke3 = interfaces::material_system->find_material(XOR("particle/vistasmokev1/vistasmokev1_emods"), XOR("Other textures"));

	if (!smoke4)
		smoke4 = interfaces::material_system->find_material(XOR("particle/vistasmokev1/vistasmokev1_emods_impactdust"), XOR("Other textures"));

	if (config.nosmoke) {
		if (!smoke1->get_material_var_flag(material_var_no_draw))
			smoke1->set_material_var_flag(material_var_no_draw, true);

		if (!smoke2->get_material_var_flag(material_var_no_draw))
			smoke2->set_material_var_flag(material_var_no_draw, true);

		if (!smoke3->get_material_var_flag(material_var_no_draw))
			smoke3->set_material_var_flag(material_var_no_draw, true);

		if (!smoke4->get_material_var_flag(material_var_no_draw))
			smoke4->set_material_var_flag(material_var_no_draw, true);

		if (g::SmokeCount.get() != 0)
			g::SmokeCount.set(0);
	}

	else {
		if (smoke1->get_material_var_flag(material_var_no_draw))
			smoke1->set_material_var_flag(material_var_no_draw, false);

		if (smoke2->get_material_var_flag(material_var_no_draw))
			smoke2->set_material_var_flag(material_var_no_draw, false);

		if (smoke3->get_material_var_flag(material_var_no_draw))
			smoke3->set_material_var_flag(material_var_no_draw, false);

		if (smoke4->get_material_var_flag(material_var_no_draw))
			smoke4->set_material_var_flag(material_var_no_draw, false);
	}
}
