#include "csgo.hpp"
#include "../../hack/features/features.hpp"

void client::initialize()
{
	interfaces::initialize();
	render::initialize();
	hooks::initialize();

	if (interfaces::engine->is_in_game())
		client::OnMapload();

	listeners.initialize();
	skins::get().AnimationHook();
	client::Welcome();

	netvars.initialize();
	paintkits::initialize();
}

void client::uninitialize()
{
	config.reset();
	visuals::ModulateWorld();
	skins::get().AnimationUnHook();
	hooks::player::UnHook();
	hooks::vars::UnHook();
	listeners.UnregisterEvents();

	interfaces::inputsystem->enable_input(true);
	SetWindowLongW(FindWindowW(L"Valve001", NULL), GWL_WNDPROC, reinterpret_cast<LONG>(g::OldWndProc));

	CDetourHook::RemoveAll();
	console::release();
}

void client::Welcome()
{
	interfaces::engine->execute_cmd("clear");

	static const auto faszfej = []() -> std::pair<std::string, char>
	{
		std::stringstream str;

		str << " " << std::endl;
		str << "_____________________________________________________________________________________________\n" << std::endl;
		str <<
			"       ___.          .__                				created by:\n"
			"   ____\\_ |__   ____ |  |  __ __  ______			        <monthyx>\n"
			"  /  _ \\| __ \\_/ __ \\|  | |  |  \\/  ___/			        <skver>\n"
			" |  |_| | \\_\\ \\  ___/|  |_|  |  /\\___ \\		                        <v5>	  \n"
			" \\____/|___  /\\___  >____/____//____  /\n"
			"           \\/     \\/                \\/ \n"
			;
		str << " \n";
		str << "              alpha build" << std::endl;
		str << " \n";
		str << "_____________________________________________________________________________________________\n" << std::endl;
		str << "  welcome to obelus, " << g::pUser << "!" << std::endl;

		str << " \n";
		str << "  build date: " << __DATE__ << " " << __TIME__ << std::endl;
		str << " \n";
		str << "_____________________________________________________________________________________________\n" << std::endl;
		constexpr auto key = random::_char<__COUNTER__>::value;
		auto txt = str.str();

		for (auto& c : txt)
			c ^= key;

		return std::make_pair(txt, key);
	}();

	c(msg, faszfej);

	utilities::Print(msg);
	console::log(msg);
}

void client::OnMapload()
{
	netvars.SetupClassData();
	g::pLocalPlayer = interfaces::entity_list->get<BaseEntity>(interfaces::engine->get_local_player());
	visuals::ModulateWorld();
}

void client::Watermark()
{
	static int alpha = 255;
	static int low_alpha = 100;

	if (!config.menu_watermark && alpha == 0)
		return;

	time_t t = std::time(nullptr);
	std::ostringstream time;
	time << std::put_time(std::localtime(&t), ("%H:%M:%S"));

	int ms = std::max(0, (int)std::round(g::fLatency * 1000.f));

	int rate = (int)std::round(1.f / interfaces::globals->interval_per_tick);

	std::string build;

#ifdef _DEBUG
	build = XOR("[debug]");
#else
	build = XOR("[alpha]");
#endif


	if (config.menu_watermark)
	{
		alpha = std::min(alpha + 7, 255);
		low_alpha = std::min(low_alpha + 7, 100);
	}
	else
	{
		alpha = std::max(alpha - 7, 0);
		low_alpha = std::max(low_alpha - 7, 0);
	}

	std::string text = tfm::format(XOR("obelus %s | %s | rtt: %ims | rate: %i | %s"), build, "benko", ms, rate, time.str().data());
	render::FontSize_t size = render::hud.size(text);

	// outline.
	render::rect_filled(g::iWidth - size.iWidth - 23, 5, size.iWidth + 15, size.iHeight + 12, { 5, 5, 5, alpha });
	render::rect_filled(g::iWidth - size.iWidth - 22, 6, size.iWidth + 13, size.iHeight + 10, { 45, 45, 45, alpha });

	// box.
	render::rect_filled(g::iWidth - size.iWidth - 21, 7, size.iWidth + 11, size.iHeight + 8, { 24, 24, 24, alpha });

	// line.
	render::rect_filled(g::iWidth - size.iWidth - 20, 8, size.iWidth + 9, size.iHeight - 12, { g_menu.ConvertColor(config.menu_color).r,g_menu.ConvertColor(config.menu_color).g,g_menu.ConvertColor(config.menu_color).b, alpha });
	render::rect_filled(g::iWidth - size.iWidth - 20, 9, size.iWidth + 9, size.iHeight - 12, { g_menu.ConvertColor(config.menu_color).r,g_menu.ConvertColor(config.menu_color).g,g_menu.ConvertColor(config.menu_color).b, low_alpha });

	// text.
	render::hud.string(g::iWidth - 15, 12, { 186, 186, 186, alpha }, text, render::ALIGN_RIGHT);

}

void client::MouseDelta()
{
	if (!g::pLocalPlayer || !g::pCmd)
		return;

	static vec3_t delta_viewangles{ };
	vec3_t delta = g::pCmd->viewangles - delta_viewangles;
	delta.clamp();

	static convar* sensitivity = interfaces::console->FindVar("sensitivity");

	if (!sensitivity)
		return;

	if (delta.x != 0.f) {
		static convar* m_pitch = interfaces::console->FindVar("m_pitch");

		if (!m_pitch)
			return;

		int final_dy = static_cast<int>((delta.x / m_pitch->GetFloat()) / sensitivity->GetFloat());
		if (final_dy <= 32767) {
			if (final_dy >= -32768) {
				if (final_dy >= 1 || final_dy < 0) {
					if (final_dy <= -1 || final_dy > 0)
						final_dy = final_dy;
					else
						final_dy = -1;
				}
				else {
					final_dy = 1;
				}
			}
			else {
				final_dy = 32768;
			}
		}
		else {
			final_dy = 32767;
		}

		g::pCmd->mousedy = static_cast<short>(final_dy);
	}

	if (delta.y != 0.f) {
		static convar* m_yaw = interfaces::console->FindVar("m_yaw");

		if (!m_yaw)
			return;

		int final_dx = static_cast<int>((delta.y / m_yaw->GetFloat()) / sensitivity->GetFloat());
		if (final_dx <= 32767) {
			if (final_dx >= -32768) {
				if (final_dx >= 1 || final_dx < 0) {
					if (final_dx <= -1 || final_dx > 0)
						final_dx = final_dx;
					else
						final_dx = -1;
				}
				else {
					final_dx = 1;
				}
			}
			else {
				final_dx = 32768;
			}
		}
		else {
			final_dx = 32767;
		}

		g::pCmd->mousedx = static_cast<short>(final_dx);
	}

	delta_viewangles = g::pCmd->viewangles;
}