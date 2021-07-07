#include "../features.hpp"

// nskinz i think??? i dont remember at all

bool should_update_hud;
static constexpr void UpdateHud() noexcept
{
	if (should_update_hud)
	{
		utilities::ForceFullUpdate();
		should_update_hud = false;
	}
}
void skins::UpdateSkins()
{
	interfaces::engine->ClientCmd_Unrestricted("record x;stop");
	should_update_hud = true;
}

void skins::Start(bool frame_end) 
{
	const auto local_index = interfaces::engine->get_local_player();
	const auto local = g::pLocalPlayer;
	if (!local)
		return;

	player_info_t player_info;
	if (!interfaces::engine->get_player_info_bool(local_index, &player_info))
		return;

	if (frame_end) 
	{
		if (local)
			UpdateHud();

		glovechanger::Start();
	}
	else 
	{
		auto weapons = local->MyWeapons();
		for (int i = 0; weapons[i].IsValid(); i++) 
		{
			BaseAttributableItem* weapon = (BaseAttributableItem*)interfaces::entity_list->get_client_entity_handle(weapons[i]);
			if (!weapon)
				continue;
			auto& definition_index = weapon->get_item_definiton_index();
			const auto active_conf = &options.Items[is_knife(definition_index) ? WEAPON_KNIFE : definition_index];
			apply_config_on_attributable_item(weapon, active_conf, player_info.xuidlow);
		}

		const auto view_model_handle = local->GetViewModel();
		if (!view_model_handle.IsValid())
			return;

		const auto view_model = reinterpret_cast<BaseViewModel*>(interfaces::entity_list->get_client_entity_handle(view_model_handle));
		if (!view_model)
			return;

		const auto view_model_weapon_handle = view_model->m_hWeapon();
		if (!view_model_weapon_handle.IsValid())
			return;

		const auto view_model_weapon = reinterpret_cast <BaseWeapon*>(interfaces::entity_list->get_client_entity_handle(view_model_weapon_handle));
		if (!view_model_weapon)
			return;

		if (WeaponInfo.count(view_model_weapon->ItemDefinitionIndex())) 
		{
			const auto override_model = WeaponInfo.at(view_model_weapon->ItemDefinitionIndex()).model;
			auto override_model_index = interfaces::model_info->get_model_index(override_model);
			view_model->ViewModelIndex() = override_model_index;
			index_shit = override_model;
			auto world_model_handle = view_model_weapon->WeaponWorldModel();

			if (!world_model_handle.IsValid())
				return;

			const auto world_model = reinterpret_cast<BaseWeaponWorldModel*>(interfaces::entity_list->get_client_entity_handle(world_model_handle));

			if (!world_model)
				return;

			*world_model->ModelIndex() = override_model_index + 1;
		}

	}
}

void skins::erase_override_if_exists_by_index(const int definition_index) 
{
	if (WeaponInfo.count(definition_index)) {
		auto& icon_override_map = options.IconOverrides;
		const auto& original_item = WeaponInfo.at(definition_index);
		if (original_item.icon && icon_override_map.count(original_item.icon))
			icon_override_map.erase(icon_override_map.at(original_item.icon));
	}
}

