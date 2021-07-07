#include "../features.hpp"

void chams::AddEffects(int basemat, i_material* pMaterial, int pearlescent, bool phong, bool reflectivity, float reflcol[4], float phongcol[4], i_material* pOverlayMaterial, float overlaycol[4])
{
	if (basemat == 0)
	{
		const float flPearlescent = pearlescent;
		i_material_var* pPearl = pMaterial->find_var(("$pearlescent"), nullptr);
		pPearl->SetVector(flPearlescent / 10.0f, flPearlescent / 10.0f, flPearlescent / 10.0f);

		if (phong)
		{
			const auto phong_col = g_menu.ConvertColor(phongcol);

			i_material_var* phongboost = pMaterial->find_var(("$phongboost"), nullptr);
			phongboost->SetFloat((phong_col.a / 255.0f) * 10.0f);

			i_material_var* phong_tint = pMaterial->find_var(("$phongtint"), nullptr);
			phong_tint->SetVector(phong_col.r / 255.0f, phong_col.g / 255.0f, phong_col.b / 255.0f);
		}
		else
		{
			i_material_var* phongboost = pMaterial->find_var(("$phongboost"), nullptr);
			phongboost->SetFloat(0.0f);
		}

		if (reflectivity)
		{
			const auto reflectivity_col = g_menu.ConvertColor(reflcol);
			i_material_var* reflectivity = pMaterial->find_var(("$envmaptint"), nullptr);
			reflectivity->SetVector((reflectivity_col.r / 255.0f), (reflectivity_col.g / 255.0f), (reflectivity_col.b / 255.0f));
		}
		else
		{
			i_material_var* reflectivity = pMaterial->find_var(("$envmaptint"), nullptr);
			reflectivity->SetVector(0, 0, 0);
		}
	}

	if (pOverlayMaterial != nullptr)
	{
		const auto overlay_col = g_menu.ConvertColor(overlaycol);
		i_material_var* overlay = pOverlayMaterial->find_var(("$envmaptint"), nullptr);
		overlay->SetVector((overlay_col.r / 255.0f), (overlay_col.g / 255.0f), (overlay_col.b / 255.0f));
	}
}

bool chams::Chams(DrawModelResults_t* pResults, const DrawModelInfo_t& info, matrix_t* pBoneToWorld, float* flFlexWeights, float* flFlexDelayedWeights, const vec3_t& vecModelOrigin, int nFlags)
{
	auto* pRenderable = (i_client_renderable*)info.pClientEntity;
	if (pRenderable == nullptr)
		return false;

	BaseEntity* pEntity = (BaseEntity*)pRenderable->GetIClientUnknown()->GetBaseEntity();
	if (pEntity == nullptr)
		return false;

	std::string_view szModelName = info.pStudioHdr->name_char_array;
	static bool temp = false;
	if (!temp)
	{
		if (g::pLocalPlayer->IsAlive())
			temp = true;
	}

	// arm models.
	if (szModelName.find((XOR("arms"))) != std::string_view::npos)
	{
		if (chams::Hands(pResults, info, pBoneToWorld, flFlexWeights, flFlexDelayedWeights, vecModelOrigin, nFlags))
			return true;
		else
			return false;
	}

	// player models.
	if (szModelName.find((XOR("player"))) != std::string_view::npos && szModelName.find((XOR("shadow"))) == std::string_view::npos)
	{
		if (chams::Players(pEntity, temp, pResults, info, pBoneToWorld, flFlexWeights, flFlexDelayedWeights, vecModelOrigin, nFlags))
			return true;
		else
			return false;
	}


	return false;
}

bool chams::Chams()
{
	return false;
}

