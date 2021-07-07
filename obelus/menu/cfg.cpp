#include "cfg.hpp"
#include "../client/json/json.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "menu.h"
#include "../hack/features/features.hpp"

ConfigSystem config;

void ConfigSystem::SetupValues()
{
	//skinchanger::SetupValues();
	skins::get().SetupValues();

	config.pushitem(&config.box, "visual", "box", config.box);
	config.pushitem(&config.name, "visual", "name", config.name);
	config.pushitem(&config.health, "visual", "health", config.health);
	config.pushitem(&config.ammo, "visual", "ammo", config.ammo);
	config.pushitem(&config.weapon, "visual", "weapon", config.weapon);
	config.pushitem(&config.weapon_icon, "visual", "weapon_icon", config.weapon_icon);
	config.pushitem(&config.flags, "visual", "flags", config.flags);
	config.pushitem(&config.playerflags[0], "visual", "flags_0", config.playerflags[0]);
	config.pushitem(&config.playerflags[1], "visual", "flags_1", config.playerflags[1]);
	config.pushitem(&config.playerflags[2], "visual", "flags_2", config.playerflags[2]);
	config.pushitem(&config.playerflags[3], "visual", "flags_3", config.playerflags[3]);
	config.pushitem(&config.playerflags[4], "visual", "flags_4", config.playerflags[4]);
	config.pushitem(&config.playerflags[5], "visual", "flags_5", config.playerflags[5]);
	config.pushitem(&config.glow, "visual", "glow", config.glow);
	config.pushitem(&config.dormant, "visual", "dormant", config.dormant);
	config.pushitem(&config.dormancy, "visual", "dormancy", config.dormancy);
	config.pushitem(&config.nightmode, "visual", "nightmode", config.nightmode);
	config.pushitem(&config.recoilcross, "visual", "recoilcrosshair", config.recoilcross);
	config.pushitem(&config.brightness, "visual", "brightness", config.brightness);

	config.pushitem(&config.dropped, "visual", "dropped", config.dropped);
	config.pushitem(&config.dropped_ammo, "visual", "dropped_ammo", config.dropped_ammo);
	config.pushitem(&config.bomb, "visual", "bomb", config.bomb);
	config.pushitem(&config.nades, "visual", "nades", config.nades);
	config.pushitem(&config.velocity, "visual", "velocity", config.velocity);
	config.pushitem(&config.velocitygraph, "visual", "velocitygraph", config.velocitygraph);
	config.pushitem(&config.nadepred, "visual", "nadepred", config.nadepred);
	config.pushitem(&config.viewmodel_fov, "visual", "viewmodel_fov", config.viewmodel_fov);
	config.pushitem(&config.view_fov, "visual", "view_fov", config.view_fov);
	config.pushitem(&config.viewmodel_fov, "visual", "viewmodel_fov", config.disablepostprocess);
	config.pushitem(&config.noscopezoom, "visual", "noscopezoom", config.noscopezoom);
	config.pushitem(&config.noflash, "visual", "noflash", config.noflash);
	config.pushitem(&config.nosmoke, "visual", "nosmoke", config.nosmoke);
	config.pushitem(&config.radar, "visual", "radar", config.radar);
	config.pushitem(&config.novisrecoil, "visual", "novisrecoil", config.novisrecoil);

	config.pushitem(&config.changemolotov, "visual", "customfire", config.changemolotov);
	config.pushitem(&config.molotov_col[0], "visual", "molotov_col_r", config.molotov_col[0]);
	config.pushitem(&config.molotov_col[1], "visual", "molotov_col_g", config.molotov_col[1]);
	config.pushitem(&config.molotov_col[2], "visual", "molotov_col_b", config.molotov_col[2]);
	config.pushitem(&config.molotov_col[3], "visual", "molotov_col_a", config.molotov_col[3]);

	config.pushitem(&config.droppedweapons[0], "visual", "droppedweapons_0", config.droppedweapons[0]);
	config.pushitem(&config.droppedweapons[1], "visual", "droppedweapons_1", config.droppedweapons[1]);

	config.pushitem(&config.nades_col[0], "visual", "nades_col_r", config.nades_col[0]);
	config.pushitem(&config.nades_col[1], "visual", "nades_col_g", config.nades_col[1]);
	config.pushitem(&config.nades_col[2], "visual", "nades_col_b", config.nades_col[2]);
	config.pushitem(&config.nades_col[3], "visual", "nades_col_a", config.nades_col[3]);
	config.pushitem(&config.nadepred_col[0], "visual", "nadepred_col_r", config.nadepred_col[0]);
	config.pushitem(&config.nadepred_col[1], "visual", "nadepred_col_g", config.nadepred_col[1]);
	config.pushitem(&config.nadepred_col[2], "visual", "nadepred_col_b", config.nadepred_col[2]);
	config.pushitem(&config.nadepred_col[3], "visual", "nadepred_col_a", config.nadepred_col[3]);

	config.pushitem(&config.weapon_col[0], "visual", "weapon_col_r", config.weapon_col[0]);
	config.pushitem(&config.weapon_col[1], "visual", "weapon_col_g", config.weapon_col[1]);
	config.pushitem(&config.weapon_col[2], "visual", "weapon_col_b", config.weapon_col[2]);
	config.pushitem(&config.weapon_col[3], "visual", "weapon_col_a", config.weapon_col[3]);


	config.pushitem(&config.weapon_icon_col[0], "visual", "weapon_icon_col_r", config.weapon_icon_col[0]);
	config.pushitem(&config.weapon_icon_col[1], "visual", "weapon_icon_col_g", config.weapon_icon_col[1]);
	config.pushitem(&config.weapon_icon_col[2], "visual", "weapon_icon_col_b", config.weapon_icon_col[2]);
	config.pushitem(&config.weapon_icon_col[3], "visual", "weapon_icon_col_a", config.weapon_icon_col[3]);

	config.pushitem(&config.dropped_col[0], "visual", "dropped_col_r", config.dropped_col[0]);
	config.pushitem(&config.dropped_col[1], "visual", "dropped_col_g", config.dropped_col[1]);
	config.pushitem(&config.dropped_col[2], "visual", "dropped_col_b", config.dropped_col[2]);
	config.pushitem(&config.dropped_col[3], "visual", "dropped_col_a", config.dropped_col[3]);


	config.pushitem(&config.dropped_ammo_col[0], "visual", "dropped_ammo_col_r", config.dropped_ammo_col[0]);
	config.pushitem(&config.dropped_ammo_col[1], "visual", "dropped_ammo_col_g", config.dropped_ammo_col[1]);
	config.pushitem(&config.dropped_ammo_col[2], "visual", "dropped_ammo_col_b", config.dropped_ammo_col[2]);
	config.pushitem(&config.dropped_ammo_col[3], "visual", "dropped_ammo_col_a", config.dropped_ammo_col[3]);

	config.pushitem(&config.box_col[0], "visual", "box_col_r", config.box_col[0]);
	config.pushitem(&config.box_col[1], "visual", "box_col_g", config.box_col[1]);
	config.pushitem(&config.box_col[2], "visual", "box_col_b", config.box_col[2]);
	config.pushitem(&config.box_col[3], "visual", "box_col_a", config.box_col[3]);

	config.pushitem(&config.name_col[0], "visual", "name_col_r", config.name_col[0]);
	config.pushitem(&config.name_col[1], "visual", "name_col_g", config.name_col[1]);
	config.pushitem(&config.name_col[2], "visual", "name_col_b", config.name_col[2]);
	config.pushitem(&config.name_col[3], "visual", "name_col_a", config.name_col[3]);

	config.pushitem(&config.ammo_col[0], "visual", "ammo_col_r", config.ammo_col[0]);
	config.pushitem(&config.ammo_col[1], "visual", "ammo_col_g", config.ammo_col[1]);
	config.pushitem(&config.ammo_col[2], "visual", "ammo_col_b", config.ammo_col[2]);
	config.pushitem(&config.ammo_col[3], "visual", "ammo_col_a", config.ammo_col[3]);

	config.pushitem(&config.glow_col[0], "visual", "glow_col_r", config.glow_col[0]);
	config.pushitem(&config.glow_col[1], "visual", "glow_col_g", config.glow_col[1]);
	config.pushitem(&config.glow_col[2], "visual", "glow_col_b", config.glow_col[2]);
	config.pushitem(&config.glow_col[3], "visual", "glow_col_a", config.glow_col[3]);


	config.pushitem(&config.players.visible, "players", "players_visible", config.players.visible);
	config.pushitem(&config.players.invisible, "players", "players_invisible", config.players.invisible);
	config.pushitem(&config.players.pearlescent, "players", "players_pearlescent", config.players.pearlescent);
	config.pushitem(&config.players.phong, "players", "players_phong", config.players.phong);
	config.pushitem(&config.players.reflectivity, "players", "players_reflectivity", config.players.reflectivity);
	config.pushitem(&config.players.basemat, "players", "players_basemat", config.players.basemat);

	config.pushitem(&config.players.overlaymat, "players", "players_overlaymat", config.players.overlaymat);
	config.pushitem(&config.players.wireframe, "players", "players_wireframe", config.players.wireframe);
	config.pushitem(&config.players.overlaywireframe, "players", "players_overlaywireframe", config.players.overlaywireframe);
	config.pushitem(&config.players.overlay_col[0], "players", "players_overlay_col_r", config.players.overlay_col[0]);
	config.pushitem(&config.players.overlay_col[1], "players", "players_overlay_col_g", config.players.overlay_col[1]);
	config.pushitem(&config.players.overlay_col[2], "players", "players_overlay_col_b", config.players.overlay_col[2]);
	config.pushitem(&config.players.overlay_col[3], "players", "players_overlay_col_a", config.players.overlay_col[3]);

	config.pushitem(&config.players.overlayxqz, "players", "players_overlayxqz", config.players.overlayxqz);
	config.pushitem(&config.players.overlay_xqz_col[0], "players", "players_overlay_xqz_col_r", config.players.overlay_xqz_col[0]);
	config.pushitem(&config.players.overlay_xqz_col[1], "players", "players_overlay_xqz_col_g", config.players.overlay_xqz_col[1]);
	config.pushitem(&config.players.overlay_xqz_col[2], "players", "players_overlay_xqz_col_b", config.players.overlay_xqz_col[2]);
	config.pushitem(&config.players.overlay_xqz_col[3], "players", "players_overlay_xqz_col_a", config.players.overlay_xqz_col[3]);

	config.pushitem(&config.players.vis_col[0], "players", "players_vis_col_r", config.players.vis_col[0]);
	config.pushitem(&config.players.vis_col[1], "players", "players_vis_col_g", config.players.vis_col[1]);
	config.pushitem(&config.players.vis_col[2], "players", "players_vis_col_b", config.players.vis_col[2]);
	config.pushitem(&config.players.vis_col[3], "players", "players_vis_col_a", config.players.vis_col[3]);

	config.pushitem(&config.players.invis_col[0], "players", "players_invis_col_r", config.players.invis_col[0]);
	config.pushitem(&config.players.invis_col[1], "players", "players_invis_col_g", config.players.invis_col[1]);
	config.pushitem(&config.players.invis_col[2], "players", "players_invis_col_b", config.players.invis_col[2]);
	config.pushitem(&config.players.invis_col[3], "players", "players_invis_col_a", config.players.invis_col[3]);

	config.pushitem(&config.players.phong_col[0], "players", "players_phong_col_r", config.players.phong_col[0]);
	config.pushitem(&config.players.phong_col[1], "players", "players_phong_col_g", config.players.phong_col[1]);
	config.pushitem(&config.players.phong_col[2], "players", "players_phong_col_b", config.players.phong_col[2]);
	config.pushitem(&config.players.phong_col[3], "players", "players_phong_col_a", config.players.phong_col[3]);

	config.pushitem(&config.players.reflectivity_col[0], "players", "players_reflectivity_col_r", config.players.reflectivity_col[0]);
	config.pushitem(&config.players.reflectivity_col[1], "players", "players_reflectivity_col_g", config.players.reflectivity_col[1]);
	config.pushitem(&config.players.reflectivity_col[2], "players", "players_reflectivity_col_b", config.players.reflectivity_col[2]);
	config.pushitem(&config.players.reflectivity_col[3], "players", "players_reflectivity_col_a", config.players.reflectivity_col[3]);



	config.pushitem(&config.backtrack_chams.visible, "backtrack_chams", "backtrack_chams_visible", config.backtrack_chams.visible);
	config.pushitem(&config.backtrack_chams.alltick, "backtrack_chams", "backtrack_chams_alltick", config.backtrack_chams.alltick);
	config.pushitem(&config.backtrack_chams.gradient, "backtrack_chams", "backtrack_chams_gradient", config.backtrack_chams.gradient);
	config.pushitem(&config.backtrack_chams.invisible, "backtrack_chams", "backtrack_chams_invisible", config.backtrack_chams.invisible);
	config.pushitem(&config.backtrack_chams.pearlescent, "backtrack_chams", "backtrack_chams_pearlescent", config.backtrack_chams.pearlescent);
	config.pushitem(&config.backtrack_chams.phong, "backtrack_chams", "backtrack_chams_phong", config.backtrack_chams.phong);
	config.pushitem(&config.backtrack_chams.reflectivity, "backtrack_chams", "backtrack_chams_reflectivity", config.backtrack_chams.reflectivity);
	config.pushitem(&config.backtrack_chams.basemat, "players", "backtrack_chams_basemat", config.backtrack_chams.basemat);

	config.pushitem(&config.backtrack_chams.vis_col[0], "backtrack_chams", "backtrack_chams_vis_col_r", config.backtrack_chams.vis_col[0]);
	config.pushitem(&config.backtrack_chams.vis_col[1], "backtrack_chams", "backtrack_chams_vis_col_g", config.backtrack_chams.vis_col[1]);
	config.pushitem(&config.backtrack_chams.vis_col[2], "backtrack_chams", "backtrack_chams_vis_col_b", config.backtrack_chams.vis_col[2]);
	config.pushitem(&config.backtrack_chams.vis_col[3], "backtrack_chams", "backtrack_chams_vis_col_a", config.backtrack_chams.vis_col[3]);

	config.pushitem(&config.backtrack_chams.invis_col[0], "backtrack_chams", "backtrack_chams_invis_col_r", config.backtrack_chams.invis_col[0]);
	config.pushitem(&config.backtrack_chams.invis_col[1], "backtrack_chams", "backtrack_chams_invis_col_g", config.backtrack_chams.invis_col[1]);
	config.pushitem(&config.backtrack_chams.invis_col[2], "backtrack_chams", "backtrack_chams_invis_col_b", config.backtrack_chams.invis_col[2]);
	config.pushitem(&config.backtrack_chams.invis_col[3], "backtrack_chams", "backtrack_chams_invis_col_a", config.backtrack_chams.invis_col[3]);

	config.pushitem(&config.backtrack_chams.phong_col[0], "backtrack_chams", "backtrack_chams_phong_col_r", config.backtrack_chams.phong_col[0]);
	config.pushitem(&config.backtrack_chams.phong_col[1], "backtrack_chams", "backtrack_chams_phong_col_g", config.backtrack_chams.phong_col[1]);
	config.pushitem(&config.backtrack_chams.phong_col[2], "backtrack_chams", "backtrack_chams_phong_col_b", config.backtrack_chams.phong_col[2]);
	config.pushitem(&config.backtrack_chams.phong_col[3], "backtrack_chams", "backtrack_chams_phong_col_a", config.backtrack_chams.phong_col[3]);

	config.pushitem(&config.backtrack_chams.reflectivity_col[0], "backtrack_chams", "backtrack_chams_reflectivity_col_r", config.backtrack_chams.reflectivity_col[0]);
	config.pushitem(&config.backtrack_chams.reflectivity_col[1], "backtrack_chams", "backtrack_chams_reflectivity_col_g", config.backtrack_chams.reflectivity_col[1]);
	config.pushitem(&config.backtrack_chams.reflectivity_col[2], "backtrack_chams", "backtrack_chams_reflectivity_col_b", config.backtrack_chams.reflectivity_col[2]);
	config.pushitem(&config.backtrack_chams.reflectivity_col[3], "backtrack_chams", "backtrack_chams_reflectivity_col_a", config.backtrack_chams.reflectivity_col[3]);


	config.pushitem(&config.weapons.enabled, "weapons", "weapons_enabled", config.weapons.enabled);
	config.pushitem(&config.weapons.pearlescent, "weapons", "weapons_pearlescent", config.weapons.pearlescent);
	config.pushitem(&config.weapons.phong, "weapons", "weapons_phong", config.weapons.phong);
	config.pushitem(&config.weapons.reflectivity, "weapons", "weapons_reflectivity", config.weapons.reflectivity);
	config.pushitem(&config.weapons.basemat, "weapons", "weapons_basemat", config.weapons.basemat);

	config.pushitem(&config.weapons.overlaymat, "weapons", "weapons_overlaymat", config.weapons.overlaymat);
	config.pushitem(&config.weapons.wireframe, "weapons", "weapons_wireframe", config.weapons.wireframe);
	config.pushitem(&config.weapons.overlaywireframe, "weapons", "weapons_overlaywireframe", config.weapons.overlaywireframe);
	config.pushitem(&config.weapons.overlay_col[0], "weapons", "weapons_overlay_col_r", config.weapons.overlay_col[0]);
	config.pushitem(&config.weapons.overlay_col[1], "weapons", "weapons_overlay_col_g", config.weapons.overlay_col[1]);
	config.pushitem(&config.weapons.overlay_col[2], "weapons", "weapons_overlay_col_b", config.weapons.overlay_col[2]);
	config.pushitem(&config.weapons.overlay_col[3], "weapons", "weapons_overlay_col_a", config.weapons.overlay_col[3]);

	config.pushitem(&config.weapons.col[0], "weapons", "weapons_col_r", config.weapons.col[0]);
	config.pushitem(&config.weapons.col[1], "weapons", "weapons_col_g", config.weapons.col[1]);
	config.pushitem(&config.weapons.col[2], "weapons", "weapons_col_b", config.weapons.col[2]);
	config.pushitem(&config.weapons.col[3], "weapons", "weapons_col_a", config.weapons.col[3]);

	config.pushitem(&config.weapons.phong_col[0], "weapons", "weapons_phong_col_r", config.weapons.phong_col[0]);
	config.pushitem(&config.weapons.phong_col[1], "weapons", "weapons_phong_col_g", config.weapons.phong_col[1]);
	config.pushitem(&config.weapons.phong_col[2], "weapons", "weapons_phong_col_b", config.weapons.phong_col[2]);
	config.pushitem(&config.weapons.phong_col[3], "weapons", "weapons_phong_col_a", config.weapons.phong_col[3]);

	config.pushitem(&config.weapons.reflectivity_col[0], "weapons", "weapons_reflectivity_col_r", config.weapons.reflectivity_col[0]);
	config.pushitem(&config.weapons.reflectivity_col[1], "weapons", "weapons_reflectivity_col_g", config.weapons.reflectivity_col[1]);
	config.pushitem(&config.weapons.reflectivity_col[2], "weapons", "weapons_reflectivity_col_b", config.weapons.reflectivity_col[2]);
	config.pushitem(&config.weapons.reflectivity_col[3], "weapons", "weapons_reflectivity_col_a", config.weapons.reflectivity_col[3]);


	config.pushitem(&config.hands.enabled, "hands", "hands_enabled", config.hands.enabled);
	config.pushitem(&config.hands.pearlescent, "hands", "hands_pearlescent", config.hands.pearlescent);
	config.pushitem(&config.hands.phong, "hands", "hands_phong", config.hands.phong);
	config.pushitem(&config.hands.reflectivity, "hands", "hands_reflectivity", config.hands.reflectivity);
	config.pushitem(&config.hands.basemat, "hands", "hands_basemat", config.hands.basemat);

	config.pushitem(&config.hands.overlaymat, "hands", "hands_overlaymat", config.hands.overlaymat);
	config.pushitem(&config.hands.wireframe, "hands", "hands_wireframe", config.hands.wireframe);
	config.pushitem(&config.hands.overlaywireframe, "hands", "hands_overlaywireframe", config.hands.overlaywireframe);
	config.pushitem(&config.hands.overlay_col[0], "hands", "hands_overlay_col_r", config.hands.overlay_col[0]);
	config.pushitem(&config.hands.overlay_col[1], "hands", "hands_overlay_col_g", config.hands.overlay_col[1]);
	config.pushitem(&config.hands.overlay_col[2], "hands", "hands_overlay_col_b", config.hands.overlay_col[2]);
	config.pushitem(&config.hands.overlay_col[3], "hands", "hands_overlay_col_a", config.hands.overlay_col[3]);

	config.pushitem(&config.hands.col[0], "hands", "hands_col_r", config.hands.col[0]);
	config.pushitem(&config.hands.col[1], "hands", "hands_col_g", config.hands.col[1]);
	config.pushitem(&config.hands.col[2], "hands", "hands_col_b", config.hands.col[2]);
	config.pushitem(&config.hands.col[3], "hands", "hands_col_a", config.hands.col[3]);

	config.pushitem(&config.hands.phong_col[0], "hands", "hands_phong_col_r", config.hands.phong_col[0]);
	config.pushitem(&config.hands.phong_col[1], "hands", "hands_phong_col_g", config.hands.phong_col[1]);
	config.pushitem(&config.hands.phong_col[2], "hands", "hands_phong_col_b", config.hands.phong_col[2]);
	config.pushitem(&config.hands.phong_col[3], "hands", "hands_phong_col_a", config.hands.phong_col[3]);

	config.pushitem(&config.hands.reflectivity_col[0], "hands", "hands_reflectivity_col_r", config.hands.reflectivity_col[0]);
	config.pushitem(&config.hands.reflectivity_col[1], "hands", "hands_reflectivity_col_g", config.hands.reflectivity_col[1]);
	config.pushitem(&config.hands.reflectivity_col[2], "hands", "hands_reflectivity_col_b", config.hands.reflectivity_col[2]);
	config.pushitem(&config.hands.reflectivity_col[3], "hands", "hands_reflectivity_col_a", config.hands.reflectivity_col[3]);


	config.pushitem(&config.bunny_hop, "misc", "bunny_hop", config.bunny_hop);
	config.pushitem(&config.auto_strafe, "misc", "auto_strafe", config.auto_strafe);
	config.pushitem(&config.backtrack, "misc", "backtrack", config.backtrack);
	config.pushitem(&config.backtrack_val, "misc", "backtrack_val", config.backtrack_val);
	config.pushitem(&config.rankreveal, "misc", "rankreveal", config.rankreveal);
	config.pushitem(&config.edgejump, "misc", "edgejump", config.edgejump);
	config.pushitem(&config.edgejumpkey, "misc", "edgejumpkey", config.edgejumpkey);
	config.pushitem(&config.edgejumpkey_style, "misc", "edgejumpkey_style", config.edgejumpkey_style);
	config.pushitem(&config.faststop, "misc", "faststop", config.faststop);


	config.pushitem(&config.menu_color[0], "menu", "menu_color_r", config.menu_color[0]);
	config.pushitem(&config.menu_color[1], "menu", "menu_color_g", config.menu_color[1]);
	config.pushitem(&config.menu_color[2], "menu", "menu_color_b", config.menu_color[2]);
	config.pushitem(&config.menu_color[3], "menu", "menu_color_a", config.menu_color[3]);
	config.pushitem(&config.menu_key, "menu", "menu_key", config.menu_key);
	config.pushitem(&config.menu_watermark, "menu", "menu_watermark", config.menu_watermark);
	config.pushitem(&config.menu_dpi_scale, "menu", "menu_dpi_scale", config.menu_dpi_scale);

	config.pushitem(&config.announcer.enabled, "announcer", "enabled", config.announcer.enabled);
	config.pushitem(&config.announcer.lang, "announcer", "lang", config.announcer.lang);
	config.pushitem(&config.announcer.log, "announcer", "log", config.announcer.log);
	config.pushitem(&config.announcer.events[0], "announcer", "event_1", config.announcer.events[0]);
	config.pushitem(&config.announcer.events[1], "announcer", "event_2", config.announcer.events[1]);
	config.pushitem(&config.announcer.events[2], "announcer", "event_3", config.announcer.events[2]);
}