void skins::apply_config_on_attributable_item(BaseAttributableItem* item, const item_setting* config, const unsigned xuid_low) {

	const auto local = g::pLocalPlayer;

	const auto active = local->ActiveWeapon();

	if (!active)
		return;

	item->get_item_id_high() = -1;
	item->get_account_id() = xuid_low;

	if (config->name[0] != '\0')
		strcpy_s(item->get_custom_name(), config->name);

	if (config->paint_kit_index)
		item->get_fallback_paint_kit() = config->paint_kit_index;

	if (config->seed)
		item->get_fallback_seed() = config->seed;

	if (config->stat_trak + 1 && config->enable_stat_track)
		item->get_fallback_stat_trak() = config->stat_trak;

	if (config->wear > 0.1)
		item->get_fallback_wear() = config->wear;
	else
		item->get_fallback_wear() = 0;

	auto& definition_index = item->get_item_definiton_index();
	auto& icon_override_map = options.IconOverrides;
	if (config->definition_override_index && config->definition_override_index != definition_index && WeaponInfo.count(config->definition_override_index)) 
	{
		const auto old_definition_index = definition_index;
		definition_index = config->definition_override_index;
		const auto& replacement_item = WeaponInfo.at(config->definition_override_index);

		item->GetModelIdx() = interfaces::model_info->get_model_index(replacement_item.model);
		item->SetModelIndex(interfaces::model_info->get_model_index(replacement_item.model));
		item->GetClientNetworkable()->PreDataUpdate(0);

		if (old_definition_index && WeaponInfo.count(old_definition_index)) 
		{
			const auto& original_item = WeaponInfo.at(old_definition_index);
			if (original_item.icon && replacement_item.icon) 
			{
				item->get_entity_quality() = 3;
				icon_override_map[original_item.icon] = replacement_item.icon;
			}
		}
	}
	else 
	{
		erase_override_if_exists_by_index(definition_index);
	}
}

const std::vector<weapon_name_t> KnifeNames =
{
	{0, "default"},
	{WEAPON_BAYONET, "bayonet"},
	{WEAPON_KNIFE_CSS, "classic knife"},
	{WEAPON_KNIFE_CORD, "cord knife"},
	{WEAPON_KNIFE_CANIS, "canis knife"},
	{WEAPON_KNIFE_OUTDOOR, "outdoor knife"},
	{WEAPON_KNIFE_SKELETON, "skeleton knife"},
	{WEAPON_KNIFE_FLIP, "flip knife"},
	{WEAPON_KNIFE_GUT, "gut knife"},
	{WEAPON_KNIFE_KARAMBIT, "karambit"},
	{WEAPON_KNIFE_M9_BAYONET, "m9 bayonet"},
	{WEAPON_KNIFE_TACTICAL, "huntsman knife"},
	{WEAPON_KNIFE_FALCHION, "falchion knife"},
	{WEAPON_KNIFE_SURVIVAL_BOWIE, "bowie knife"},
	{WEAPON_KNIFE_BUTTERFLY, "butterfly knife"},
	{WEAPON_KNIFE_PUSH, "shadow daggers"},
	{WEAPON_KNIFE_URSUS, "ursus knife"},
	{WEAPON_KNIFE_GYPSY_JACKKNIFE, "navaja knife"},
	{WEAPON_KNIFE_STILETTO, "stiletto knife"},
	{WEAPON_KNIFE_WIDOWMAKER, "talon knife"}
};