bool chams::Players(BaseEntity* pEntity, bool temp, DrawModelResults_t* pResults, const DrawModelInfo_t& info, matrix_t* pBoneToWorld, float* flFlexWeights, float* flFlexDelayedWeights, const vec3_t& vecModelOrigin, int nFlags)
{
	static auto oDrawModel = detour::draw_model.GetOriginal<decltype(&hooks::hkDrawModel)>();

	if (nFlags & (STUDIO_RENDER | STUDIO_SKIP_FLEXES | STUDIO_DONOTMODIFYSTENCILSTATE | STUDIO_NOLIGHTING_OR_CUBEMAP | STUDIO_SKIP_DECALS))
		return false;

	static i_material* pMaterial;
	static i_material* pOverlayMaterial;
	static i_material* GlowOverlay = CreateOverlay(1);
	static i_material* DevGlowOverlay = CreateOverlay(2);

	static i_material* playerMaterials[1] =
	{
		CreateMaterial((XOR("custom")), (XOR("VertexLitGeneric")), (XOR("vgui/white")), (XOR("env_cubemap"))),
	};

	if (!pEntity->IsAlive() || pEntity->Dormant())
		return false;

	if (g::pLocalPlayer->TeamNum() != pEntity->TeamNum())
	{
		if (config.players.basemat == 0)
			pMaterial = playerMaterials[0];
		else
			pMaterial = interfaces::material_system->find_material("debug/debugdrawflat", "Model textures", true);

		if (config.players.overlaymat == 1)
			pOverlayMaterial = GlowOverlay;
		else
			pOverlayMaterial = DevGlowOverlay;

		if (pMaterial == nullptr || pMaterial->is_error_material() || pOverlayMaterial == nullptr || pOverlayMaterial->is_error_material()
			|| GlowOverlay == nullptr || GlowOverlay->is_error_material() || DevGlowOverlay == nullptr || DevGlowOverlay->is_error_material())
			return false;

		pMaterial->increment_reference_count();
		GlowOverlay->increment_reference_count();
		DevGlowOverlay->increment_reference_count();
		pOverlayMaterial->increment_reference_count();

		Color colVisible = g_menu.ConvertColor(config.players.vis_col);
		Color colHidden = g_menu.ConvertColor(config.players.invis_col);

		if (config.players.visible)
		{
			if (config.players.invisible && temp)
			{
				pMaterial->increment_reference_count();
				pMaterial->set_material_var_flag(material_var_ignorez, true);
				pMaterial->set_material_var_flag(material_var_wireframe, config.players.wireframe ? true : false);
				interfaces::studio_render->set_color_modulation(colHidden.Base().data());
				interfaces::studio_render->set_alpha_modulation(colHidden.aBase());
				interfaces::studio_render->forced_material_override(pMaterial);

				oDrawModel(interfaces::studio_render, 0, pResults, info, pBoneToWorld, flFlexWeights, flFlexDelayedWeights, vecModelOrigin, nFlags);
			}

			if (config.players.overlaymat != 0 && config.players.overlayxqz && temp)
			{
				const auto overlay_col = g_menu.ConvertColor(config.players.overlay_xqz_col);
				i_material_var* overlay = pOverlayMaterial->find_var(("$envmaptint"), nullptr);
				overlay->SetVector((overlay_col.r / 255.0f), (overlay_col.g / 255.0f), (overlay_col.b / 255.0f));
				pOverlayMaterial->set_material_var_flag(material_var_ignorez, true);
				pOverlayMaterial->set_material_var_flag(material_var_wireframe, config.players.overlaywireframe ? true : false);
				interfaces::studio_render->forced_material_override(pOverlayMaterial);
				oDrawModel(interfaces::studio_render, 0, pResults, info, pBoneToWorld, flFlexWeights, flFlexDelayedWeights, vecModelOrigin, nFlags);
			}

			pMaterial->increment_reference_count();
			pMaterial->set_material_var_flag(material_var_ignorez, false);

			interfaces::studio_render->set_color_modulation(colVisible.Base().data());
			interfaces::studio_render->set_alpha_modulation(colVisible.aBase());
			interfaces::studio_render->forced_material_override(pMaterial);

			if (config.players.overlaymat != 0)
			{
				oDrawModel(interfaces::studio_render, 0, pResults, info, pBoneToWorld, flFlexWeights, flFlexDelayedWeights, vecModelOrigin, nFlags);

				pOverlayMaterial->set_material_var_flag(material_var_ignorez, false);
				pOverlayMaterial->set_material_var_flag(material_var_wireframe, config.players.overlaywireframe ? true : false);
				interfaces::studio_render->forced_material_override(pOverlayMaterial);
			}
		}

		chams::AddEffects(config.players.basemat, pMaterial, config.players.pearlescent, config.players.phong, config.players.reflectivity, config.players.reflectivity_col, config.players.phong_col, pOverlayMaterial, config.players.overlay_col);;

		return true;
	}
}

