#include "interfaces.hpp"
#include "../utilities/csgo.hpp"
#include "../../hack/features/misc/prediction.hpp"

bool interfaces::initialize() 
{

	client::m_kernel32_dll = PE::GetModule(HASH("kernel32.dll"));
	client::m_user32_dll = PE::GetModule(HASH("user32.dll"));
	client::m_shell32_dll = PE::GetModule(HASH("shell32.dll"));
	client::m_shlwapi_dll = PE::GetModule(HASH("shlwapi.dll"));
	client::m_client_dll = PE::GetModule(HASH("client.dll"));
	client::m_engine_dll = PE::GetModule(HASH("engine.dll"));
	client::m_vstdlib_dll = PE::GetModule(HASH("vstdlib.dll"));
	client::m_tier0_dll = PE::GetModule(HASH("tier0.dll"));
	client::m_shaderapidx9_dll = PE::GetModule(HASH("shaderapidx9.dll"));

	console::log(XOR("[setup] clientapi initialized!\n"));
	client = get_interface<IBaseClientDll>(XOR("client.dll"), XOR("VClient"));
	entity_list = get_interface<IClientEntityList>(XOR("client.dll"), XOR("VClientEntityList"));
	engine = get_interface<IVEngineClient>(XOR("engine.dll"), XOR("VEngineClient"));
	panel = get_interface<IPanel>(XOR("vgui2.dll"), XOR("VGUI_Panel"));
	surface = get_interface<ISurface>(XOR("vguimatsurface.dll"), XOR("VGUI_Surface"));
	material_system = get_interface<IMaterialSystem>(XOR("materialsystem.dll"), XOR("VMaterialSystem"));
	model_info = get_interface<IVModelInfo>(XOR("engine.dll"), XOR("VModelInfoClient"));
	model_render = get_interface<IVModelRender>(XOR("engine.dll"), XOR("VEngineModel"));
	render_view = get_interface<IRenderView>(XOR("engine.dll"), XOR("VEngineRenderView"));
	console = get_interface<IConsole>(XOR("vstdlib.dll"), XOR("VEngineCvar"));
	localize = get_interface<ILocalize>(XOR("localize.dll"), XOR("Localize_"));
	event_manager = get_interface<IGameEventManager2>(XOR("engine.dll"), XOR("GAMEEVENTSMANAGER002"));
	debug_overlay = get_interface<IVDebugOverlay>(XOR("engine.dll"), XOR("VDebugOverlay"));
	inputsystem = get_interface<IInputSystem>(XOR("inputsystem.dll"), XOR("InputSystemVersion"));
	trace_ray = get_interface<ITrace>(XOR("engine.dll"), XOR("EngineTraceClient"));
	game_movement = get_interface<IPlayerGameMovement>(XOR("client.dll"), XOR("GameMovement"));
	prediction = get_interface<PlayerPrediction>(XOR("client.dll"), XOR("VClientPrediction"));
	studio_render = get_interface<IStudioRender>(XOR("studiorender.dll"), XOR("VStudioRender"));
	engine_vgui = get_interface<IEngineVGui>(XOR("engine.dll"), XOR("VEngineVGui"));
	engine_sound = get_interface<IEngineSound>(XOR("engine.dll"), XOR("IEngineSoundClient"));
	mem_alloc = *(IMemAlloc**)GetProcAddress(GetModuleHandle(XOR("tier0.dll")), (XOR("g_pMemAlloc")));
	clientmode = **reinterpret_cast<IClientMode * **>((*reinterpret_cast<uintptr_t * *>(client))[10] + 0x5);
	globals = **reinterpret_cast<GlobalVarsBase***>((*reinterpret_cast<uintptr_t**>(client))[0] + 0x1F);
	clientstate = **reinterpret_cast<IClientState***>(pattern::Scan(XOR("engine.dll"), sig_client_state) + 1);
	directx = **reinterpret_cast<IDirect3DDevice9***>(pattern::Scan(XOR("shaderapidx9.dll"), sig_directx) + 1);
	input = *reinterpret_cast<IInput**>(pattern::Scan(XOR("client.dll"), sig_input) + 1);
	glow_manager = reinterpret_cast<GlowManager*>(*reinterpret_cast<uintptr_t*>(pattern::Scan(XOR("client.dll"), sig_glow_manager) + 3));
	move_helper = **reinterpret_cast<PlayerMoveHelper***>(pattern::Scan(XOR("client.dll"), sig_pmovehelper) + 2);
	weapon_system = *reinterpret_cast<IWeaponSystem**>(pattern::Scan(XOR("client.dll"), sig_weapon_data) + 2);
	player_vtable = new customHook(*reinterpret_cast<uintptr_t**>(pattern::Scan(XOR("client.dll"), XOR("55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C")) + 0x47));
	view_render = pattern::Find(client::m_client_dll, XOR("8B 0D ? ? ? ? 57 8B 01 FF 50 14 E8 ? ? ? ? 5F")).add(2).get< CViewRender* >(2);

	g::UTIL_TraceLine = pattern::Find(client::m_client_dll, XOR("55 8B EC 83 E4 F0 83 EC 7C 56 52"));
	g::CTraceFilterSimple_vmt = g::UTIL_TraceLine.add(0x3D).to();
	g::aHasC4 = pattern::Scan(XOR("client.dll"), XOR("56 8B F1 85 F6 74 31"));
	g::SetAbsOrigin = pattern::Scan(XOR("client.dll"), XOR("55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ?"));
	g::LockStudioHdr = pattern::Scan(XOR("client.dll"), XOR("55 8B EC 51 53 8B D9 56 57 8D B3"));
	g::studioHdr = pattern::Find(client::m_client_dll, XOR("8B 86 ? ? ? ? 89 44 24 10 85 C0")).add(2).to< size_t >();
	g::BoneAccessor = pattern::Find(client::m_client_dll, XOR("8D 81 ? ? ? ? 50 8D 84 24")).add(2).to< size_t >();
	g::AngleMatrix = pattern::Find(client::m_client_dll, XOR("E8 ? ? ? ? 8B 07 89 46 0C")).rel32(0x1).as< g::AngleMatrix_t >();
	g::PlayerAnimState = pattern::Find(client::m_client_dll, XOR("8B 8E ? ? ? ? 85 C9 74 3E")).add(2).to< size_t >();
	g::SmokeCount = pattern::Find(client::m_client_dll, XOR("A3 ? ? ? ? 57 8B CB")).add(0x1).to();

	console::log(XOR("[setup] interfaces initialized!\n"));

	convars::update_rate = interfaces::console->FindVar(XOR("cl_updaterate"));
	convars::max_update_rate = interfaces::console->FindVar(XOR("sv_maxupdaterate"));
	convars::interp = interfaces::console->FindVar(XOR("cl_interp"));
	convars::interp_ratio = interfaces::console->FindVar(XOR("cl_interp_ratio"));
	convars::min_interp_ratio = interfaces::console->FindVar(XOR("sv_client_min_interp_ratio"));
	convars::max_interp_ratio = interfaces::console->FindVar(XOR("sv_client_max_interp_ratio"));
	convars::max_unlag = interfaces::console->FindVar(XOR("sv_maxunlag"));
	convars::cl_cmdrate = interfaces::console->FindVar(XOR("cl_cmdrate"));
	convars::cl_lagcompensation = interfaces::console->FindVar(XOR("cl_lagcompensation"));
	convars::mp_teammates_are_enemies = interfaces::console->FindVar(XOR("mp_teammates_are_enemies"));
	convars::cl_crosshair_sniper_width = interfaces::console->FindVar(XOR("cl_crosshair_sniper_width"));
	convars::r_drawspecificstaticprop = interfaces::console->FindVar(XOR("r_drawspecificstaticprop"));
//	prediction::m_nPredictionRandomSeed = utilities::GetMethod(prediction, 19).add(0x30).get< int* >();
//	prediction::m_pPredictionPlayer = utilities::GetMethod(prediction, 19).add(0x54).get< BaseEntity* >();

	console::log(XOR("[setup] cvars initialized!\n"));

	return true;
}