int a = 0;
void skins::menu() {

	static char search_bar[32];

	if (interfaces::engine->is_in_game() && interfaces::engine->is_connected() && g::pCmd != nullptr)
		if (g::pLocalPlayer->IsAlive())
			PickCurrentWeapon(&weapon_index_skins, &weapon_vector_index_skins, WeaponNamesFull);


	static auto posCalc = [](int num) -> ImVec2 {
		return ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 3 - 31) * num + 20 * num, ui::GetWindowPos().y + 52 + 27);
	};

	static auto posDouble = [](int num) -> ImVec2 {
		if (num == 1)
			return ImVec2(ui::GetWindowPos().x - 4 + 26 + (ui::GetWindowSize().x / 2 - 36) * num + 20 * num, ui::GetWindowPos().y + 52 + 27);
		else
			return ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 2 - 36) * num + 20 * num, ui::GetWindowPos().y + 52 + 27);
	};

	auto child_size = ImVec2(ui::GetWindowSize().x / 3 - 31 + 65 + 18, ui::GetWindowSize().y - 80 - 27);
	auto child_size_d = ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y - 80 - 27);
	auto child_size_xd = ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27);

	auto& selected_entry = options.Items[WeaponNamesFull[weapon_vector_index_skins].definition_index];
	selected_entry.definition_index = weapon_index_skins;
	selected_entry.definition_vector_index = weapon_vector_index_skins;
	bool is_gloves = false;

	ui::SetNextWindowPos(posDouble(0));
	ui::BeginChild("models", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 65 + 31 - 30 - 30 - 41));
	{

		if (selected_entry.definition_index == WEAPON_KNIFE)
		{
			ui::SingleSelect("knife model", &config.knife_model, { "default", "bayonet", "classic", "cord", "canis", "outdoor", "skeleton", "flip", "gut", "karambit", "m9 bayonet", "huntsman", "falchion", "bowie", "butterfly", "daggers", "ursus", "navaja", "stiletto", "talon" });
			selected_entry.definition_override_vector_index = config.knife_model;
			selected_entry.definition_override_index = KnifeNames.at(selected_entry.definition_override_vector_index).definition_index;
		}
		else
		{
			ui::SingleSelect("knife model", &config.knife_model, { "default", "bayonet", "classic", "cord", "canis", "outdoor", "skeleton", "flip", "gut", "karambit", "m9 bayonet", "huntsman", "falchion", "bowie", "butterfly", "daggers", "ursus", "navaja", "stiletto", "talon" });
		}

		ui::SingleSelect("glove model", &config.glove_model, { "default","bloodhound","sport","driver","hand wraps","motorcycle","specialist","hydra", "broken fang" });

		std::vector<const char*> glove_skinz_blood = { "charred",
			"snakebite",
			"bronzed",
			"guerilla" };

		std::vector<const char*> glove_skinz_sport = {
			"hedge maze",
			"pandoras box",
			"superconductor",
			"arid",
			"vice",
			"omega",
			"amphibious",
			"bronze morph"
		};
		std::vector<const char*> glove_skinz_driver = {
		"lunar weave",
		"convoy",
		"crimson weave",
		"diamondback",
		"overtake",
		"racing green",
		"king snake",
		"imperial plaid"
		};
		std::vector<const char*> glove_skinz_handwar = {
		"leather",
		"spruce ddpat",
		"slaughter",
		"badlands",
		"cobalt skulls",
		"overprint",
		"duct tape",
		"arboreal"
		};
		std::vector<const char*> glove_skinz_motor = {
		"eclipse",
		"spearmint",
		"boom",
		"cool mint",
		"turtle",
		"transport",
		"polygon",
		"pow"
		};
		std::vector<const char*> glove_skinz_specialist = {
		"forest ddpat",
		"crimson kimono",
		"emerald web",
		"foundation",
		"crimson web",
		"buckshot",
		"fade",
		"mogul"
		};
		std::vector<const char*> glove_skinz_hydra = {
		"emerald",
		"mangrove",
		"rattler",
		"case hardened"
		};
		std::vector<const char*> glove_skinz_fang = {
"jade",
"needle poin",
"unhinged",
"yellow-banded"
		};
		switch (config.glove_model)
		{
		case 1:
			ui::SingleSelect("glove skin", &config.glove_skin_blood, { glove_skinz_blood });
			break;
		case 2:
			ui::SingleSelect("glove skin", &config.glove_skin_sport, { glove_skinz_sport });
			break;
		case 3:
			ui::SingleSelect("glove skin", &config.glove_skin_driver, { glove_skinz_driver });
			break;
		case 4:
			ui::SingleSelect("glove skin", &config.glove_skin_handwar, { glove_skinz_handwar });
			break;
		case 5:
			ui::SingleSelect("glove skin", &config.glove_skin_motor, { glove_skinz_motor });
			break;
		case 6:
			ui::SingleSelect("glove skin", &config.glove_skin_specialist, { glove_skinz_specialist });
			break;
		case 7:
			ui::SingleSelect("glove skin", &config.glove_skin_hydra, { glove_skinz_hydra });
			break;
		case 8:
			ui::SingleSelect("glove skin", &config.glove_skin_fang, { glove_skinz_fang });
			break;
		}

		if (ui::SliderFloat("glove wear", &config.glove_wear, 0.f, 1.f, "%.1f"))
			config.glove_wear = std::clamp(config.glove_wear, 0.f, 1.f);


	};
	ui::EndChild();

	ui::SetNextWindowPos(posDouble(1));
	ui::BeginChild("skins", child_size_d); {
		static bool showAllWeapons = false;
		ui::Checkbox("stattrak", &selected_entry.enable_stat_track);
		ui::SliderInt("##stattrak", &selected_entry.stat_trak, 0, 1000, "%d");

		ui::SliderInt("weapon seed", &selected_entry.seed, 1, 5555, "%.1f");

		if (ui::SliderFloat("weapon wear", &selected_entry.wear, 0.f, 1.f, "%.1f"))
			selected_entry.wear = std::clamp(selected_entry.wear, 0.f, 1.f);
		ui::Checkbox("filter by weapon", &showAllWeapons);

		ui::InputText("##search_bar_skins", search_bar, sizeof(search_bar));

		ui::ListBoxHeader("##paintkits", ImVec2(-1 - 12, -1 - 30));

		if (ui::Selectable("Default", selected_entry.paint_kit_index == 0)) {
			selected_entry.paint_kit_index = 0;
			//selected_entry.paint_kit_vector_index = 0;
		}

		static auto GetColRar = [&](int rar) -> ImVec4 {
			switch (rar)
			{
			case 1:
				return ImColor(150, 150, 150, 255);
				break;
			case 2:
				return ImColor(100, 100, 255, 255);
				break;
			case 3:
				return ImColor(50, 50, 255, 255);
				break;
			case 4:
				return ImColor(255, 64, 242, 255);
				break;
			case 5:
				return ImColor(255, 50, 50, 255);
				break;
			case 6:
				return ImColor(255, 50, 50, 255);
				break;
			case 7:
				return ImColor(255, 196, 46, 255);
				break;
			default:
				return ImColor(150, 150, 150, 255);
				break;
			}
		};

		//int defindex = (selected_entry.definition_index == WEAPON_KNIFE || selected_entry.definition_index == WEAPON_KNIFE_T) ? definition_override_index : ((selected_entry.definition_index == GLOVE_T_SIDE || selected_entry.definition_index == GLOVE_CT_SIDE) ? GetGloveDefinitionIndex(selected_entry.definition_override_vector_index) : selected_entry.definition_index);
		bool is_knife = selected_entry.definition_index == WEAPON_KNIFE || selected_entry.definition_index == WEAPON_KNIFE_T;
		int defindex = (is_knife || selected_entry.definition_index == GLOVE_T_SIDE || selected_entry.definition_index == GLOVE_CT_SIDE) ? selected_entry.definition_override_index : selected_entry.definition_index;
		std::string skinName = GetWeaponNameById(defindex);
		if (skinName.compare("") != 0 || !showAllWeapons)
		{

			std::string filter = std::string(search_bar);

			int curItem = -1;
			int s = 0;

			auto set = weaponSkins[skinName];
			std::vector<std::string> list(set.size());
			std::vector<std::string> anti_repeat;



			std::copy(set.begin(), set.end(), list.begin());

			if (!showAllWeapons) {
				list = {};
				for (auto [key, value] : weaponSkins) {
					for (auto skin : value) {
						list.push_back(skin);
					}
				}
			}

			auto fnAntiRepeat = [&](std::string tx) -> bool {
				auto ret = std::find(anti_repeat.begin(), anti_repeat.end(), tx) == anti_repeat.end();

				if (ret)
					anti_repeat.push_back(tx);
				return ret;
			};

			for (auto skin : list) {
				int pk = skinMap[skin].paintkit;
				if (pk == selected_entry.paint_kit_index)
					curItem = s;

				bool passed_filter = true;

				if (!filter.empty()) {
					std::string tempName = skinNames[skinMap[skin].tagName];
					std::string tempQuery = filter;
					std::transform(tempName.begin(), tempName.end(), tempName.begin(), ::tolower);
					std::transform(tempQuery.begin(), tempQuery.end(), tempQuery.begin(), ::tolower);
					if (tempName.find(tempQuery) == std::string::npos)
						passed_filter = false;
				}

				if (passed_filter && fnAntiRepeat(skinNames[skinMap[skin].tagName])) {
					ui::PushStyleColor(ImGuiCol_Text, is_knife ? GetColRar(6) : GetColRar(skinMap[skin].rarity));
					if (ui::Selectable((skinNames[skinMap[skin].tagName]).c_str(), pk == selected_entry.paint_kit_index)) {
						selected_entry.paint_kit_index = pk;
					}
					ui::PopStyleColor();
				}

				s++;
			}
		}

		ui::ListBoxFooter();
	}
	ui::EndChild();

	skins::get().first_time_render = false;
}


