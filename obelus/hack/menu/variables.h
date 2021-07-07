#pragma once
#include "config\config.h"

struct Variables_t
{

	#pragma region player visuals
		C_ADDVARIABLE(bool, box, false);
		C_ADDVARIABLE(bool, name, false);
		C_ADDVARIABLE(bool, weapon, false);
		C_ADDVARIABLE(bool, ammo, false);
		C_ADDVARIABLE(bool, health, false);
		C_ADDVARIABLE(bool, flags, false);
		C_ADDVARIABLE(bool, glow, false);
		C_ADDVARIABLE(bool, radar, false);
		C_ADDVARIABLE(bool, dormant, true);
		C_ADDVARIABLE(float, dormancy, 6.0f);
		C_ADDVARIABLE(bool, chams, false);
		C_ADDVARIABLE(bool, chams_xqz, false);
		C_ADDVARIABLE(bool, flat_chams, false);
		C_ADDVARIABLE(bool, droppedweapon, false);
		C_ADDVARIABLE(bool, droppedweapon_ammo, false);
		C_ADDVARIABLE(bool, bomb, false);
		C_ADDVARIABLE(bool, grenades, false);

		C_ADDVARIABLE(bool, remove_scope_overlay, false);


		C_ADDVARIABLE(bool, phong, false);
		C_ADDVARIABLE(Color, phong_col, Color(255, 255, 255, 255));
		C_ADDVARIABLE(float, pearlescent, 0);
		C_ADDVARIABLE(bool, reflectivity, false);
		C_ADDVARIABLE(Color, reflectivity_col, Color(255, 255, 255, 255));

		C_ADDVARIABLE(int, br, 255);
		C_ADDVARIABLE(int, bg, 255);
		C_ADDVARIABLE(int, bb, 255);
		C_ADDVARIABLE(int, ba, 220);

		C_ADDVARIABLE(Color, box_col, Color(255, 255, 255, 0xb4));
		C_ADDVARIABLE(Color, name_col, Color(255, 255, 255, 0xb4));
		C_ADDVARIABLE(Color, dropped_col, Color(255, 255, 255, 0xb4));
		C_ADDVARIABLE(Color, nade_col, Color(255, 255, 255, 0xb4));
		C_ADDVARIABLE(Color, weapon_col, Color(255, 255, 255, 0xb4));
		C_ADDVARIABLE(Color, dropped_ammo_col, Color(80, 140, 200, 0xb4));
		C_ADDVARIABLE(Color, ammo_col, Color(80, 140, 200, 0xb4));
		C_ADDVARIABLE(Color, glow_col, Color(180, 60, 120, 153));
		C_ADDVARIABLE(Color, chams_col, Color(150, 200, 60, 255));
		C_ADDVARIABLE(Color, chams_xqz_col, Color(60, 120, 180, 179));
		C_ADDVARIABLE(Color, menu_col, Color(43, 114, 227, 255));
	#pragma endregion player visuals

	#pragma region visuals
		C_ADDVARIABLE(bool, nightmode, false);
		C_ADDVARIABLE(bool, full_bright, false);
		C_ADDVARIABLE(float, nightmode_brightness, 80.0f);

		C_ADDVARIABLE(int, flashlight_key, 0);
		C_ADDVARIABLE(int, flashlight_key_method, 1);
		C_ADDVARIABLE(bool, flashlight, false);
		C_ADDVARIABLE(float, flashlight_fov, 35);
	#pragma endregion visuals

	#pragma region misc/movement
		C_ADDVARIABLE(bool, bhop, false);
		C_ADDVARIABLE(bool, strafe, false);
		C_ADDVARIABLE(bool, wasdstrafe, false);
		C_ADDVARIABLE(bool, fast_duck, false);
		C_ADDVARIABLE(int, edge_jump_key, 0);
		C_ADDVARIABLE(int, edge_key_method, 1);
		C_ADDVARIABLE(bool, edge_jump, false);
		C_ADDVARIABLE(int, jump_bug_key, 0);
		C_ADDVARIABLE(int, jump_key_method, 1);
		C_ADDVARIABLE(bool, jump_bug, 0);
		C_ADDVARIABLE(int, edge_bug_key, 0);
		C_ADDVARIABLE(int, edge_bug_method, 1);
		C_ADDVARIABLE(bool, edge_bug, 0);

		C_ADDVARIABLE(bool, backtrack, false);
		C_ADDVARIABLE(float, backtrack_val, 200);
	#pragma endregion misc/movement

	#pragma region menu pos
		C_ADDVARIABLE(int, menu_x, 400);
		C_ADDVARIABLE(int, menu_y, 200);
	#pragma endregion menu pos
};

inline Variables_t Vars;