bool loading_config;
void ColorPickerr(const char* name, float* color, bool alpha) {

	ImGuiWindow* window = ui::GetCurrentWindow();
	ImGuiStyle* style = &ui::GetStyle();

	ui::SameLine(0 + window->Size.x - 43 - 7 + 18.f);
	auto alphaSliderFlag = alpha ? ImGuiColorEditFlags_AlphaBar : ImGuiColorEditFlags_NoAlpha;

	ui::ColorEdit4(std::string{ "##" }.append(name).append("Picker").c_str(), color, alphaSliderFlag | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip);
}
#define ColorSelect(x1,x2,x3) ui::SameLine(167.f); ColorPickerr(x1, x2, x3);


inline bool file_exists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void ConfigSystem::pushitem(bool* pointer, std::string category, std::string name, bool def_value) {
	this->booleans.push_back({ pointer, category, name, def_value });
}
void ConfigSystem::pushitem(int* pointer, std::string category, std::string name, int def_value) {
	this->ints.push_back({ pointer, category, name, def_value });
}
void ConfigSystem::pushitem(float* pointer, std::string category, std::string name, float def_value) {
	this->floats.push_back({ pointer, category, name, def_value });
}
void ConfigSystem::pushitem(Color* pointer, std::string category, std::string name, Color def_value) {
	this->colors.push_back({ pointer, category, name, def_value });
}