void skins::SetupValues() {
	for (auto& [key, val] : WeaponNamesFull) {
		auto& option = options.Items[key];
		config.pushitem(&option.definition_vector_index, val, "d_vec_index", option.definition_vector_index);
		config.pushitem(&option.definition_index, val, "d_index", option.definition_index);
		config.pushitem(&option.paint_kit_vector_index, val, "pk_vec_index", option.paint_kit_vector_index);
		config.pushitem(&option.paint_kit_index, val, "pk_index", option.paint_kit_index);
		config.pushitem(&option.definition_override_index, val, "do_index", option.definition_override_index);
		config.pushitem(&option.definition_override_vector_index, val, "do_vec_index", option.definition_override_vector_index);
		config.pushitem(&option.seed, val, "seed", option.seed);
		config.pushitem(&option.enable_stat_track, val, "stattrack", option.enable_stat_track);
		config.pushitem(&option.stat_trak, val, "stat_trak_val", option.stat_trak);
		config.pushitem(&option.wear, val, "wear", option.wear);
	}
	config.pushitem(&config.knife_model, "skins", "knife_model", config.knife_model);
	config.pushitem(&config.glove_model, "skins", "glove_model", config.glove_model);
	config.pushitem(&config.glove_wear, "skins", "glove_wear", config.glove_wear);
	config.pushitem(&config.glove_skin_blood, "skins", "glove_skin_blood", config.glove_skin_blood);
	config.pushitem(&config.glove_skin_sport, "skins", "glove_skin_sport", config.glove_skin_sport);
	config.pushitem(&config.glove_skin_driver, "skins", "glove_skin_driver", config.glove_skin_driver);
	config.pushitem(&config.glove_skin_handwar, "skins", "glove_skin_handwar", config.glove_skin_handwar);
	config.pushitem(&config.glove_skin_motor, "skins", "glove_skin_motor", config.glove_skin_motor);
	config.pushitem(&config.glove_skin_specialist, "skins", "glove_skin_specialist", config.glove_skin_specialist);
	config.pushitem(&config.glove_skin_hydra, "skins", "glove_skin_hydra", config.glove_skin_hydra);
	config.pushitem(&config.glove_skin_fang, "skins", "glove_skin_fang", config.glove_skin_fang);
	config.pushitem(&config.custom_viewmodel, "misc", "custom_viewmodel", config.custom_viewmodel);
	config.pushitem(&config.viewmodel_offset_x, "misc", "viewmodel_offset_x", config.viewmodel_offset_x);
	config.pushitem(&config.viewmodel_offset_y, "misc", "viewmodel_offset_y", config.viewmodel_offset_y);
	config.pushitem(&config.viewmodel_offset_z, "misc", "viewmodel_offset_z", config.viewmodel_offset_z);
}

