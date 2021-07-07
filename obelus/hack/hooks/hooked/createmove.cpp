#include "../../features/features.hpp"

bool __fastcall hooks::hkCreateMove(void* thisptr, int edx, float flInputSampleTime, UserCmd* pCmd) 
{
	static auto oCreateMove = detour::create_move.GetOriginal<decltype(&hkCreateMove)>();
	oCreateMove(thisptr, edx, flInputSampleTime, pCmd);

	bool ret = detour::create_move.GetOriginal<decltype(&hkCreateMove)>();

	if (!pCmd || !pCmd->command_number)
		return ret;

	if (ret)
		interfaces::engine->set_view_angles(pCmd->viewangles);

	g::pLocalPlayer = interfaces::entity_list->get<BaseEntity>(interfaces::engine->get_local_player());
	if (g::pLocalPlayer)
	{
		g::pLocalWeapon = g::pLocalPlayer->ActiveWeapon();
		if (g::pLocalWeapon)
			g::pLocalWeaponData = interfaces::weapon_system->get_weapon_data(g::pLocalWeapon->ItemDefinitionIndex());
	}

	const auto vecViewangles = pCmd->viewangles;
	client::MouseDelta();
	g::pCmd = pCmd;
	g::iButtons = pCmd->buttons;
	g::iFlags = g::pLocalPlayer->Flags();
	g::bAlive = g::pLocalPlayer && g::pLocalPlayer->IsAlive();
	g::fLatency = interfaces::clientstate->net_channel->get_latency(0);

	if (g_menu.IsMenuOpened())
		pCmd->buttons &= ~IN_ATTACK;

	misc::Bunnyhop();
	misc::InfiniteDuck();
	misc::ViewModelOffset();
	misc::RankRevealer();
	misc::RecoilCrosshair();
	misc::FastStop();

	int unFlags = g::pLocalPlayer->Flags();

	prediction.Start(pCmd, g::pLocalPlayer);
	{
		misc::AutoStrafe();
		visuals::GatherMovementData();	
		grenades::NadePred();
	}
	prediction.End(pCmd, g::pLocalPlayer);

	int preFlags = g::pLocalPlayer->Flags();

	misc::EdgeJump(unFlags);
	misc::JumpBug(unFlags, preFlags);

	math::FixMove(pCmd, vecViewangles);

	return false;
}