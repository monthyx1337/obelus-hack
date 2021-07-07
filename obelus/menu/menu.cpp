#include "menu.h"
#include "imgui/fonts.h"
#include "imgui/imgui_internal.h"
#include "../hack/features/skins/skins.h"
#include "../hack/features/misc/announcer.h"

Menu g_menu;
Input g_input;
Keyhandler g_keyhandler;
 
std::size_t NumFiles(std::filesystem::path path)
{
	using std::filesystem::directory_iterator;
	return std::distance(directory_iterator(path), directory_iterator{});
}

void Menu::Draw()
{
	Menu::Handle();

	if (!this->m_bIsOpened && ui::GetStyle().Alpha == 0.f)
		return;

	ui::SetNextWindowSizeConstraints(ImVec2(490 + 8 + 4 + 2 + 3 + 4 + 5, 556 + 7), ImVec2(g::iWidth - 2, g::iHeight - 2));
	ui::Begin("obelus csgo", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MainWindow);
	ui::TabButton("aimbot", &this->m_bCurrentTab, 0, 6);
	ui::TabButton("antiaim", &this->m_bCurrentTab, 1, 6);
	ui::TabButton("visuals", &this->m_bCurrentTab, 2, 6);
	ui::TabButton("misc", &this->m_bCurrentTab, 3, 6);
	ui::TabButton("config", &this->m_bCurrentTab, 4, 6);
	ui::TabButton("skins", &this->m_bCurrentTab, 5, 6);

	auto child_size = ImVec2(ui::GetWindowSize().x / 3 - 31 + 65 + 18, ui::GetWindowSize().y - 80 - 27);
	auto child_size_d = ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y - 80 - 27);
	auto child_size_xd = ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27);

	static auto posCalc = [](int num) -> ImVec2
	{
		return ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 3 - 31) * num + 20 * num, ui::GetWindowPos().y + 52 + 27);
	};

	static auto posDouble = [](int num) -> ImVec2
	{
		if (num == 1)
			return ImVec2(ui::GetWindowPos().x - 4 + 26 + (ui::GetWindowSize().x / 2 - 36) * num + 20 * num, ui::GetWindowPos().y + 52 + 27);
		else
			return ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 2 - 36) * num + 20 * num, ui::GetWindowPos().y + 52 + 27);
	};


	if (this->m_bCurrentTab == 0)
	{

		ui::SetNextWindowPos(ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 2 - 36) * 0 + 20 * 0, ui::GetWindowPos().y + 52 + 27));
		ui::BeginChild("other", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 65 + 31 - 30 - 30 - 120 + 24));
		{

		}
		ui::EndChild();

	}

	if (this->m_bCurrentTab == 1)
	{

		ui::SetNextWindowPos(posDouble(0));
		ui::BeginChild("anti-aim", child_size_d);
		{


		}
		ui::EndChild();


		ui::SetNextWindowPos(posDouble(1));
		ui::BeginChild("other", child_size_d);
		{

		}
		ui::EndChild();

	}

	if (this->m_bCurrentTab == 2)
	{

		ui::SetNextWindowPos(posDouble(0));
		ui::BeginChild("players", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 65 + 31 - 30 - 30));
		{
			ui::Checkbox("bounding box", &config.box);
			ColorSelect("##box_col", config.box_col, true);
			ui::Checkbox("name", &config.name);
			ColorSelect("##name_col", config.name_col, true);
			ui::Checkbox("weapon", &config.weapon);
			ColorSelect("##weapon_col", config.weapon_col, true);
			ui::Checkbox("weapon icon", &config.weapon_icon);
			ColorSelect("##weaponicon_col", config.weapon_icon_col, true);
			ui::Checkbox("ammo", &config.ammo);
			ColorSelect("##ammo_col", config.ammo_col, true);
			ui::Checkbox("health", &config.health);
			ui::Checkbox("flags", &config.flags);
			if (config.flags)
				ui::MultiSelect("##pflags", &config.playerflags, { "money", "armor", "scoped", "flashed", "reload", "hparmor text" });
			ui::Checkbox("glow", &config.glow);
			ColorSelect("##glow", config.glow_col, true);
			ui::Checkbox("dormant", &config.dormant);
			if (config.dormant)
				ui::SliderInt("##dormancy", &config.dormancy, 1, 10, "%ds");

		}
		ui::EndChild();

		ui::SetNextWindowPos(ImVec2(ui::GetWindowPos().x - 4 + 26 + 4 + (ui::GetWindowSize().x / 2 - 36) * 0 + 20 * 0, ui::GetWindowPos().y + (ui::GetWindowSize().y / 2 - 36) + 52 + 27 - 60 + 68 - 30));
		ui::BeginChild("others", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 27 + 1 + 30));
		{
			ui::MultiSelect("dropped weapons", &config.droppedweapons, { "text", "icon" });
			ColorSelect("##dropped_col", config.dropped_col, true);
			ui::Checkbox("dropped weapons ammo", &config.dropped_ammo);
			ColorSelect("##dropped_ammo_col", config.dropped_ammo_col, true);
			ui::Checkbox("grenades", &config.nades);
			ColorSelect("##nades_col", config.nades_col, true);
			ui::Checkbox("grenade prediciton", &config.nadepred);
			ColorSelect("##pc", config.nadepred_col, true);
			ui::Checkbox("bomb", &config.bomb);
			ui::Checkbox("radar", &config.radar);
			ui::Checkbox("velocity render", &config.velocity);
			ui::Checkbox("velocity graph", &config.velocitygraph);
			ui::Checkbox("recoil crosshair", &config.recoilcross);
			ui::SliderInt("viewmodel fov", &config.viewmodel_fov, 0, 60, "%d%%");
			ui::SliderInt("view fov", &config.view_fov, 0, 60, "%d%%");
		}
		ui::EndChild();

		ui::SetNextWindowPos(posDouble(1));
		ui::BeginChild("colored models", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 65 + 31 - 30));
		{
			static int selected = 0;
			ui::SingleSelect("model", &selected, { "players", "hands"/*, "weapons"*/ });

			switch (selected)
			{
			case 0:
			{
				ui::SingleSelect("##basemat", &config.players.basemat, { "normal", "flat" });
				ui::Checkbox("enabled", &config.players.visible);
				ColorSelect("##visible_col", config.players.vis_col, true);
				ui::Checkbox("behind walls", &config.players.invisible);
				ColorSelect("##invisible_col", config.players.invis_col, true);
				ui::Checkbox("wireframe", &config.players.wireframe);
				if (config.players.basemat == 0)
				{
					ui::Checkbox("phong", &config.players.phong);
					ColorSelect("##phong_col", config.players.phong_col, true);
					ui::Checkbox("reflectivity", &config.players.reflectivity);
					ColorSelect("##reflectivity_col", config.players.reflectivity_col, true);
					ui::SliderInt("pearlescent", &config.players.pearlescent, 0, 100, "%d%%");
				}
				ui::SingleSelect("overlay material", &config.players.overlaymat, { "none", "glow", "devglow" });
				ColorSelect("##overlayco", config.players.overlay_col, true);
				ui::Checkbox("overlay wireframe", &config.players.overlaywireframe);
				if (config.players.overlaymat != 0)
				{
					ui::Checkbox("overlay behind walls", &config.players.overlayxqz);
					ColorSelect("##overlayco2", config.players.overlay_xqz_col, true);
				}
			}
			break;
			case 1:
			{
				ui::SingleSelect("##basemat", &config.hands.basemat, { "normal", "flat" });
				ui::Checkbox("enabled", &config.hands.enabled);
				ColorSelect("##col", config.hands.col, true);
				ui::Checkbox("wireframe", &config.hands.wireframe);
				if (config.hands.basemat == 0)
				{
					ui::Checkbox("phong", &config.hands.phong);
					ColorSelect("##phong_col", config.hands.phong_col, true);
					ui::Checkbox("reflectivity", &config.hands.reflectivity);
					ColorSelect("##reflectivity_col", config.hands.reflectivity_col, true);
					ui::SliderInt("pearlescent", &config.hands.pearlescent, 0, 100, "%d%%");
				}

				ui::SingleSelect("overlay material", &config.hands.overlaymat, { "none", "glow", "devglow" });
				ColorSelect("##overlayco", config.hands.overlay_col, true);
				ui::Checkbox("overlay wireframe", &config.hands.overlaywireframe);
			}
			break;
			case 2:
			{
				ui::SingleSelect("##basemat", &config.weapons.basemat, { "normal", "flat" });
				ui::Checkbox("enabled", &config.weapons.enabled);
				ColorSelect("##col", config.weapons.col, true);
				ui::Checkbox("wireframe", &config.weapons.wireframe);
				if (config.weapons.basemat == 0)
				{
					ui::Checkbox("phong", &config.weapons.phong);
					ColorSelect("##phong_col", config.weapons.phong_col, true);
					ui::Checkbox("reflectivity", &config.weapons.reflectivity);
					ColorSelect("##reflectivity_col", config.weapons.reflectivity_col, true);
					ui::SliderInt("pearlescent", &config.weapons.pearlescent, 0, 100, "%d%%");
				}

				ui::SingleSelect("overlay material", &config.weapons.overlaymat, { "none", "glow", "devglow" });
				ColorSelect("##overlayco", config.weapons.overlay_col, true);
				ui::Checkbox("overlay wireframe", &config.weapons.overlaywireframe);

			}
			break;
			/*case 2:
			{
				ui::SingleSelect("##basemat", &config.backtrack_chams.basemat, { "normal", "flat" });
				ui::Checkbox("enabled", &config.backtrack_chams.visible);
				ColorSelect("##visible_col", config.backtrack_chams.vis_col, true);
				ui::Checkbox("render all ticks", &config.backtrack_chams.alltick);
				ui::Checkbox("gradient", &config.backtrack_chams.gradient);
				if (config.backtrack_chams.basemat == 0)
				{
					ui::Checkbox("phong", &config.backtrack_chams.phong);
					ColorSelect("##phong_col", config.backtrack_chams.phong_col, true);
					ui::Checkbox("reflectivity", &config.backtrack_chams.reflectivity);
					ColorSelect("##reflectivity_col", config.backtrack_chams.reflectivity_col, true);
					ui::SliderInt("pearlescent", &config.backtrack_chams.pearlescent, 0, 100, "%d%%");
				}	

			}
			break;*/
			}


		}
		ui::EndChild();

		ui::SetNextWindowPos(ImVec2(ui::GetWindowPos().x - 4 + 26 + (ui::GetWindowSize().x / 2 - 36) * 1 + 20 * 1, ui::GetWindowPos().y + (ui::GetWindowSize().y / 2 - 36) + 52 + 27 - 60 + 68));
		ui::BeginChild("effects", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 27 + 1));
		{
			ui::Checkbox("remove scope", &config.noscope);
			ui::Checkbox("remove scope zoom", &config.noscopezoom);
			ui::Checkbox("remove smoke", &config.nosmoke);
			ui::Checkbox("remove flash effects", &config.noflash);
			ui::Checkbox("remove visual recoil", &config.novisrecoil);
			ui::Checkbox("disable post processing", &config.disablepostprocess);

			//ui::Checkbox("full bright", &config.fullbright);
			ui::Checkbox("night mode", &config.nightmode);
			if (config.nightmode)
			{
				ui::SliderInt("##nval", &config.brightness, 1, 100, "%d%%");
			}
			ui::Checkbox("custom fire color", &config.changemolotov);
			ColorSelect("##fcol", config.molotov_col, true);
		}
		ui::EndChild();
	}

	if (this->m_bCurrentTab == 3)
	{

		ui::SetNextWindowPos(ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 2 - 36) * 0 + 20 * 0, ui::GetWindowPos().y + 52 + 27));
		ui::BeginChild("menu", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 65 + 31 - 30 - 30 - 120 + 24));
		{
			ui::Text("menu color");
			ColorSelect("##menu_color", config.menu_color, true);
			ui::Text("menu key");
			ui::Keybind("##menukey", &config.menu_key);
			ui::Checkbox("watermark", &config.menu_watermark);
			ui::SingleSelect("dpi scale", &config.menu_dpi_scale, { "100%" });
		}
		ui::EndChild();
		ui::SetNextWindowPos(ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 2 - 36) * 0 + 20 * 0, ui::GetWindowPos().y + (ui::GetWindowSize().y / 2 - 36) + 52 + 27 + 95 - 236 + 24 - 3));
		ui::BeginChild("movement", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 65 + 31 - 30 - 45 - 8 - 4 + 147));
		{
			ui::Checkbox("bunny hop", &config.bunny_hop);
			ui::Checkbox("air strafe", &config.auto_strafe);
			//ui::Checkbox("wasd strafe", &config.movement_strafe);	
			ui::Checkbox("infinite duck", &config.infinite_duck, false);	
			//ui::Checkbox("jump bug", &config.jumpbug);
			//ui::Keybind("##jbug", &config.jumpbugkey, &config.jumpbugkey_style);
			ui::Checkbox("edge jump", &config.edgejump);
			ui::Keybind("##ej", &config.edgejumpkey, &config.edgejumpkey_style);
			ui::Checkbox("fast stop", &config.faststop);

		}
		ui::EndChild();

		ui::SetNextWindowPos(posDouble(1));
		ui::BeginChild("others", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 + 27 - 94 + 70 + 37));
		{
			//ui::Checkbox("auto accept matchmaking", &config.auto_accept);
			ui::Checkbox("reveal competitive ranks", &config.rankreveal);
			//ui::Checkbox("announcer", &config.announcer.enabled);
			//if (config.announcer.enabled)
			//{
			//	if (NumFiles("C:\\obeluscsgo\\audio\\hun\\") != 22)
			//	{
			//		announcer::bWork = false;
			//		ui::Text("missing audio files!");
			//		ui::Text(std::to_string(NumFiles("C:\\obeluscsgo\\audio\\hun\\")).c_str());
			//		ui::SameLine();
			//		ui::Text("/  22");

			//		if (NumFiles("C:\\obeluscsgo\\audio\\hun\\") == 0)
			//		if (ui::Button("download audio pack (4)", ImVec2(ui::GetWindowSize().x - 58 + 6 - 19, 0)))
			//			announcer::initialize(1);
			//		if (NumFiles("C:\\obeluscsgo\\audio\\hun\\") == 5)
			//		if (ui::Button("download audio pack (3)", ImVec2(ui::GetWindowSize().x - 58 + 6 - 19, 0)))
			//			announcer::initialize(2);
			//		if (NumFiles("C:\\obeluscsgo\\audio\\hun\\") == 10)
			//		if (ui::Button("download audio pack (2)", ImVec2(ui::GetWindowSize().x - 58 + 6 - 19, 0)))
			//			announcer::initialize(3);
			//		if (NumFiles("C:\\obeluscsgo\\audio\\hun\\") == 16)
			//		if (ui::Button("download audio pack (1)", ImVec2(ui::GetWindowSize().x - 58 + 6 - 19, 0)))
			//			announcer::initialize(4);


			//		if (NumFiles("C:\\obeluscsgo\\audio\\hun\\") > 16)
			//		{
			//			if (ui::Button("download missing files", ImVec2(ui::GetWindowSize().x - 58 + 6 - 19, 0)))
			//			{
			//				announcer::initialize(1);
			//				announcer::initialize(2);
			//				announcer::initialize(3);
			//				announcer::initialize(4);
			//			}

			//		}


			//	}
			//	else
			//	{
			//		announcer::bWork = true;
			//		ui::SingleSelect("language", &config.announcer.lang, { "hungarian", "fasz" });
			//		ui::MultiSelect("events", &config.announcer.events, { "round start", "round end", "player kills"/*, "planted bomb"*/ });
			//		ui::Checkbox("log plays", &config.announcer.log);
			//	}


			//	
			//}

		
		}
		ui::EndChild();

		ui::SetNextWindowPos(ImVec2(ui::GetWindowPos().x - 4 + 26 + (ui::GetWindowSize().x / 2 - 36) * 1 + 20 * 1, ui::GetWindowPos().y + (ui::GetWindowSize().y / 2 - 36) + 52 + 27 - 60 + 68 + 70 - 3 - 24 + 37));
		ui::BeginChild("extra", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 27 + 1 - 50 - 12 - 5 + 24 - 37));
		{
			if (ui::Button("force crash", ImVec2(ui::GetWindowSize().x - 58 + 6 - 19, 0)))
			{
				Sleep(1000000000000000);
			}
		}
		ui::EndChild();

	}

	if (this->m_bCurrentTab == 4)
	{

		config.menu();

		ui::SetNextWindowPos(ImVec2(ui::GetWindowPos().x - 4 + 26 + (ui::GetWindowSize().x / 2 - 36) * 1 + 20 * 1, ui::GetWindowPos().y + 52 + 27));
		ui::BeginChild("extra", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y - 80 - 27));
		{

		}
		ui::EndChild();


	}

	if (this->m_bCurrentTab == 5)
	{
		static bool faszom = true;
		if (faszom)
		{
			skins::get().first_time_render = true;
			faszom = false;
		}
		skins::get().menu();
		ui::SetNextWindowPos(ImVec2(ui::GetWindowPos().x - 4 + 26 + 4 + (ui::GetWindowSize().x / 2 - 36) * 0 + 20 * 0, ui::GetWindowPos().y + (ui::GetWindowSize().y / 2 - 36) + 52 + 27 - 60 + 68 - 30 - 40));
		ui::BeginChild("extra", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 27 + 1 + 30 + 40));
		{

			ui::Checkbox("custom viewmodel offset", &config.custom_viewmodel);
			if (config.custom_viewmodel)
			{
				ui::SliderFloat("offset x", &config.viewmodel_offset_x, -25.f, 25.f, "%.1f");
				ui::SliderFloat("offset y", &config.viewmodel_offset_y, -25.f, 25.f, "%.1f");
				ui::SliderFloat("offset z", &config.viewmodel_offset_z, -25.f, 25.f, "%.1f");
			}

		}
		ui::EndChild();
	}

	ui::End();
}