void skins::CheckUpdate()
{

	static int stored_knife = 0;
	static int stored_glovemodel_shit = 0;
	static int stored_glove_blood = 0;
	static int stored_glove_sport = 0;
	static int stored_glove_driver = 0;
	static int stored_glove_handwar = 0;
	static int stored_glove_motor = 0;
	static int stored_glove_specialist = 0;
	static int stored_glove_hydra = 0;
	static int stored_glove_fang = 0;
	if (stored_glove_hydra != config.glove_skin_hydra || stored_glove_fang != config.glove_skin_fang || stored_glove_specialist != config.glove_skin_specialist || stored_glove_motor != config.glove_skin_motor || stored_glove_handwar != config.glove_skin_handwar || stored_glove_driver != config.glove_skin_driver || stored_glove_sport != config.glove_skin_sport || stored_glove_blood != config.glove_skin_blood || stored_glovemodel_shit != config.glove_model || stored_knife != config.knife_model)
	{
		if (interfaces::engine->is_in_game() && interfaces::engine->is_connected() && g::pLocalPlayer && g::pLocalPlayer->IsAlive() && g::pLocalPlayer->ActiveWeapon())
		{
			UpdateSkins();
		}
		stored_glovemodel_shit = config.glove_model;
		stored_glove_blood = config.glove_skin_blood;
		stored_glove_sport = config.glove_skin_sport;
		stored_glove_driver = config.glove_skin_driver;
		stored_glove_handwar = config.glove_skin_handwar;
		stored_glove_motor = config.glove_skin_motor;
		stored_glove_specialist = config.glove_skin_specialist;
		stored_glove_hydra = config.glove_skin_hydra;
		stored_glove_fang = config.glove_skin_fang;
		stored_knife = config.knife_model;
	}



	auto& selected_entry = options.Items[WeaponNamesFull[weapon_vector_index_skins].definition_index];

	static int stored_paint_kint = 0;
	static bool stored_statrak_bool = false;
	static int stored_statrak = 0;
	static int stored_seed = 0;
	static float stored_wear = 0.0;
	static float stored_glove_wear = 0.0;



	if (stored_paint_kint != selected_entry.paint_kit_index || stored_seed != selected_entry.seed || stored_wear != selected_entry.wear || stored_glove_wear != config.glove_wear || stored_statrak != selected_entry.stat_trak || stored_statrak_bool != selected_entry.enable_stat_track)
	{
		if (interfaces::engine->is_in_game() && interfaces::engine->is_connected() && g::pLocalPlayer && g::pLocalPlayer->IsAlive() && g::pLocalPlayer->ActiveWeapon() && g_menu.m_bIsOpened)
		{
			UpdateSkins();
		}
		stored_paint_kint = selected_entry.paint_kit_index;
		stored_seed = selected_entry.seed;
		stored_statrak_bool = selected_entry.enable_stat_track;
		stored_statrak = selected_entry.stat_trak;
		stored_wear = selected_entry.wear;
		stored_wear = selected_entry.wear;
		stored_glove_wear = config.glove_wear;
	}

}