using nlohmann::json;
void ConfigSystem::read(std::string path) {
	std::filesystem::create_directory("c:\\obeluscsgo\\");
	path = "c:\\obeluscsgo\\" + path + ".cfg";
	if (!file_exists(path)) return;

	json retData;
	std::ifstream reader(path);

	json data;
	reader >> data;

	reader.close();

	auto sanityChecks = [](json& d, std::string& c, std::string& n) -> bool {
		return d.find(c) != d.end() && d[c].find(n) != d[c].end();
	};

	for (auto& b : booleans)
		if (sanityChecks(data, b.category, b.name))
			*b.pointer = data[b.category][b.name].get<bool>();
	for (auto& i : ints)
		if (sanityChecks(data, i.category, i.name))
			*i.pointer = data[i.category][i.name].get<int>();
	for (auto& f : floats)
		if (sanityChecks(data, f.category, f.name))
			*f.pointer = data[f.category][f.name].get<float>();
	for (auto& c : colors) {
		if (sanityChecks(data, c.category, c.name)) {
			auto elem = data[c.category][c.name];
			c.pointer->r = elem["r"].get<int>();
			c.pointer->g = elem["g"].get<int>();
			c.pointer->b = elem["b"].get<int>();
			c.pointer->a = elem["a"].get<int>();
		}
	}
	for (auto& i : keybinds)
		if (sanityChecks(data, i.category, i.name))
			*i.pointer = data[i.category][i.name].get<int>();

}