bool chams::Hands(DrawModelResults_t* pResults, const DrawModelInfo_t& info, matrix_t* pBoneToWorld, float* flFlexWeights, float* flFlexDelayedWeights, const vec3_t& vecModelOrigin, int nFlags)
{
	static auto oDrawModel = detour::draw_model.GetOriginal<decltype(&hooks::hkDrawModel)>();
	static i_material* armMaterials[1] =
	{
		CreateMaterial((XOR("custom")), (XOR("VertexLitGeneric")), (XOR("vgui/white")), (XOR("env_cubemap"))),
	};

	static i_material* pMaterial;
	static i_material* pOverlayMaterial;
	static i_material* GlowOverlay = CreateOverlay(1);
	static i_material* DevGlowOverlay = CreateOverlay(2);

	if (config.hands.basemat == 0)
		pMaterial = armMaterials[0];
	else
		pMaterial = interfaces::material_system->find_material("debug/debugdrawflat", "Model textures", true);

	if (config.hands.overlaymat == 1)
		pOverlayMaterial = GlowOverlay;
	else
		pOverlayMaterial = DevGlowOverlay;

	if (pMaterial == nullptr || pMaterial->is_error_material() || pOverlayMaterial == nullptr || pOverlayMaterial->is_error_material()
		|| GlowOverlay == nullptr || GlowOverlay->is_error_material() || DevGlowOverlay == nullptr || DevGlowOverlay->is_error_material())
		return false;

	pMaterial->increment_reference_count();
	GlowOverlay->increment_reference_count();
	DevGlowOverlay->increment_reference_count();
	pOverlayMaterial->increment_reference_count();

	Color col = g_menu.ConvertColor(config.hands.col);

	if (config.hands.enabled)
	{
		pMaterial->set_material_var_flag(material_var_ignorez, false);
		pMaterial->set_material_var_flag(material_var_wireframe, config.hands.wireframe ? true : false);
		interfaces::studio_render->set_color_modulation(col.Base().data());
		interfaces::studio_render->set_alpha_modulation(col.aBase());
		interfaces::studio_render->forced_material_override(pMaterial);

		if (config.hands.overlaymat != 0)
		{
			oDrawModel(interfaces::studio_render, 0, pResults, info, pBoneToWorld, flFlexWeights, flFlexDelayedWeights, vecModelOrigin, nFlags);
			pOverlayMaterial->set_material_var_flag(material_var_ignorez, false);
			pOverlayMaterial->set_material_var_flag(material_var_wireframe, config.hands.overlaywireframe ? true : false);
			interfaces::studio_render->forced_material_override(pOverlayMaterial);
		}
	}

	chams::AddEffects(config.hands.basemat, pMaterial, config.hands.pearlescent, config.hands.phong, config.hands.reflectivity, config.hands.reflectivity_col, config.hands.phong_col, pOverlayMaterial, config.hands.overlay_col);

	return true;
}

i_material* chams::CreateOverlay(int num)
{
	if (num == 1)
	{
		static const std::string material_name = "game_material.vmt";
		std::string shade_type;
		shade_type = "VertexLitGeneric";
		std::string material_data;
		material_data += "\"" + shade_type + "\"\n{\n";
		material_data += "\t\"$additive\" \"1\"\n";
		material_data += "\t\"$envmap\" \"models/effects/cube_white\"\n";
		material_data += "\t\"$envmaptint\" \"[1 1 1]\"\n";
		material_data += "\t\"$envmapfresnel\" \"1\"\n";
		material_data += "\t\"$envmapfresnelminmaxexp\" \"[0 1 2]\"\n";
		material_data += "\t\"$alpha\" \"0.8\"\n";
		material_data += "}\n";

		CKeyValues* pKeyValues = (CKeyValues*)CKeyValues::operator new(sizeof(CKeyValues));
		pKeyValues->Init(shade_type.c_str());
		pKeyValues->LoadFromBuffer(material_name.c_str(), material_data.c_str());

		return interfaces::material_system->CreateMaterial(material_name.c_str(), pKeyValues);

	}
	else if (num == 2)
	{
		return interfaces::material_system->find_material("dev/glow_armsrace", nullptr, true);
	}
}

i_material* chams::CreateMaterial(std::string szName, std::string szShader, std::string szBaseTexture, std::string szEnvMap, bool bIgnorez, bool bWireframe, std::string szProxies)
{
	std::string szMaterial = fmt::format((R"#("{shader}"
	{{
		"$basetexture"	"{texture}"
		"$ignorez"		"{ignorez}"
		"$envmap"		"{envmap}"
		"$wireframe"	"{wireframe}"

		"$model"		"1"
		"$selfillum"	"1"
		"$halflambert"	"1"
		"$nofog"		"1"
		"$nocull"       "0"
		"$znearer"		"0"
		"$flat"			"1" 
		"$normalmapalphaenvmask" "1"

		"$bumpmap"		"effects\flat_normal"

		"$envmaptint"		"[0 0 0]"
		"$envmapfresnel" "0"

		"$phongalbedotint" "1"
		"$phong" "1"
		"$phongexponent" "15"
		"$phongboost" "0"
		"$phongtint"		"[0 0 0]"

		"$rimlight" "0"
		"$rimlightexponent" "15"
		"$rimlightboost" "1"

		"$phongfresnelranges" "[.5 .5 .5]"

		"$pearlescent" "0"

		"proxies"
		{{ 
			{proxies}
		}}
	}})#"), fmt::arg(("shader"), szShader), fmt::arg(("texture"), szBaseTexture), fmt::arg(("envmap"), szEnvMap), fmt::arg(("ignorez"), bIgnorez ? 1 : 0), fmt::arg(("wireframe"), bWireframe ? 1 : 0), fmt::arg(("proxies"), szProxies));

	CKeyValues* pKeyValues = (CKeyValues*)CKeyValues::operator new(sizeof(CKeyValues));
	pKeyValues->Init(szShader.data());
	pKeyValues->LoadFromBuffer(szName.data(), szMaterial.c_str());


	return interfaces::material_system->CreateMaterial(szName.data(), pKeyValues);
}