#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);

RecvVarProxy_t fnSequenceProxyFn = nullptr;
RecvVarProxy_t oRecvnModelIndex;

void Hooked_RecvProxy_Viewmodel(CRecvProxyData* pData, void* pStruct, void* pOut)
{
	static int default_t = interfaces::model_info->get_model_index("models/weapons/v_knife_default_t.mdl");
	static int default_ct = interfaces::model_info->get_model_index("models/weapons/v_knife_default_ct.mdl");
	static int iBayonet = interfaces::model_info->get_model_index("models/weapons/v_knife_bayonet.mdl");
	static int iButterfly = interfaces::model_info->get_model_index("models/weapons/v_knife_butterfly.mdl");
	static int iFlip = interfaces::model_info->get_model_index("models/weapons/v_knife_flip.mdl");
	static int iGut = interfaces::model_info->get_model_index("models/weapons/v_knife_gut.mdl");
	static int iKarambit = interfaces::model_info->get_model_index("models/weapons/v_knife_karam.mdl");
	static int iM9Bayonet = interfaces::model_info->get_model_index("models/weapons/v_knife_m9_bay.mdl");
	static int iHuntsman = interfaces::model_info->get_model_index("models/weapons/v_knife_tactical.mdl");
	static int iFalchion = interfaces::model_info->get_model_index("models/weapons/v_knife_falchion_advanced.mdl");
	static int iDagger = interfaces::model_info->get_model_index("models/weapons/v_knife_push.mdl");
	static int iBowie = interfaces::model_info->get_model_index("models/weapons/v_knife_survival_bowie.mdl");
	static int iUrsus = interfaces::model_info->get_model_index("models/weapons/v_knife_ursus.mdl");
	static int iNavaja = interfaces::model_info->get_model_index("models/weapons/v_knife_gypsy_jackknife.mdl");
	static int iStiletto = interfaces::model_info->get_model_index("models/weapons/v_knife_stiletto.mdl");
	static int iTalon = interfaces::model_info->get_model_index("models/weapons/v_knife_widowmaker.mdl");
	static int iNomad = interfaces::model_info->get_model_index("models/weapons/v_knife_outdoor.mdl");
	static int iSkeleton = interfaces::model_info->get_model_index("models/weapons/v_knife_skeleton.mdl");
	static int iSurvival = interfaces::model_info->get_model_index("models/weapons/v_knife_canis.mdl");
	static int iParacord = interfaces::model_info->get_model_index("models/weapons/v_knife_cord.mdl");
	static int iClassic = interfaces::model_info->get_model_index("models/weapons/v_knife_css.mdl");

	if (g::pLocalPlayer && config.knife_model > 0)
	{
		if (g::pLocalPlayer->IsAlive() && (pData->m_Value.m_Int == default_ct
			|| pData->m_Value.m_Int == default_t
			|| pData->m_Value.m_Int == iGut
			|| pData->m_Value.m_Int == iKarambit
			|| pData->m_Value.m_Int == iM9Bayonet
			|| pData->m_Value.m_Int == iHuntsman
			|| pData->m_Value.m_Int == iFalchion
			|| pData->m_Value.m_Int == iDagger
			|| pData->m_Value.m_Int == iBowie
			|| pData->m_Value.m_Int == iButterfly
			|| pData->m_Value.m_Int == iFlip
			|| pData->m_Value.m_Int == iBayonet
			|| pData->m_Value.m_Int == iUrsus
			|| pData->m_Value.m_Int == iNavaja
			|| pData->m_Value.m_Int == iStiletto
			|| pData->m_Value.m_Int == iTalon
			|| pData->m_Value.m_Int == iNomad
			|| pData->m_Value.m_Int == iSkeleton
			|| pData->m_Value.m_Int == iSurvival
			|| pData->m_Value.m_Int == iParacord
			|| pData->m_Value.m_Int == iClassic))
		{
			pData->m_Value.m_Int = interfaces::model_info->get_model_index(skins::get().index_shit);


		}
	}


	oRecvnModelIndex(pData, pStruct, pOut);
}