template <typename T>
T* interfaces::get_interface(const char* module_name, const std::string& szInterface) {
	static auto GetRegisterList = [module_name]() -> CInterfaceRegister*
	{
		FARPROC oCreateInterface = nullptr;

		if (const auto hModule = GetModuleHandle(module_name); hModule != nullptr)
			oCreateInterface = GetProcAddress(hModule, (XOR("CreateInterface")));

		if (oCreateInterface == nullptr)
			throw std::runtime_error((XOR("failed get interface address")));

		const std::uintptr_t	uCreateInterfaceJmp = (std::uintptr_t)oCreateInterface + 0x4;
		const std::int32_t		iJmpDisp = *(std::int32_t*)(uCreateInterfaceJmp + 0x1);
		const std::uintptr_t	uCreateInterface = uCreateInterfaceJmp + 0x5 + iJmpDisp;
		return **(CInterfaceRegister***)(uCreateInterface + 0x6);
	};

	for (auto pRegister = GetRegisterList(); pRegister != nullptr; pRegister = pRegister->pNext)
	{
		if ((!std::string(pRegister->szName).compare(0U, szInterface.length(), szInterface) &&
			atoi(pRegister->szName + szInterface.length()) > 0) ||
			!szInterface.compare(pRegister->szName))
			return (T*)pRegister->pCreateFn();
	}

	return nullptr;
}