#pragma once

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#define FMT_HEADER_ONLY
#include "../../client/fmt/format.h"
#include "../../client/fmt/chrono.h"

#include <cstdint>
#include <string>
#include <stdexcept>

#include "i_base_client_dll.hpp"
#include "i_client_entity_list.hpp"
#include "iv_engine_client.hpp"
#include "i_client_state.hpp"
#include "i_panel.hpp"
#include "i_surface.hpp"
#include "c_global_vars_base.hpp"
#include "i_material_system.hpp"
#include "iv_model_info.hpp"
#include "iv_model_render.hpp"
#include "iv_debug_overlay.hpp"
#include "i_console.hpp"
#include "i_localize.hpp"
#include "i_game_event_manager.hpp"
#include "i_input.hpp"
#include "i_input_system.hpp"
#include "i_trace.hpp"
#include "i_render_view.hpp"
#include "glow_manager.hpp"
#include "i_player_movement.hpp"
#include "i_weapon_system.hpp"

using MemAllocFailHandler_t = std::size_t(__cdecl*)(std::size_t);

class IMemAlloc
{
public:
	// Release versions
	virtual void* Alloc(std::size_t nSize) = 0;
	virtual void* Realloc(void* pMemory, std::size_t nSize) = 0;
	virtual void  Free(void* pMemory) = 0;
	virtual void* Expand(void* pMemory, std::size_t nSize) = 0; // no longer supported

	// Debug versions
	virtual void* Alloc(std::size_t nSize, const char* szFileName, int nLine) = 0;
	virtual void* Realloc(void* pMemory, std::size_t nSize, const char* szFileName, int nLine) = 0;
	virtual void  Free(void* pMemory, const char* szFileName, int nLine) = 0;
	virtual void* Expand(void* pMemory, std::size_t nSize, const char* szFileName, int nLine) = 0; // no longer supported

	// Returns size of a particular allocation
	virtual std::size_t GetSize(void* pMemory) = 0;
};


using CreateInterfaceFn = void* (__cdecl*)(const char*, int*);
using InstantiateInterfaceFn = void* (__cdecl*)(void);
using DemoCustomDataCallbackFn = void(__cdecl*)(unsigned char*, std::size_t);

class CInterfaceRegister
{
public:
	InstantiateInterfaceFn	pCreateFn;
	const char* szName;
	CInterfaceRegister* pNext;
};

class customHook
{
public:
	explicit customHook(void* ent)
	{
		base = reinterpret_cast<uintptr_t**>(ent);
	}

	template<typename function, typename original_function>
	function apply(uint32_t index, original_function func)
	{
		auto old_func = base[index];
		DWORD old;
		VirtualProtect(&base[index], sizeof uintptr_t, PAGE_EXECUTE_READWRITE, &old);
		base[index] = reinterpret_cast<uintptr_t*>(func);
		VirtualProtect(&base[index], sizeof uintptr_t, old, &old);
		return reinterpret_cast<function>(old_func);
	}
private:

	std::uintptr_t** base;
};

enum VGuiPanel_t 
{
	PANEL_ROOT = 0, 
	PANEL_GAMEUIDLL, 
	PANEL_CLIENTDLL, 
	PANEL_TOOLS, 
	PANEL_INGAMESCREENS,
	PANEL_GAMEDLL,
	PANEL_CLIENTDLL_TOOLS
};

class IEngineVGui {
public:
	virtual	~IEngineVGui(void) {}
	virtual uint32_t GetPanel(VGuiPanel_t type) = 0;
	virtual bool IsGameUIVisible() = 0;
};
class CViewSetup {
public:
	int         m_x;					// 0x0000
	int         m_old_x;				// 0x0004
	int         m_y;					// 0x0008
	int	        m_old_y;				// 0x000C
	int         m_width;				// 0x0010
	int         m_old_width;			// 0x0014
	int         m_height;				// 0x0018
	int         m_old_height;			// 0x001C
	bool        m_ortho;				// 0x0020
	float       m_ortho_left;			// 0x0024
	float       m_ortho_top;			// 0x0028
	float       m_ortho_right;			// 0x002C
	float       m_ortho_bottom;			// 0x0030
	bool        m_custom_view_matrix;	// 0x0034
	matrix_t m_custom_matrix;		// 0x0038
private:
	PAD(0x48);						// 0x0068
public:
	float       m_fov;                  // 0x00B0
	float       m_viewmodel_fov;		// 0x00B4
	vec3_t      m_origin;
	vec3_t       m_angles;
	float       m_znear;
	float       m_zfar;
	float       m_znear_viewmodel;
	float       m_zfar_viewmodel;
	float       m_aspect_ratio;
	float       m_near_blur_depth;
	float       m_near_focus_depth;
	float       m_far_focus_depth;
	float       m_far_blur_depth;
	float       m_near_blur_radius;
	float       m_far_blur_radius;
	int         m_quality;
	int         m_motionblur_mode;
	float       m_shutter_time;
	vec3_t      m_shutter_open_pos;
	vec3_t       m_shutter_open_angles;
	vec3_t      m_shutter_close_pos;
	vec3_t       m_shutter_close_angles;
	float       m_offcenter_top;
	float       m_offcenter_bottom;
	float       m_offcenter_left;
	float       m_offcenter_right;
	int         m_edge_blur;
};

class CViewRender {
public:
	// virtuals.
	__forceinline const CViewSetup* GetPlayerViewSetup(int slot = -1) {
		return utilities::GetMethod< const CViewSetup* (__thiscall*)(decltype(this), int) >(this, 12)(this, slot);
	}

	// members.
	PAD(0x4);
	CViewSetup m_view;
};
class IEngineSound
{
public:
	virtual bool precachesound(const char* pSample, bool bPreload = false, bool bIsUISound = false) = 0;
	virtual bool issoundprecached(const char* pSample) = 0;
};

namespace interfaces {
	enum class interface_type { index, bruteforce };

	template <typename T>
	T* get_interface(const char*, const std::string&);

	inline IBaseClientDll* client;
	inline IInput* input;
	inline IClientEntityList* entity_list;
	inline IVEngineClient* engine;
	inline IClientMode* clientmode;
	inline IClientState* clientstate;
	inline IPanel* panel;
	inline ISurface* surface;
	inline GlobalVarsBase* globals;
	inline IMaterialSystem* material_system;
	inline IVModelInfo* model_info;
	inline IVModelRender* model_render;
	inline IRenderView* render_view;
	inline IEngineSound* engine_sound;
	inline IVDebugOverlay* debug_overlay;
	inline IConsole* console;
	inline ILocalize* localize;
	inline IGameEventManager2* event_manager;
	inline IInputSystem* inputsystem;
	inline IDirect3DDevice9* directx;
	inline ITrace* trace_ray;
	inline GlowManager* glow_manager;
	inline IPlayerGameMovement* game_movement;
	inline PlayerPrediction* prediction;
	inline PlayerMoveHelper* move_helper;
	inline IWeaponSystem* weapon_system;
	inline IStudioRender* studio_render;
	inline IMemAlloc* mem_alloc;
	inline customHook* player_vtable;
	inline IEngineVGui* engine_vgui;
	inline CViewRender* view_render;

	bool initialize();
}