void SetViewModelSequence(const CRecvProxyData* pDataConst, void* pStruct, void* pOut) {
	// Make the incoming data editable.
	CRecvProxyData* pData = const_cast<CRecvProxyData*>(pDataConst);

	// Confirm that we are replacing our view model and not someone elses.
	BaseViewModel* pViewModel = (BaseViewModel*)pStruct;

	if (pViewModel) {
		auto pOwner = reinterpret_cast<BaseEntity*>(interfaces::entity_list->get_client_entity_handle(uintptr_t(pViewModel->Owner())));

		// Compare the owner entity of this view model to the local player entity.
		if (pOwner && pOwner->Index() == interfaces::engine->get_local_player()) {
			// Get the filename of the current view model.
			const void* pModel = interfaces::model_info->get_model(pViewModel->ViewModelIndex());

			const char* szModel = interfaces::model_info->get_model_name((model_t*)pModel);

			// Store the current sequence.
			int m_nSequence = pData->m_Value.m_Int;


			if (!strcmp(szModel, "models/weapons/v_knife_butterfly.mdl")) {
				// Fix animations for the Butterfly Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03); break;
				default:
					m_nSequence++;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_falchion_advanced.mdl")) {
				// Fix animations for the Falchion Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02); break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_push.mdl")) {
				// Fix animations for the Shadow Daggers.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5); break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1); break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_survival_bowie.mdl")) {
				// Fix animations for the Bowie Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1; break;
				default:
					m_nSequence--;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_ursus.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_stiletto.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(12, 13);
					break;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_widowmaker.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(14, 15);
					break;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_css.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = 15;
					break;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_cord.mdl") ||
				!strcmp(szModel, "models/weapons/v_knife_canis.mdl") ||
				!strcmp(szModel, "models/weapons/v_knife_outdoor.mdl") ||
				!strcmp(szModel, "models/weapons/v_knife_skeleton.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
					break;
				default:
					m_nSequence++;
				}
			}
			// Set the fixed sequence.
			pData->m_Value.m_Int = m_nSequence;
		}
	}

	// Call original function with the modified data.
	fnSequenceProxyFn(pData, pStruct, pOut);
}


