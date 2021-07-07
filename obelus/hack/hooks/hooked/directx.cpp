#include "../../features/features.hpp"
#include "../../../menu/menu.h"

HRESULT D3DAPI hooks::hkPresent(IDirect3DDevice9Ex* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion)
{
	static auto oPresent = detour::present.GetOriginal<decltype(&hkPresent)>();

	static bool initialized_renderer = false;

	if (!initialized_renderer)
	{
		initialized_renderer = true;
		config.SetupValues();
		config.Refresh();
	}
	else
	{
		g_input.update();
		IDirect3DStateBlock9* state = NULL;
		IDirect3DVertexDeclaration9* vertDec;
		IDirect3DVertexShader9* vertShader;
		device->CreateStateBlock(D3DSBT_PIXELSTATE, &state);
		state->Capture();
		device->GetVertexDeclaration(&vertDec);
		device->GetVertexShader(&vertShader);

		device->SetVertexShader(nullptr);
		device->SetPixelShader(nullptr);
		device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		device->SetRenderState(D3DRS_LIGHTING, false);
		device->SetRenderState(D3DRS_FOGENABLE, false);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
		device->SetRenderState(D3DRS_ZWRITEENABLE, false);
		device->SetRenderState(D3DRS_STENCILENABLE, false);

		device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, false);
		device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, true);

		device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

		device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
		device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

		device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

		g_menu.DrawBegin(device);
		{
			g_menu.Draw();
		}
		g_menu.DrawEnd();

		state->Apply();
		state->Release();
		device->SetVertexDeclaration(vertDec);
		device->SetVertexShader(vertShader);
	}
	return oPresent(device, src, dest, windowOverride, dirtyRegion);
}

HRESULT D3DAPI hooks::hkReset(IDirect3DDevice9Ex* device, D3DPRESENT_PARAMETERS* params)
{
	static auto oPresent = detour::reset.GetOriginal<decltype(&hkReset)>();
	return oPresent(device, params);
}

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI hooks::WndProc(HWND wnd, uint32_t msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_LBUTTONDOWN:
		g_input.SetDown(VK_LBUTTON);
		break;

	case WM_LBUTTONUP:
		g_input.SetUp(VK_LBUTTON);
		break;

	case WM_RBUTTONDOWN:
		g_input.SetDown(VK_RBUTTON);
		break;

	case WM_RBUTTONUP:
		g_input.SetUp(VK_RBUTTON);
		break;

	case WM_MBUTTONDOWN:
		g_input.SetDown(VK_MBUTTON);
		break;

	case WM_MBUTTONUP:
		g_input.SetUp(VK_MBUTTON);
		break;

	case WM_XBUTTONDOWN:
		if (GET_XBUTTON_WPARAM(wp) == XBUTTON1)
			g_input.SetDown(VK_XBUTTON1);

		else if (GET_XBUTTON_WPARAM(wp) == XBUTTON2)
			g_input.SetDown(VK_XBUTTON2);

		break;

	case WM_XBUTTONUP:
		if (GET_XBUTTON_WPARAM(wp) == XBUTTON1)
			g_input.SetUp(VK_XBUTTON1);

		else if (GET_XBUTTON_WPARAM(wp) == XBUTTON2)
			g_input.SetUp(VK_XBUTTON2);

		break;

	case WM_KEYDOWN:
		if ((size_t)wp < g_input.m_keys.size())
			g_input.SetDown(wp);

		break;

	case WM_KEYUP:
		if ((size_t)wp < g_input.m_keys.size())
			g_input.SetUp(wp);

		break;

	case WM_SYSKEYDOWN:
		if (wp == VK_MENU)
			g_input.SetDown(VK_MENU);

		break;

	case WM_SYSKEYUP:
		if (wp == VK_MENU)
			g_input.SetUp(VK_MENU);

		break;

	case WM_MOUSEWHEEL:
		g_input.m_scroll = (GET_WHEEL_DELTA_WPARAM(wp) / WHEEL_DELTA);
		break;

	case WM_CHAR:
		switch (wp) {
		case VK_BACK:
			if (!g_input.m_buffer.empty())
				g_input.m_buffer.pop_back();
			break;

		case VK_ESCAPE:
		case VK_TAB:
		case VK_RETURN:
			break;

		default:
			if (wp > 0 && wp < 0x10000)
				g_input.m_buffer += static_cast<char>(wp);

			break;
		}

		break;

	default:
		break;
	}

	if (msg == WM_KEYDOWN)
	{
		if (wp == VK_INSERT)
			g_menu.SetMenuOpened(!g_menu.IsMenuOpened());
	}

	if (g_menu.m_bIsOpened)
	{
		ImGui_ImplWin32_WndProcHandler(wnd, msg, wp, lp);

		if (wp != 'W' && wp != 'A' && wp != 'S' && wp != 'D' && wp != VK_SHIFT && wp != VK_CONTROL && wp != VK_TAB && wp != VK_SPACE || ui::GetIO().WantTextInput)
			return true;
	}


	return CallWindowProcA(g::OldWndProc, wnd, msg, wp, lp);
}