void Menu::DrawEnd() 
{
	if (!this->m_bInitialized)
		return;

	ui::EndFrame();
	ui::Render();
	ImGui_ImplDX9_RenderDrawData(ui::GetDrawData());
}

bool Menu::IsMenuInitialized() 
{
	return this->m_bInitialized;
}

bool Menu::IsMenuOpened() 
{
	return this->m_bIsOpened;
}

void Menu::SetMenuOpened(bool v) 
{
	this->m_bIsOpened = v;
}

void Menu::Initialize(IDirect3DDevice9* pDevice)
{
	if (this->m_bInitialized)
		return;

	ui::CreateContext();
	auto io = ui::GetIO();
	auto style = &ui::GetStyle();

	style->WindowRounding = 0.f;
	style->AntiAliasedLines = false;
	style->AntiAliasedFill = false;
	style->ScrollbarRounding = 0.f;
	style->ScrollbarSize = 6.f;
	style->WindowPadding = ImVec2(0, 0);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(45 / 255.f, 45 / 255.f, 45 / 255.f, 1.f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(65 / 255.f, 65 / 255.f, 65 / 255.f, 1.f);

	this->m_bIsOpened = true;

	IDirect3DSwapChain9* pChain = nullptr;
	D3DPRESENT_PARAMETERS pp = {};
	D3DDEVICE_CREATION_PARAMETERS param = {};
	pDevice->GetCreationParameters(&param);
	pDevice->GetSwapChain(0, &pChain);

	if (pChain)
		pChain->GetPresentParameters(&pp);

	ImGui_ImplWin32_Init(param.hFocusWindow);
	ImGui_ImplDX9_Init(pDevice);


	ImFontConfig cfg;

	cfg.RasterizerFlags = 1 << 7 | 1 << 4; // semi-disable antialiasing

	float m_flDpiScale = 1;
	io.Fonts->AddFontFromMemoryTTF(fs_tahoma_8px_ttf, fs_tahoma_8px_ttf_len, 12.0f * m_flDpiScale, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->AddFontFromMemoryTTF(fs_tahoma_8px_ttf, fs_tahoma_8px_ttf_len, 12.0f * m_flDpiScale, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->AddFontFromMemoryTTF(keybinds_font, 25600, 10.f * m_flDpiScale, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->AddFontFromMemoryTTF(undefeated_ttf, undefeated_ttf_len, 22.f * m_flDpiScale, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	io.Fonts->AddFontDefault();

	ImGuiFreeType::BuildFontAtlas(io.Fonts, 0x00);

	this->m_bInitialized = true;
}


void Menu::DrawBegin(IDirect3DDevice9* device) {

	static bool once{ false };
	if (!once) 
	{
		g_menu.Initialize(device);

		once = true;
	}

	if (!this->m_bInitialized)
		return;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ui::NewFrame();
}

void Menu::Handle()
{
	if (!this->m_bIsOpened && ui::GetStyle().Alpha > 0.f)
	{
		float fc = 255.f / 0.2f * ui::GetIO().DeltaTime;
		ui::GetStyle().Alpha = std::clamp(ui::GetStyle().Alpha - fc / 255.f, 0.f, 1.f);
	}

	if (this->m_bIsOpened && ui::GetStyle().Alpha < 1.f)
	{
		float fc = 255.f / 0.2f * ui::GetIO().DeltaTime;
		ui::GetStyle().Alpha = std::clamp(ui::GetStyle().Alpha + fc / 255.f, 0.f, 1.f);
	}

}

bool Keyhandler::IsKeyDown(int key) 
{
	return HIWORD(GetKeyState(key));
}

bool Keyhandler::IsKeyUp(int key) 
{
	return !HIWORD(GetKeyState(key));
}

bool Keyhandler::CheckKey(int key, int keystyle) 
{
	switch (keystyle) {
	case 0:
		return true;
	case 1:
		return this->IsKeyDown(key);
	case 2:
		return LOWORD(GetKeyState(key));
	case 3:
		return this->IsKeyUp(key);
	default:
		return true;
	}
}