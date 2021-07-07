
static auto get_wearable_create_fn() -> CreateClientClass_t {
	auto clazz = interfaces::client->get_client_classes();

	while (strcmp(clazz->m_pNetworkName, "CEconWearable"))
		clazz = clazz->m_pNext;
	return clazz->m_pCreate;
}
static std::uintptr_t findPattern(const wchar_t* module, const char* pattern, size_t offset = 0) noexcept
{
	static auto id = 0;
	++id;

	if (MODULEINFO moduleInfo; GetModuleInformation(GetCurrentProcess(), GetModuleHandleW(module), &moduleInfo, sizeof(moduleInfo))) {
		auto start = static_cast<const char*>(moduleInfo.lpBaseOfDll);
		const auto end = start + moduleInfo.SizeOfImage;

		auto first = start;
		auto second = pattern;

		while (first < end && *second) {
			if (*first == *second || *second == '?') {
				++first;
				++second;
			}
			else {
				first = ++start;
				second = pattern;
			}
		}

		if (!*second)
			return reinterpret_cast<std::uintptr_t>(const_cast<char*>(start) + offset);
	}
	MessageBoxA(NULL, (" pattern #" + std::to_string(id) + '!').c_str(), " obelus", MB_OK | MB_ICONWARNING);
	return 0;
}

struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};
template<class T>
static T* find_hud_element(const char* name) {
	static auto fn = *reinterpret_cast<DWORD**>(pattern::Scan("client.dll", ("B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08")) + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(pattern::Scan("client.dll", ("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28")));
	return (T*)find_hud_element(fn, name);
}



struct weapon_name_t {
	constexpr weapon_name_t(int32_t definition_index, const char* name) :
		definition_index(definition_index),
		name(name) {
	}

	int32_t definition_index = 0;
	const char* name = nullptr;
};
struct weaponinfo_t {
	constexpr weaponinfo_t(const char* model, const char* icon = nullptr) :
		model(model),
		icon(icon)
	{}

	const char* model;
	const char* icon;
};


class skins : public singleton<skins> {
public:

	const char* index_shit;
	void AnimationHook();
	void AnimationUnHook();
	const std::map<size_t, weaponinfo_t> WeaponInfo =
	{
		{WEAPON_KNIFE,{"models/weapons/v_knife_default_ct.mdl", "models/weapons/w_knife_default_ct.mdl"}},
		{WEAPON_KNIFE_T,{"models/weapons/v_knife_default_t.mdl", "models/weapons/w_knife_default_t.mdl"}},
		{WEAPON_BAYONET, {"models/weapons/v_knife_bayonet.mdl", "models/weapons/w_knife_bayonet.mdl"}},
		{WEAPON_KNIFE_FLIP, {"models/weapons/v_knife_flip.mdl", "models/weapons/w_knife_flip.mdl"}},
		{WEAPON_KNIFE_GUT, {"models/weapons/v_knife_gut.mdl", "models/weapons/w_knife_gut.mdl"}},
		{WEAPON_KNIFE_KARAMBIT, {"models/weapons/v_knife_karam.mdl", "models/weapons/w_knife_karam.mdl"}},
		{WEAPON_KNIFE_M9_BAYONET, {"models/weapons/v_knife_m9_bay.mdl", "models/weapons/w_knife_m9_bay.mdl"}},
		{WEAPON_KNIFE_TACTICAL, {"models/weapons/v_knife_tactical.mdl", "models/weapons/w_knife_tactical.mdl"}},
		{WEAPON_KNIFE_FALCHION, {"models/weapons/v_knife_falchion_advanced.mdl", "models/weapons/w_knife_falchion_advanced.mdl"}},
		{WEAPON_KNIFE_SURVIVAL_BOWIE, {"models/weapons/v_knife_survival_bowie.mdl", "models/weapons/w_knife_survival_bowie.mdl"}},
		{WEAPON_KNIFE_BUTTERFLY, {"models/weapons/v_knife_butterfly.mdl", "models/weapons/w_knife_butterfly.mdl"}},
		{WEAPON_KNIFE_PUSH, {"models/weapons/v_knife_push.mdl", "models/weapons/w_knife_push.mdl"}},
		{WEAPON_KNIFE_URSUS,{"models/weapons/v_knife_ursus.mdl", "models/weapons/w_knife_ursus.mdl"}},
		{WEAPON_KNIFE_GYPSY_JACKKNIFE,{"models/weapons/v_knife_gypsy_jackknife.mdl", "models/weapons/w_knife_gypsy_jackknife.mdl"}},
		{WEAPON_KNIFE_STILETTO,{"models/weapons/v_knife_stiletto.mdl", "models/weapons/w_knife_stiletto.mdl"}},
		{WEAPON_KNIFE_WIDOWMAKER,{"models/weapons/v_knife_widowmaker.mdl", "models/weapons/w_knife_widowmaker.mdl"}},
		{WEAPON_KNIFE_CSS,{"models/weapons/v_knife_css.mdl", "models/weapons/w_knife_css.mdl"}},
		{WEAPON_KNIFE_SKELETON,{"models/weapons/v_knife_skeleton.mdl", "models/weapons/w_knife_skeleton.mdl"}},
		{WEAPON_KNIFE_OUTDOOR,{"models/weapons/v_knife_outdoor.mdl", "models/weapons/w_knife_outdoor.mdl"}},
		{WEAPON_KNIFE_CANIS, {"models/weapons/v_knife_canis.mdl", "models/weapons/w_knife_canis.mdl"}},
		{WEAPON_KNIFE_CORD, {"models/weapons/v_knife_cord.mdl", "models/weapons/w_knife_cord.mdl"}},
		{GLOVE_STUDDED_BLOODHOUND,{"models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"}},
		{GLOVE_T_SIDE,{"models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless.mdl"}},
		{GLOVE_CT_SIDE,{"models/weapons/v_models/arms/glove_hardknuckle/v_glove_hardknuckle.mdl"}},
		{GLOVE_SPORTY,{"models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"}},
		{GLOVE_SLICK,{"models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"}},
		{GLOVE_LEATHER_WRAP,{"models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"}},
		{GLOVE_MOTORCYCLE,{"models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"}},
		{GLOVE_SPECIALIST,{"models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"}},
		{GLOVE_HYDRA,{"models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl"}}
	};

	std::vector< weapon_name_t> WeaponNamesFull =
	{
	{WEAPON_KNIFE, "Knife"},
	{GLOVE_T_SIDE, "Glove"},
	{ WEAPON_AK47, "AK-47" },
	{ WEAPON_AUG, "AUG" },
	{ WEAPON_AWP, "AWP" },
	{ WEAPON_CZ75A, "CZ75 Auto" },
	{ WEAPON_DEAGLE, "Desert Eagle" },
	{ WEAPON_ELITE, "Dual Berettas" },
	{ WEAPON_FAMAS, "FAMAS" },
	{ WEAPON_FIVESEVEN, "Five-SeveN" },
	{ WEAPON_G3SG1, "G3SG1" },
	{ WEAPON_GALILAR, "Galil AR" },
	{ WEAPON_GLOCK, "Glock-18" },
	{ WEAPON_M249, "M249" },
	{ WEAPON_M4A1_SILENCER, "M4A1-S" },
	{ WEAPON_M4A1, "M4A4" },
	{ WEAPON_MAC10, "MAC-10" },
	{ WEAPON_MAG7, "MAG-7" },
	{ WEAPON_MP7, "MP7" },
	{ WEAPON_MP5SD, "MP5" },
	{ WEAPON_MP9, "MP9" },
	{ WEAPON_NEGEV, "Negev" },
	{ WEAPON_NOVA, "Nova" },
	{ WEAPON_HKP2000, "P2000" },
	{ WEAPON_P250, "P250" },
	{ WEAPON_P90, "P90" },
	{ WEAPON_BIZON, "PP-Bizon" },
	{ WEAPON_REVOLVER, "R8 Revolver" },
	{ WEAPON_SAWEDOFF, "Sawed-Off" },
	{ WEAPON_SCAR20, "SCAR-20" },
	{ WEAPON_SSG08, "SSG 08" },
	{ WEAPON_SG556, "SG 553" },
	{ WEAPON_TEC9, "Tec-9" },
	{ WEAPON_UMP45, "UMP-45" },
	{ WEAPON_USP_SILENCER, "USP-S" },
	{ WEAPON_XM1014, "XM1014" },
	};
	const char* GetWeaponNameById(int id)
	{
		switch (id)
		{
		case 1:
			return "deagle";
		case 2:
			return "elite";
		case 3:
			return "fiveseven";
		case 4:
			return "glock";
		case 7:
			return "ak47";
		case 8:
			return "aug";
		case 9:
			return "awp";
		case 10:
			return "famas";
		case 11:
			return "g3sg1";
		case 13:
			return "galilar";
		case 14:
			return "m249";
		case 60:
			return "m4a1_silencer";
		case 16:
			return "m4a1";
		case 17:
			return "mac10";
		case 19:
			return "p90";
		case 23:
			return "mp5sd";
		case 24:
			return "ump45";
		case 25:
			return "xm1014";
		case 26:
			return "bizon";
		case 27:
			return "mag7";
		case 28:
			return "negev";
		case 29:
			return "sawedoff";
		case 30:
			return "tec9";
		case 32:
			return "hkp2000";
		case 33:
			return "mp7";
		case 34:
			return "mp9";
		case 35:
			return "nova";
		case 36:
			return "p250";
		case 38:
			return "scar20";
		case 39:
			return "sg556";
		case 40:
			return "ssg08";
		case 61:
			return "usp_silencer";
		case 63:
			return "cz75a";
		case 64:
			return "revolver";
		case 508:
			return "knife_m9_bayonet";
		case 500:
			return "bayonet";
		case 505:
			return "knife_flip";
		case 506:
			return "knife_gut";
		case 507:
			return "knife_karambit";
		case 509:
			return "knife_tactical";
		case 512:
			return "knife_falchion";
		case 514:
			return "knife_survival_bowie";
		case 515:
			return "knife_butterfly";
		case 516:
			return "knife_push";

		case 519:
			return "knife_ursus";
		case 520:
			return "knife_gypsy_jackknife";
		case 522:
			return "knife_stiletto";
		case 523:
			return "knife_widowmaker";

		case WEAPON_KNIFE_CSS:
			return "knife_css";

		case WEAPON_KNIFE_CORD:
			return "knife_cord";

		case WEAPON_KNIFE_CANIS:
			return "knife_canis";

		case WEAPON_KNIFE_OUTDOOR:
			return "knife_outdoor";

		case WEAPON_KNIFE_SKELETON:
			return "knife_skeleton";

		case 5027:
			return "studded_bloodhound_gloves";
		case 5028:
			return "t_gloves";
		case 5029:
			return "ct_gloves";
		case 5030:
			return "sporty_gloves";
		case 5031:
			return "slick_gloves";
		case 5032:
			return "leather_handwraps";
		case 5033:
			return "motorcycle_gloves";
		case 5034:
			return "specialist_gloves";
		case 5035:
			return "studded_hydra_gloves";

		default:
			return "";
		}
	}


	static auto is_knife(const int i) -> bool {
		return (i >= WEAPON_BAYONET && i < GLOVE_STUDDED_BLOODHOUND) || i == WEAPON_KNIFE_T || i == WEAPON_KNIFE;
	}



	template <typename T>
	void PickCurrentWeapon(int* idx, int* vec_idx, std::vector<T> arr) {

		const auto local = g::pLocalPlayer;

		if (!local) return;
		if (!interfaces::engine->is_in_game()) return;
		auto weapon = local->ActiveWeapon();
		if (!weapon) return;
		short wpn_idx = weapon->ItemDefinitionIndex();
		if (is_knife(wpn_idx)) {
			*idx = WeaponNamesFull.at(0).definition_index;
			*vec_idx = 0;
			return;
		}
		auto wpn_it = std::find_if(arr.begin(), arr.end(), [wpn_idx](const T& a) {
			return a.definition_index == wpn_idx;
			});
		if (wpn_it != arr.end()) {
			*idx = wpn_idx;
			*vec_idx = std::abs(std::distance(arr.begin(), wpn_it));
		}
	}
public:
	struct skinInfo {
		int seed = -1;
		int paintkit;
		int rarity;
		std::string tagName;
		std::string     shortname; // shortname
		std::string     name;      // full skin name
	};
public:
	std::unordered_map<std::string, std::set<std::string>> weaponSkins;
	std::unordered_map<std::string, skinInfo> skinMap;
	std::unordered_map<std::string, std::string> skinNames;
private:
	int weapon_index_skins = 7;
	int weapon_vector_index_skins = 0;
private:
	struct item_setting {
		char name[32] = "";
		int definition_vector_index = 0;
		int definition_index = 1;
		int paint_kit_vector_index = 0;
		int paint_kit_index = 0;
		int definition_override_vector_index = 0;
		int definition_override_index = 0;
		int seed = 0;
		bool enable_stat_track = false;
		int stat_trak = 0;
		float wear = 0.0f;
	};
	struct options_t {
		std::map<int, item_setting> Items;
		std::unordered_map<std::string, std::string> IconOverrides;
	};
	options_t options;
private:
	auto get_icon_override(const std::string original) const -> const char* {
		return options.IconOverrides.count(original) ? options.IconOverrides.at(original).data() : nullptr;
	};
private:
	void erase_override_if_exists_by_index(const int definition_index);
	void apply_config_on_attributable_item(BaseAttributableItem* item, const item_setting* config, const unsigned xuid_low);
public:
	void Start(bool frame_end);
public:
	void UpdateSkins();
	void menu();
	void CheckUpdate();
	bool first_time_render;
	void SetupValues();
};