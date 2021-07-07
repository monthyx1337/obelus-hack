#include "../features.hpp"

static auto CreateWearable() -> CreateClientClass_t {
	auto client_class = interfaces::client->get_client_classes();

	for (client_class = interfaces::client->get_client_classes();
		client_class; client_class = client_class->m_pNext) {

		if (client_class->m_ClassID == class_ids::ceconwearable) {
			return client_class->m_pCreate;
		}
	}
}

static auto make_glove(int entry, int serial) -> BaseAttributableItem* {
	static auto create_wearable_fn = CreateWearable();
	create_wearable_fn(entry, serial);

	const auto glove = reinterpret_cast<BaseAttributableItem*>(interfaces::entity_list->get_client_entity(entry));
	assert(glove); {
		static auto set_abs_origin_addr = pattern::Scan(XOR("client.dll"), XOR("55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		const auto set_abs_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const std::array<float, 3>&)>(set_abs_origin_addr);
		static constexpr std::array<float, 3> new_pos = { 10000.f, 10000.f, 10000.f };
		set_abs_origin_fn(glove, new_pos);
	}
	return glove;
}

bool glovechanger::ApplyGloveModel(BaseAttributableItem* glove, const char* model) noexcept {
	*reinterpret_cast<int*>(uintptr_t(glove) + 0x64) = -1;
	return true;
}

bool glovechanger::ApplyGloveSkins(BaseAttributableItem* glove, int item_definition_index, int paint_kit, int model_index, int world_model_index, int entity_quality, float fallback_wear) noexcept {
	glove->get_item_definiton_index() = item_definition_index;
	glove->get_fallback_paint_kit() = paint_kit;
	glove->SetModelIndex(model_index);
	glove->get_entity_quality() = entity_quality;
	glove->get_fallback_wear() = fallback_wear;

	return true;
}
int GetGloveSkinByMenu(int gloveIndex, int skinIndex)
{
	if (gloveIndex == 0) // bloudhound
	{
		switch (skinIndex)
		{
		case 0:
			return 10006;
		case 1:
			return 10007;
		case 2:
			return 10008;
		case 3:
			return 10039;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 1) // Sport
	{
		switch (skinIndex)
		{
		case 0:
			return 10038;
		case 1:
			return 10037;
		case 2:
			return 10018;
		case 3:
			return 10019;
		case 4:
			return 10048;
		case 5:
			return 10047;
		case 6:
			return 10045;
		case 7:
			return 10046;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 2) // Driver
	{
		switch (skinIndex)
		{
		case 0:
			return 10013;
		case 1:
			return 10015;
		case 2:
			return 10016;
		case 3:
			return 10040;
		case 4:
			return 10043;
		case 5:
			return 10044;
		case 6:
			return 10041;
		case 7:
			return 10042;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 3) // Wraps
	{
		switch (skinIndex)
		{
		case 0:
			return 10009;
		case 1:
			return 10010;
		case 2:
			return 10021;
		case 3:
			return 10036;
		case 4:
			return 10053;
		case 5:
			return 10054;
		case 6:
			return 10055;
		case 7:
			return 10056;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 4) // Moto
	{
		switch (skinIndex)
		{
		case 0:
			return 10024;
		case 1:
			return 10026;
		case 2:
			return 10027;
		case 3:
			return 10028;
		case 4:
			return 10050;
		case 5:
			return 10051;
		case 6:
			return 10052;
		case 7:
			return 10049;

		default:
			return 0;
		}
	}
	else if (gloveIndex == 5) // Specialist
	{
		switch (skinIndex)
		{
		case 0:
			return 10030;
		case 1:
			return 10033;
		case 2:
			return 10034;
		case 3:
			return 10035;
		case 4:
			return 10061;
		case 5:
			return 10062;
		case 6:
			return 10063;
		case 7:
			return 10064;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 6)
	{
		switch (skinIndex)
		{
		case 0:
			return 10057;
		case 1:
			return 10058;
		case 2:
			return 10059;
		case 3:
			return 10060;
		}
	}
	else if (gloveIndex == 7)
	{
	switch (skinIndex)
	{
	case 0:
		return 10085;
	case 1:
		return 10087;
	case 2:
		return 10088;
	case 3:
		return 10086;
	}
	}
	else
		return 0;
	return 0;
}


void glovechanger::Start() noexcept 
{
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game() && g::pCmd == nullptr)
		return;

	const auto local_index = interfaces::engine->get_local_player();
	const auto local_player = g::pLocalPlayer;
	if (!local_player)
		return;


	if (config.glove_model != 0) {
		auto model_blood = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl";
		auto model_sport = "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl";
		auto model_slick = "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl";
		auto model_leath = "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl";
		auto model_moto = "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl";
		auto model_speci = "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl";
		auto model_hydra = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl";
		auto model_fang = "models/weapons/v_models/arms/anarchist/v_glove_anarchist.mdl";

		auto index_blood = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"));
		auto index_sport = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"));
		auto index_slick = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"));
		auto index_leath = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"));
		auto index_moto = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"));
		auto index_speci = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"));
		auto index_hydra = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl"));
		auto index_fang = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/anarchist/v_glove_anarchist.mdl"));

		//credit to namazso for nskinz

		const auto wearables = local_player->Wearables();
		if (!wearables)
			return;

		static UINT glove_handle = UINT(0);

		auto glove = reinterpret_cast<BaseAttributableItem*>(interfaces::entity_list->get_client_entity_handle(wearables[0]));

		if (!glove) {
			const auto our_glove = reinterpret_cast<BaseAttributableItem*>(interfaces::entity_list->get_client_entity_handle(glove_handle));

			if (our_glove) {
				wearables[0] = glove_handle;
				glove = our_glove;
			}
		}
		if (!local_player->IsAlive()) {
			if (glove) {
				glove->SetDestroyedOnRecreateEntities();
				glove->Release();
			}
			return;
		}
		if (!glove) {
			const auto entry = interfaces::entity_list->get_highest_index() + 1;
			const auto serial = rand() % 0x1000;
			glove = make_glove(entry, serial);
			wearables[0] = entry | serial << 16;
			glove_handle = wearables[0];
		}
		if (glove) {
			//float gloves_wear = 0.0000001f;
			float gloves_wear = config.glove_wear;
			//switch (config_system.item.glove_wear) {
			//case 0:
			//	gloves_wear = 0.0000001f;
			//	break;
			//case 1:
			//	gloves_wear = 0.07f;
			//	break;
			//case 2:
			//	gloves_wear = 0.15f;
			//	break;
			//case 3:
			//	gloves_wear = 0.38f;
			//	break;
			//case 4:
			//	gloves_wear = 0.45f;
			//	break;
			//}

			//apply glove model
			switch (config.glove_model) {
			case 0:
				break;
			case 1:
				ApplyGloveModel(glove, model_blood);
				break;
			case 2:
				ApplyGloveModel(glove, model_sport);
				break;
			case 3:
				ApplyGloveModel(glove, model_slick);
				break;
			case 4:
				ApplyGloveModel(glove, model_leath);
				break;
			case 5:
				ApplyGloveModel(glove, model_moto);
				break;
			case 6:
				ApplyGloveModel(glove, model_speci);
				break;
			case 7:
				ApplyGloveModel(glove, model_hydra);
				break;
			case 8:
				ApplyGloveModel(glove, model_fang);
				break;
			}

			switch (config.glove_model) {
			case 0:
				break;
			case 1:
				ApplyGloveSkins(glove, GLOVE_STUDDED_BLOODHOUND, GetGloveSkinByMenu(0, config.glove_skin_blood), index_blood, 0, 3, gloves_wear);
				break;
			case 2:
				ApplyGloveSkins(glove, GLOVE_SPORTY, GetGloveSkinByMenu(1, config.glove_skin_sport), index_sport, 0, 3, gloves_wear);
				break;
			case 3:
				ApplyGloveSkins(glove, GLOVE_SLICK, GetGloveSkinByMenu(2, config.glove_skin_driver), index_slick, 0, 3, gloves_wear);
				break;
			case 4:
				ApplyGloveSkins(glove, GLOVE_LEATHER_WRAP, GetGloveSkinByMenu(3, config.glove_skin_handwar), index_leath, 0, 3, gloves_wear);
				break;
			case 5:
				ApplyGloveSkins(glove, GLOVE_MOTORCYCLE, GetGloveSkinByMenu(4, config.glove_skin_motor), index_moto, 0, 3, gloves_wear);
				break;
			case 6:
				ApplyGloveSkins(glove, GLOVE_SPECIALIST, GetGloveSkinByMenu(5, config.glove_skin_specialist), index_speci, 0, 3, gloves_wear);
				break;
			case 7:
				ApplyGloveSkins(glove, GLOVE_HYDRA, GetGloveSkinByMenu(6, config.glove_skin_hydra), index_hydra, 0, 3, gloves_wear);
				break;
			case 8:
				ApplyGloveSkins(glove, GLOVE_STUDDED_BROKENFANG, GetGloveSkinByMenu(7, config.glove_skin_fang), index_fang, 0, 3, gloves_wear);
				break;
			}

			glove->get_item_id_high() = -1;
			glove->get_fallback_seed() = 0;
			glove->get_fallback_stat_trak() = -1;
			glove->PreDataUpdate(DATA_UPDATE_CREATED);
		}
	}
}