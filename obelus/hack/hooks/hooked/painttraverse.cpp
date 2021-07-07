#include "../../features/features.hpp"

using ulong_t = unsigned long;
using VPANEL = ulong_t;

void __fastcall hooks::hkPaintTraverse(void* thisptr, int edx, unsigned int uPanel, bool bForceRepaint, bool bForce) 
{
	static auto oPaintTraverse = detour::paint_traverse.GetOriginal<decltype(&hkPaintTraverse)>();
	static VPANEL tools{}, zoom{};
	int iCurrentWidth, iCurrentHeight = 0;

	interfaces::engine->GetScreenSize(iCurrentWidth, iCurrentHeight);
	if (g::vScreenSize.x != iCurrentWidth || g::vScreenSize.y != iCurrentHeight)
	{
		// re-init rendering so we fix the font issues.
		render::initialize();

		g::vScreenSize.x = iCurrentWidth, g::vScreenSize.y = iCurrentHeight;
	}


	if (!zoom && FNV1a::get(interfaces::panel->get_panel_name(uPanel)) == FNV1A::HashConst("HudZoom"))
		zoom = uPanel;

	if (!tools && uPanel == interfaces::engine_vgui->GetPanel(PANEL_TOOLS))
		tools = uPanel;

	// render shits inside.
	if (uPanel == tools)
	{
		// player & world esp.
		visuals::players::Run();
		visuals::world::Run();

		// event logs.
		eventlog.Think();

		// watermark.
		client::Watermark();
	}

	if (uPanel == zoom && config.noscope)
		return;

	oPaintTraverse(thisptr, edx, uPanel, bForceRepaint, bForce);
}


void __fastcall hooks::hkLockCursor(void* thisptr, int edx)
{
	static auto oLockCursor = detour::lock_cursor.GetOriginal<decltype(&hkLockCursor)>();

	if (g::pLocalPlayer && g::pLocalPlayer->IsAlive() && interfaces::engine->is_in_game())
		interfaces::inputsystem->enable_input(true);

	g_menu.IsMenuOpened() ? interfaces::surface->unlock_cursor() : oLockCursor(thisptr, edx);
}