void ConfigSystem::save(std::string path) {
	std::filesystem::create_directory("c:\\obeluscsgo\\");
	path = "c:\\obeluscsgo\\" + path + ".cfg";

	json retData;

	auto sanityChecks = [&](std::string category) -> void {
		if (retData.find(category) == retData.end())
			retData[category] = json();
	};

	for (auto& b : booleans) {
		sanityChecks(b.category);
		retData[b.category][b.name] = *b.pointer;
	}
	for (auto& i : ints) {
		sanityChecks(i.category);
		retData[i.category][i.name] = *i.pointer;
	}
	for (auto& f : floats) {
		sanityChecks(f.category);
		retData[f.category][f.name] = *f.pointer;
	}
	for (auto& c : colors) {
		sanityChecks(c.category);
		json d;
		Color clr = *c.pointer;
		d["r"] = clr.r;
		d["g"] = clr.g;
		d["b"] = clr.b;
		d["a"] = clr.a;
		retData[c.category][c.name] = d;
	}
	for (auto& i : keybinds) {
		sanityChecks(i.category);
		retData[i.category][i.name] = *i.pointer;
	}

	std::ofstream reader(path);
	reader.clear();
	reader << std::setw(4) << retData << std::endl;
	reader.close();
}

void ConfigSystem::Refresh() {
	config_files.clear();
	std::filesystem::create_directory("c:\\obeluscsgo\\");

	for (const auto& p : std::filesystem::recursive_directory_iterator("c:\\obeluscsgo\\")) {
		if (!std::filesystem::is_directory(p) &&
			p.path().extension().string() == ".cfg") {
			auto file = p.path().filename().string();
			config_files.push_back(file.substr(0, file.size() - 4));
		}
	}
}

