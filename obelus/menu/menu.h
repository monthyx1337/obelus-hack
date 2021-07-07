#pragma once
#include "../client/utilities/csgo.hpp"
#include "../hack/menu/variables.h"
#include "imgui/imgui.h"
#include "imgui/imgui_freetype.h"
#pragma comment(lib, "freetype.lib")
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"

#define ColorSelect(x1,x2,x3) ui::SameLine(167.f); g_menu.ColorPicker(x1, x2, x3);

class Menu  
{
public:

	void Initialize(IDirect3DDevice9* pDevice);
	void DrawBegin(IDirect3DDevice9* pDevice);
	void Draw();
	void DrawEnd();
	void Handle();
	bool IsMenuInitialized();
	bool IsMenuOpened();
	void SetMenuOpened(bool v);

	int m_bAimbotType;
	int m_iWeaponType;
	int rifle_selected;
	int sniper_selected;
	int smg_selected;
	int auto_selected;
	int other_selected;
	int baszom_anyadat_budos_cigany_fasszopo_roman_stheoknak_meg_cat_vagy_ki_a_geci_nek_soha_nem_lesz_skeetje;

	bool m_bInitialized;
	bool m_bIsOpened;
	IDirect3DTexture9* m_pTexture;
	int m_bCurrentTab;
	int chams_to_edit;
	int chams_layer;

	Color ConvertColor(float* col) 
	{
		return Color((int)(col[0] * 255.f), (int)(col[1] * 255.f), (int)(col[2] * 255.f), (int)(col[3] * 255.f));
	}

	Color ConvertColor(float* col, float alpha_override) 
	{
		return Color((int)(col[0] * 255.f), (int)(col[1] * 255.f), (int)(col[2] * 255.f), (int)(alpha_override * 255.f));
	}

	void ColorPicker(const char* name, float* color, bool alpha)
	{

		ImGuiWindow* window = ui::GetCurrentWindow();
		ImGuiStyle* style = &ui::GetStyle();

		ui::SameLine(0 + window->Size.x - 43 - 7 + 18.f);
		auto alphaSliderFlag = alpha ? ImGuiColorEditFlags_AlphaBar : ImGuiColorEditFlags_NoAlpha;

		ui::ColorEdit4(std::string{ "##" }.append(name).append("Picker").c_str(), color, alphaSliderFlag | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip);
	}

};

extern Menu g_menu;

class Keyhandler {
public:
	bool IsKeyDown(int key);
	bool IsKeyUp(int key);
	bool CheckKey(int key, int keystyle);
};

extern Keyhandler g_keyhandler;


class Input {
public:
	struct key_t {
		bool down;
		bool pressed;
		int  tick;
		int  oldtick;
	};

	std::array< key_t, 256 > m_keys;
	vec2_t					 m_mouse;
	vec2_t					 m_delta;
	int m_scroll;
	std::string				 m_buffer;

public:

	__forceinline void updateMouse() {
		POINT p;
		if (!GetCursorPos(&p))
			return;

		ScreenToClient(g::hWindow, &p);

		m_delta = m_mouse - vec2_t(
			static_cast<float>(p.x),
			static_cast<float>(p.y));

		m_mouse = {
			static_cast<float>(p.x),
			static_cast<float>(p.y) };
	}

	__forceinline void update() {
		// iterate all keys.
		for (int i{}; i <= 254; ++i) {
			key_t* key = &m_keys[i];

			key->pressed = false;

			if (key->down && key->tick > key->oldtick) {
				key->oldtick = key->tick;
				key->pressed = true;
			}
		}
	}

	// mouse within coords.
	__forceinline bool IsCursorInBounds(int x, int y, int x2, int y2) const {
		return m_mouse.x > x && m_mouse.y > y && m_mouse.x < x2&& m_mouse.y < y2;
	}

	// mouse within rectangle.
	__forceinline bool IsCursorInRect(Rect area) const {
		return IsCursorInBounds(area.x, area.y, area.x + area.w, area.y + area.h);
	}

	__forceinline void SetDown(int vk) {
		key_t* key = &m_keys[vk];

		key->down = true;
		key->tick = GetTickCount();
	}

	__forceinline void SetUp(int vk) {
		key_t* key = &m_keys[vk];
		key->down = false;
	}

	// key is being held.
	__forceinline bool GetKeyState(int vk) {
		if (vk == -1)
			return false;

		return m_keys[vk].down;
	}

	// key was pressed.
	__forceinline bool GetKeyPress(int vk) {
		if (vk == -1)
			return false;

		key_t* key = &m_keys[vk];
		return key->pressed;
	}
};

extern Input g_input;