#pragma once
#include "../client/utilities/csgo.hpp"
#include <filesystem>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>


template <typename T>
struct item_t {
	T* pointer;
	std::string category;
	std::string name;
	T default_value;
};


class ConfigSystem {
private:
	std::vector<item_t<bool>> booleans;
	std::vector<item_t<int>> ints;
	std::vector<item_t<float>> floats;
	std::vector<item_t<Color>> colors;
	std::vector<item_t<int>> keybinds;
public:
	void pushitem(bool* pointer, std::string category, std::string name, bool def_value);
	void pushitem(int* pointer, std::string category, std::string name, int def_value);
	void pushitem(float* pointer, std::string category, std::string name, float def_value);
	void pushitem(Color* pointer, std::string category, std::string name, Color def_value);
	void SetupValues();
	void read(std::string path);
	void save(std::string path);
	void remove(std::string path);
	void Refresh();
	void reset();
	void menu();
	void playerlist();
	std::vector<std::string> config_files;


	bool backtrack;
	int backtrack_val = 200;

	bool box;
	float box_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 0xb4 / 255.f };
	bool name;
	float name_col[4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 0xb4 / 255.f };
	bool weapon;
	float weapon_col[4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 0xb4 / 255.f };
	bool weapon_icon;
	float weapon_icon_col[4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 0xb4 / 255.f };
	bool ammo;
	float ammo_col[4] = { 80 / 255.f, 140 / 255.f, 200 / 255.f, 0xb4 / 255.f };
	bool glow;
	float glow_col[4] = { 180 / 255.f, 60 / 255.f, 120 / 255.f, 150 / 255.f };
	bool flags;
	std::unordered_map<int, bool> playerflags;
	bool health;
	bool dormant = true;
	int dormancy = 5;

	std::unordered_map<int, bool> droppedweapons;

	bool dropped;
	float dropped_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 0xb4 / 255.f };
	bool dropped_ammo;
	float dropped_ammo_col[4] = { 80 / 255.f, 140 / 255.f, 200 / 255.f, 0xb4 / 255.f };
	bool nades;
	float nades_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 0xb4 / 255.f };
	bool bomb;
	bool radar;
	bool offscreen;
	bool velocity;
	bool velocitygraph;
	int velocitycomp = 1;
	int velocitywidth = 25;

	bool changemolotov;

	bool nightmode;
	bool recoilcross;
	bool rankreveal;
	bool nadepred;
	bool fullbright;
	int brightness = 85;
	bool noscope;
	bool nosmoke;
	bool noflash;
	bool disablepostprocess;
	bool flashlight;
	int flashlight_key;
	int flashlight_key_style;
	int flashlight_fov;
	bool faststop;
	bool novisrecoil;
	bool noscopezoom;
	int view_fov;
	int viewmodel_fov;

	float sky_col[4] = { 60 / 255.f, 60 / 255.f, 60 / 255.f };
	float molotov_col[4] = { 50 / 255.f, 255 / 255.f, 180 / 255.f };
	float nadepred_col[4] = { 80 / 255.f, 140 / 255.f, 200 / 255.f, 230 / 255.f };
	float offscreen_col[4] = { 255 / 255.f, 30 / 255.f, 300 / 255.f, 190 / 255.f };
	struct {
		bool visible;
		bool invisible;
		bool phong;
		bool reflectivity;
		bool wireframe;
		int basemat;
		float vis_col[4] = { 150 / 255.f, 200 / 255.f, 60 / 255.f, 255 / 255.f };
		float invis_col[4] = { 60 / 255.f, 120 / 255.f, 180 / 255.f, 255 / 255.f };
		float phong_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 255 / 255.f };
		float reflectivity_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 255 / 255.f };
		int pearlescent = 0;
		int overlaymat;
		bool overlaywireframe;
		bool overlayxqz;
		float overlay_col[4] = { 150 / 255.f, 150 / 255.f, 150 / 255.f, 255 / 255.f };
		float overlay_xqz_col[4] = { 150 / 255.f, 150 / 255.f, 150 / 255.f, 255 / 255.f };

	}players;

	struct {
		bool enabled;
		bool phong;
		bool reflectivity;
		bool wireframe;
		int basemat;
		int overlaymat;
		bool overlaywireframe;
		float col[4] = { 150 / 255.f, 200 / 255.f, 60 / 255.f, 255 / 255.f };
		float phong_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 255 / 255.f };
		float reflectivity_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 255 / 255.f };
		float overlay_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 255 / 255.f };
		int pearlescent = 0;
	}hands;

	struct {
		bool enabled;
		bool phong;
		bool reflectivity;
		bool wireframe;
		int basemat;
		int overlaymat;
		bool overlaywireframe;
		float col[4] = { 150 / 255.f, 200 / 255.f, 60 / 255.f, 255 / 255.f };
		float phong_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 255 / 255.f };
		float reflectivity_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 255 / 255.f };
		float overlay_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 255 / 255.f };
		int pearlescent = 0;
	}weapons;

	struct {
		bool enabled;
		bool phong;
		bool reflectivity;
		bool wireframe;
		int basemat;
		int overlaymat;
		bool overlaywireframe;
		float col[4] = { 150 / 255.f, 200 / 255.f, 60 / 255.f, 255 / 255.f };
		float phong_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 255 / 255.f };
		float reflectivity_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 255 / 255.f };
		float overlay_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 255 / 255.f };
		int pearlescent = 0;
	}sleeves;

	struct {
		bool visible;
		bool invisible;
		bool alltick;
		bool gradient;
		bool phong;
		bool reflectivity;
		int basemat = 1;
		float vis_col[4] = { 215 / 255.f, 120 / 255.f, 250 / 255.f, 35 / 255.f };
		float invis_col[4] = { 60 / 255.f, 120 / 255.f, 180 / 255.f, 255 / 255.f };
		float phong_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 255 / 255.f };
		float reflectivity_col[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 255 / 255.f };
		int pearlescent = 0;
	}backtrack_chams;

	bool bunny_hop;
	bool auto_accept;
	bool auto_strafe;
	bool movement_strafe;
	bool infinite_duck;

	bool edgebug;
	int edgebugkey;
	int edgebugkey_style = 1;

	bool jumpbug;
	int jumpbugkey;
	int jumpbugkey_style = 1;
	
	bool edgejump;
	int edgejumpkey;
	int edgejumpkey_style = 1;


	int knife_model;
	int knife_skin;
	float glove_wear;
	int glove_model;
	int glove_skin_blood = 0;
	int glove_skin_sport = 0;
	int glove_skin_driver = 0;
	int glove_skin_handwar = 0;
	int glove_skin_motor = 0;
	int glove_skin_specialist = 0;
	int glove_skin_hydra = 0;
	int glove_skin_fang = 0;


	bool custom_viewmodel = true;
	float viewmodel_offset_x = 1.f;
	float viewmodel_offset_y = 1.f;
	float viewmodel_offset_z = -1.f;

	int menu_key = 0x2D;
	int menu_dpi_scale = 2;
	bool menu_watermark = true;
	float menu_color[4] = { 85 / 255.f, 135 / 255.f, 235 / 255.f, 255 / 255.f };


	struct {
		bool enabled;
		int lang;
		bool log;
		std::unordered_map<int, bool> events;
		int volume = 50.f;
		
	}announcer;


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
	
};

extern ConfigSystem config;