void ConfigSystem::remove(std::string path) {
	std::filesystem::create_directory("c:\\obeluscsgo\\");
	path = "c:\\obeluscsgo\\" + path + ".cfg";

	std::filesystem::remove(path);
	Refresh();
}
float RandomFloat(float min, float max)
{
	static auto random_float = reinterpret_cast<float(*)(float, float)>(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomFloat"));

	return random_float(min, max);
}
void ConfigSystem::reset() {
	for (auto& b : booleans)
		*b.pointer = b.default_value;
	for (auto& i : ints)
		*i.pointer = i.default_value;
	for (auto& c : colors)
		*c.pointer = c.default_value;
	for (auto& f : floats)
		*f.pointer = f.default_value;
	for (auto& i : keybinds)
		*i.pointer = i.default_value;

}
bool lol;
void ConfigSystem::playerlist()
{
	/*Refresh();

	static std::string selected_cfg = "";
	static char cfg_name[32];

	static auto posCalc = [](int num) -> ImVec2 {
		return ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 3 - 31) * num + 20 * num, ui::GetWindowPos().y + 52 + 27);
	};

	static auto posDouble = [](int num) -> ImVec2 {
		if (num == 1)
			return ImVec2(ui::GetWindowPos().x - 4 + 26 + (ui::GetWindowSize().x / 2 - 36) * num + 20 * num, ui::GetWindowPos().y + 52 + 27);
		else
			return ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 2 - 36) * num + 20 * num, ui::GetWindowPos().y + 52 + 27);
	};

	auto cfg = cconfig::get();
	static auto posDoubleXD = [](int num) -> ImVec2 {
		if (num == 1)
			return ImVec2(ui::GetWindowPos().x - 4 + 26 + (ui::GetWindowSize().x / 2 - 36) * num + 20 * num, ui::GetWindowPos().y + (ui::GetWindowSize().y / 2 - 36) + 52 + 27 - 60);
		else
			return ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 2 - 36) * num + 20 * num, ui::GetWindowPos().y + (ui::GetWindowSize().y / 2 - 36) + 52 + 27 - 60);
	};

	auto child_size = ImVec2(ui::GetWindowSize().x / 3 - 31 + 65 + 18, ui::GetWindowSize().y - 80 - 27);
	auto child_size_d = ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y - 80 - 27);
	auto child_size_xd = ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27);
	auto players = c_playerlist::get().get_players();
	auto key = "plist_" + c_playerlist::get().get_steam_id(players, cfg->i["plist_selected"]) + "_";
	ui::SetNextWindowPos(posDouble(0));

	ui::BeginChild("players", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y - 80 - 27)); {

		ui::ListBoxHeader("##plist", ImVec2(-1, -1 - 50 + 31 - 145 - 20 + 96));

		for (auto p : players) {
			auto name = std::string(p.name);

			std::transform(name.begin(), name.end(), name.begin(), ::tolower);


			if (ui::Selectable(p.name, cfg->i["plist_selected"] == p.userid))
				cfg->i["plist_selected"] = p.userid;
		}

		ui::ListBoxFooter();
		ui::SetCursorPosY(ui::GetWindowSize().y - 100 + 25);
		ui::SetCursorPosX(4);
		if (ui::Button("reset all", ImVec2(ui::GetWindowSize().x - 58 + 6, 0))) {
			for (auto p : players) {
				key = "plist_" + c_playerlist::get().get_steam_id(players, p.userid) + "_";

				cfg->b[key + "friend"] = false;
				cfg->b[key + "noresolver"] = false;
				cfg->b[key + "pitch"] = false;
				cfg->i[key + "pitchval"] = 0;
				cfg->b[key + "yaw"] = false;
				cfg->i[key + "yawval"] = 0;
				cfg->i[key + "yawtype"] = 0;
				cfg->b[key + "baim"] = false;
				cfg->b[key + "novis"] = false;
			}
		}
		ui::SetCursorPosX(4);
		if (ui::Button("apply to all", ImVec2(ui::GetWindowSize().x - 58 + 6, 0))) {
			for (auto p : players) {
				auto key_temp = "plist_" + c_playerlist::get().get_steam_id(players, p.userid) + "_";

				cfg->b[key_temp + "friend"] = cfg->b[key + "friend"];
				cfg->b[key_temp + "noresolver"] = cfg->b[key + "noresolver"];
				cfg->b[key_temp + "pitch"] = cfg->b[key + "pitch"];
				cfg->i[key_temp + "pitchval"] = cfg->i[key + "pitchval"];
				cfg->b[key_temp + "yaw"] = cfg->b[key + "yaw"];
				cfg->i[key_temp + "yawval"] = cfg->i[key + "yawval"];
				cfg->i[key_temp + "yawtype"] = cfg->i[key + "yawtype"];
				cfg->b[key_temp + "baim"] = cfg->b[key + "baim"];
				cfg->b[key_temp + "novis"] = cfg->b[key + "novis"];
			}
		}

	};
	ui::EndChild();

	ui::SetNextWindowPos(posDouble(1));
	ui::BeginChild("options", child_size_d); {

		ui::Checkbox("friend", &cfg->b[key + "novis"]);
		ColorSelect("##friend_col", config.friend_col, true);
		ui::Checkbox("pitch", &cfg->b[key + "pitch"]);
		ui::SliderInt("##custompitch", &cfg->i[key + "pitchval"], -90, 90, "%d°");


		ui::Checkbox("yaw", &cfg->b[key + "yaw"]);
		ui::SliderInt("##customyaw", &cfg->i[key + "yawval"], -180, 180, "%d°");




	};
	ui::EndChild();*/
}

static std::string selected_cfg = "";

bool confirm_opened = false;
bool confirmed = false;
bool deleteshit = false;
void confirm_save_xd(bool deleting)
{
	confirm_opened = true;
	deleteshit = deleting;
}
void final_save()
{
	if (deleteshit)
	{
		config.remove(selected_cfg);

		std::stringstream ss;
		ss << "deleted config " << selected_cfg << ".    ";
		eventlog.add(ss.str().c_str(), Color(30, 240, 30, 255));
	}
	else
	{
		config.save(selected_cfg);

		std::stringstream ss;
		ss << "saved config " << selected_cfg << ".    ";
		eventlog.add(ss.str().c_str(), Color(30, 240, 30, 255));
	}


	confirm_opened = false;
	confirmed = true;
}
void no_confirm()
{
	confirm_opened = false;
	confirmed = false;
	deleteshit ? eventlog.add("deleting cancelled.    ", Color(235, 100, 100, 255)) : eventlog.add("saving cancelled.    ", Color(235, 100, 100, 255));

}

void windowshit()
{
	if (!confirm_opened && g::StoredAlpha > 0.f) {
		float fc = 255.f / 0.2f * ui::GetIO().DeltaTime;
		g::StoredAlpha = std::clamp(g::StoredAlpha - fc / 255.f, 0.f, 1.f);
	}

	if (confirm_opened && g::StoredAlpha < 1.f) {
		float fc = 255.f / 0.2f * ui::GetIO().DeltaTime;
		g::StoredAlpha = std::clamp(g::StoredAlpha + fc / 255.f, 0.f, 1.f);
	}
	if (!confirm_opened && g::StoredAlpha == 0.f)
		return;

	ui::Begin("confirmation", 0, ImVec2(0, 0), 1.f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_Speclist | ImGuiWindowFlags_PlayerlistTab);
	ui::SetWindowSize(ImVec2(240, 145));
	ui::SetCursorPosY(39);
	if (confirm_opened)
		deleteshit ? ui::Text("   are you sure want to delete this config?") : ui::Text("     are you sure want to save this config?");
	ui::SetCursorPosX(25 - 21);
	ui::SetCursorPosY(76 - 15);
	if (confirm_opened && ui::Button(deleteshit ? "delete" : "save", ImVec2(ui::GetWindowSize().x - 58 + 6, 0)))
		final_save();
	ui::SetCursorPosY(116 - 30);
	ui::SetCursorPosX(25 - 21);
	if (confirm_opened && ui::Button("cancel", ImVec2(ui::GetWindowSize().x - 58 + 6, 0)))
		no_confirm();

	ui::End();

}
bool enable_drawing;
float drawing_col[4] = { 255 / 255.f, 95 / 255.f, 142 / 255.f, 255 / 255.f };
void ConfigSystem::menu()
{
	Refresh();


	static char cfg_name[26];

	static auto posCalc = [](int num) -> ImVec2 {
		return ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 3 - 31) * num + 20 * num, ui::GetWindowPos().y + 52 + 27);
	};

	static auto posDouble = [](int num) -> ImVec2 {
		if (num == 1)
			return ImVec2(ui::GetWindowPos().x - 4 + 26 + (ui::GetWindowSize().x / 2 - 36) * num + 20 * num, ui::GetWindowPos().y + 52 + 27);
		else
			return ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 2 - 36) * num + 20 * num, ui::GetWindowPos().y + 52 + 27);
	};

	static auto posDoubleXD = [](int num) -> ImVec2 {
		if (num == 1)
			return ImVec2(ui::GetWindowPos().x - 4 + 26 + (ui::GetWindowSize().x / 2 - 36) * num + 20 * num, ui::GetWindowPos().y + (ui::GetWindowSize().y / 2 - 36) + 52 + 27 - 60);
		else
			return ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 2 - 36) * num + 20 * num, ui::GetWindowPos().y + (ui::GetWindowSize().y / 2 - 36) + 52 + 27 - 60);
	};

	auto child_size = ImVec2(ui::GetWindowSize().x / 3 - 31 + 65 + 18, ui::GetWindowSize().y - 80 - 27);
	auto child_size_d = ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y - 80 - 27);
	auto child_size_xd = ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27);

	ui::SetNextWindowPos(posDouble(0));
	ui::BeginChild("presets", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y - 80 - 27)); {

		ui::ListBoxHeader("##cfglist", ImVec2(-1, -1 - 50 + 31 - 145 - 20));

		for (auto cfg : config_files)
			if (ui::Selectable(cfg.c_str(), cfg == selected_cfg))
				selected_cfg = cfg;

		ui::ListBoxFooter();
		ui::SetCursorPosY(ui::GetWindowSize().y - 153 - 20);
		if (ui::InputText("", cfg_name, 26)) selected_cfg = std::string(cfg_name);
		if (std::string(cfg_name) != "")
		{
			ui::SetCursorPosX(4);
			if (ui::Button("create", ImVec2(ui::GetWindowSize().x - 58 + 6, 0))) {

				if (selected_cfg.empty())
				{
					eventlog.add("failed to create/save config.    ", Color(235, 100, 100, 255));
				}
				else
				{
					std::stringstream ss;
					ss << "created & saved config " << selected_cfg << ".    ";
					eventlog.add(ss.str().c_str(), Color(30, 240, 30, 255));
					save(selected_cfg);
					//skinchanger::get().UpdateSkins();
				}
				Refresh();
			}
		}
		ui::SetCursorPosX(4);
		if (ui::Button("load", ImVec2(ui::GetWindowSize().x - 58 + 6, 0))) {
			if (selected_cfg.empty())
			{
				eventlog.add("failed to load config.    ", Color(235, 100, 100, 255));
			}
			else {
				loading_config = true;
				std::stringstream ss;
				ss << "loaded config " << selected_cfg << ".    ";
				eventlog.add(ss.str().c_str(), Color(30, 240, 30, 255));

				read(selected_cfg);
				//skinchanger::get().UpdateSkins();
				loading_config = false;
			}
			Refresh();
		}
		ui::SetCursorPosX(4);
		if (ui::Button("save", ImVec2(ui::GetWindowSize().x - 58 + 6, 0))) {
			if (selected_cfg.empty())
			{
				eventlog.add("failed to save config.    ", Color(235, 100, 100, 255));
			}
			else
			{
				confirm_save_xd(false);

			}
			Refresh();
		}

		windowshit();

		ui::SetCursorPosX(4);
		if (ui::Button("delete", ImVec2(ui::GetWindowSize().x - 58 + 6, 0))) {
			if (selected_cfg.empty())
			{
				eventlog.add("failed to remove config.    ", Color(235, 100, 100, 255));
			}
			else
			{
				confirm_save_xd(true);

			}
			Refresh();
		}
		ui::SetCursorPosX(4);
		if (ui::Button("reset", ImVec2(ui::GetWindowSize().x - 58 + 6, 0)))
		{
			eventlog.add("successfully resetted menu state.    ", Color(30, 240, 30, 255));
			reset();
			//skinchanger::get().UpdateSkins();
		}


	};
	ui::EndChild();

	//ui::SetNextWindowPos(posDouble(1));
	//ui::BeginChild("drawing", child_size_d); 
	//{
	//	ui::Checkbox("enable drawing", &enable_drawing);
	//	if (enable_drawing)
	//	{
	//		ColorSelect("##drawcol", drawing_col, true);
	//		ImDrawList* draw_list = ui::GetWindowDrawList();
	//		static ImVector<ImVec2> points;
	//		static bool adding_line = false;
	//		if (ui::Button("clear")) points.clear();
	//		if (points.Size >= 2) { if (ui::Button("undo")) { points.pop_back(); points.pop_back(); } }

	//		// Here we are using InvisibleButton() as a convenience to 1) advance the cursor and 2) allows us to use IsItemHovered()
	//		// But you can also draw directly and poll mouse/keyboard by yourself. You can manipulate the cursor using GetCursorPos() and SetCursorPos().
	//		// If you only use the ImDrawList API, you can notify the owner window of its extends by using SetCursorPos(max).
	//		ImVec2 canvas_pos = ui::GetCursorScreenPos();            // ImDrawList API uses screen coordinates!
	//		ImVec2 canvas_size = ui::GetContentRegionAvail();        // Resize canvas to what's available
	//		if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
	//		if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;
	//		draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x - 11, canvas_pos.y - 60 + canvas_size.y), IM_COL32(30, 30, 30, 255), IM_COL32(30, 30, 30, 255), IM_COL32(30, 30, 30, 255), IM_COL32(30, 30, 30, 255));
	//		draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x - 11, canvas_pos.y - 60 + canvas_size.y), IM_COL32(10, 10, 10, 255));

	//		bool adding_preview = false;
	//		ui::InvisibleButton("canvas", canvas_size);
	//		ImVec2 mouse_pos_in_canvas = ImVec2(ui::GetIO().MousePos.x - canvas_pos.x, ui::GetIO().MousePos.y - canvas_pos.y);
	//		if (adding_line)
	//		{
	//			adding_preview = true;
	//			points.push_back(mouse_pos_in_canvas);
	//			if (!ui::IsMouseDown(0))
	//				adding_line = adding_preview = false;
	//		}
	//		if (ui::IsItemHovered())
	//		{
	//			if (!adding_line && ui::IsMouseClicked(0))
	//			{
	//				points.push_back(mouse_pos_in_canvas);
	//				adding_line = true;
	//			}
	//			if (ui::IsMouseClicked(1) && !points.empty())
	//			{
	//				adding_line = adding_preview = false;
	//				points.pop_back();
	//				points.pop_back();
	//			}
	//		}
	//		draw_list->PushClipRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), true);      // clip lines within the canvas (if we resize it, etc.)
	//		for (int i = 0; i < points.Size - 1; i += 2)
	//			draw_list->AddLine(ImVec2(canvas_pos.x + points[i].x, canvas_pos.y + points[i].y), ImVec2(canvas_pos.x + points[i + 1].x, canvas_pos.y + points[i + 1].y), ImColor(drawing_col[0], drawing_col[1], drawing_col[2], drawing_col[3]));
	//		draw_list->PopClipRect();
	//		if (adding_preview)
	//			points.pop_back();
	//	}
	//};
	//ui::EndChild();
}