void skins::AnimationHook()
{
	ClientClass* pClass = interfaces::client->get_client_classes();
	while (pClass)
	{
		const char* pszName = pClass->m_pRecvTable->m_pNetTableName;

		if (!strcmp(pszName, XOR("DT_BaseViewModel"))) {
			// Search for the 'm_nModelIndex' property.
			RecvTable* pClassTable = pClass->m_pRecvTable;

			for (int nIndex = 0; nIndex < pClass->m_pRecvTable->m_nProps; nIndex++) {
				RecvProp* pProp = &(pClass->m_pRecvTable->m_pProps[nIndex]);

				if (!pProp || strcmp(pProp->m_pVarName, XOR("m_nSequence")))
					continue;

				// Store the original proxy function.
				fnSequenceProxyFn = (RecvVarProxy_t)pProp->m_ProxyFn;

				// Replace the proxy function with our sequence changer.
				pProp->m_ProxyFn = (RecvVarProxy_t)SetViewModelSequence;
			}
		}

		if (!strcmp(pszName, XOR("DT_BaseViewModel")))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp* pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char* name = pProp->m_pVarName;

				// Knives
				if (!strcmp(name, XOR("m_nModelIndex")))
				{
					oRecvnModelIndex = (RecvVarProxy_t)pProp->m_ProxyFn;
					pProp->m_ProxyFn = (RecvVarProxy_t)Hooked_RecvProxy_Viewmodel;
				}
			}
		}
		pClass = pClass->m_pNext;
	}
}

void skins::AnimationUnHook()
{
	for (ClientClass* pClass = interfaces::client->get_client_classes(); pClass; pClass = pClass->m_pNext) {
		if (!strcmp(pClass->m_pNetworkName, XOR("CBaseViewModel"))) {
			// Search for the 'm_nModelIndex' property.
			RecvTable* pClassTable = pClass->m_pRecvTable;

			for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
				RecvProp* pProp = &pClassTable->m_pProps[nIndex];

				if (!pProp || strcmp(pProp->m_pVarName, XOR("m_nSequence")))
					continue;

				// Replace the proxy function with our sequence changer.
				pProp->m_ProxyFn = fnSequenceProxyFn;

				break;
			}

			break;
		}
	}

	for (ClientClass* pClass = interfaces::client->get_client_classes(); pClass; pClass = pClass->m_pNext) {
		if (!strcmp(pClass->m_pNetworkName, XOR("CBaseViewModel"))) {
			// Search for the 'm_nModelIndex' property.
			RecvTable* pClassTable = pClass->m_pRecvTable;

			for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
				RecvProp* pProp = &pClassTable->m_pProps[nIndex];

				if (!pProp || strcmp(pProp->m_pVarName, XOR("m_nModelIndex")))
					continue;

				// Replace the proxy function with our sequence changer.
				pProp->m_ProxyFn = oRecvnModelIndex;

				break;
			}

			break;
		}
	}
}