#include "../features.hpp"

#define D3DX_PI    ((FLOAT)  3.141592654f)

void misc::Bunnyhop()
{
	if (!config.bunny_hop)
		return;

	//if (m_bIsJumpbugging)
	//	return;

	if (g::pLocalPlayer->MoveType() == MOVETYPE_LADDER || 
		g::pLocalPlayer->MoveType() == MOVETYPE_NOCLIP || 
		g::pLocalPlayer->MoveType() == MOVETYPE_OBSERVER)
		return;

	static auto last_jumped = false;
	static auto should_fake = false;

	if (!last_jumped && should_fake)
	{
		should_fake = false;
		g::pCmd->buttons |= IN_JUMP;
	}
	else if (g::pCmd->buttons & IN_JUMP)
	{
		if (g::pLocalPlayer->Flags() & FL_ONGROUND)
		{
			last_jumped = true;
			should_fake = true;
		}
		else
		{
			g::pCmd->buttons &= ~IN_JUMP;
			last_jumped = false;
		}
	}
	else
	{
		last_jumped = false;
		should_fake = false;
	}
}


void misc::RecoilCrosshair()
{
	static auto cl_crosshair_recoil = interfaces::console->FindVar("cl_crosshair_recoil");
	cl_crosshair_recoil->SetValue(config.recoilcross ? 1 : 0);

}

void misc::RankRevealer()
{
	if (config.rankreveal)
	{
		if (GetAsyncKeyState(VK_TAB))
			interfaces::client->dispatch_user_message(cs_um_serverrankrevealall, 0, 0, nullptr);
	}
}

void misc::ViewModelOffset()
{
	if (!config.custom_viewmodel)
	{
		interfaces::console->FindVar("viewmodel_offset_x")->SetValue(1);
		interfaces::console->FindVar("viewmodel_offset_y")->SetValue(1);
		interfaces::console->FindVar("viewmodel_offset_z")->SetValue(-1);
	}
	else
	{
		interfaces::console->FindVar("viewmodel_offset_x")->SetValue(config.viewmodel_offset_x);
		interfaces::console->FindVar("viewmodel_offset_y")->SetValue(config.viewmodel_offset_y);
		interfaces::console->FindVar("viewmodel_offset_z")->SetValue(config.viewmodel_offset_z);
	}

}


void misc::FastStop()
{
	if (!config.faststop)
		return;

	auto cmd = g::pCmd;
	if (!(g::pLocalPlayer->Flags() & FL_ONGROUND))
		return;

	auto pressed_move_key = cmd->buttons & IN_FORWARD || cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_BACK || cmd->buttons & IN_MOVERIGHT || cmd->buttons & IN_JUMP;

	if (pressed_move_key)
		return;
	auto velocity = g::pLocalPlayer->Velocity();

	if (velocity.length_2d() > 20.0f)
	{
		vec3_t direction;
		vec3_t real_view;

		math::vector_angles(velocity, direction);
		interfaces::engine->get_view_angles(real_view);

		direction.y = real_view.y - direction.y;

		vec3_t forward;
		math::angle_vectors(direction, forward);

		static auto cl_forwardspeed = interfaces::console->FindVar("cl_forwardspeed");
		static auto cl_sidespeed = interfaces::console->FindVar("cl_sidespeed");

		auto negative_forward_speed = -cl_forwardspeed->GetFloat();
		auto negative_side_speed = -cl_sidespeed->GetFloat();

		auto negative_forward_direction = forward * negative_forward_speed;
		auto negative_side_direction = forward * negative_side_speed;

		cmd->forwardmove = negative_forward_direction.x;
		cmd->sidemove = negative_side_direction.y;
	}

}

void misc::AutoStrafe() 
{
	if (g::pLocalPlayer->MoveType() == MOVETYPE_LADDER ||
		g::pLocalPlayer->MoveType() == MOVETYPE_NOCLIP ||
		g::pLocalPlayer->MoveType() == MOVETYPE_OBSERVER)
		return;

	if (g::pLocalPlayer->Flags() & FL_ONGROUND)
		return;

	static auto cl_sidespeed = interfaces::console->FindVar(XOR("cl_sidespeed"));
	auto side_speed = cl_sidespeed->GetFloat();

	if (config.movement_strafe)
	{
		static auto old_yaw = 0.0f;

		auto get_velocity_degree = [](float velocity)
		{
			auto tmp = RAD2DEG(atan(30.0f / velocity));

			if (CheckIfNonValidNumber(tmp) || tmp > 90.0f)
				return 90.0f;

			else if (tmp < 0.0f)
				return 0.0f;
			else
				return tmp;
		};

		if (g::pLocalPlayer->MoveType() != MOVETYPE_WALK)
			return;

		auto velocity = g::pLocalPlayer->Velocity();
		velocity.z = 0.0f;

		auto forwardmove = g::pCmd->forwardmove;
		auto sidemove = g::pCmd->sidemove;

		if (velocity.length_2d() < 5.0f && !forwardmove && !sidemove)
			return;

		static auto flip = false;
		flip = !flip;

		auto turn_direction_modifier = flip ? 1.0f : -1.0f;
		auto viewangles = g::pCmd->viewangles;

		if (forwardmove || sidemove)
		{
			g::pCmd->forwardmove = 0.0f;
			g::pCmd->sidemove = 0.0f;

			auto turn_angle = atan2(-sidemove, forwardmove);
			viewangles.y += turn_angle * M_RADPI;
		}
		else if (forwardmove)
			g::pCmd->forwardmove = 0.0f;

		auto strafe_angle = RAD2DEG(atan(15.0f / velocity.length_2d()));

		if (strafe_angle > 90.0f)
			strafe_angle = 90.0f;
		else if (strafe_angle < 0.0f)
			strafe_angle = 0.0f;

		auto temp = vec3_t(0.0f, viewangles.y - old_yaw, 0.0f);
		temp.y = math::NormalizeYaw(temp.y);

		auto yaw_delta = temp.y;
		old_yaw = viewangles.y;

		auto abs_yaw_delta = fabs(yaw_delta);

		if (abs_yaw_delta <= strafe_angle || abs_yaw_delta >= 30.0f)
		{
			vec3_t velocity_angles;
			math::vector_angles(velocity, velocity_angles);

			temp = vec3_t(0.0f, viewangles.y - velocity_angles.y, 0.0f);
			temp.y = math::NormalizeYaw(temp.y);

			auto velocityangle_yawdelta = temp.y;
			auto velocity_degree = get_velocity_degree(velocity.length_2d());

			if (velocityangle_yawdelta <= velocity_degree || velocity.length_2d() <= 15.0f)
			{
				if (-velocity_degree <= velocityangle_yawdelta || velocity.length_2d() <= 15.0f)
				{
					viewangles.y += strafe_angle * turn_direction_modifier;
					g::pCmd->sidemove = side_speed * turn_direction_modifier;
				}
				else
				{
					viewangles.y = velocity_angles.y - velocity_degree;
					g::pCmd->sidemove = side_speed;
				}
			}
			else
			{
				viewangles.y = velocity_angles.y + velocity_degree;
				g::pCmd->sidemove = -side_speed;
			}
		}
		else if (yaw_delta > 0.0f)
			g::pCmd->sidemove = -side_speed;
		else if (yaw_delta < 0.0f)
			g::pCmd->sidemove = side_speed;

		auto move = vec3_t(g::pCmd->forwardmove, g::pCmd->sidemove, 0.0f);
		auto speed = move.length();

		vec3_t angles_move;
		math::vector_angles(move, angles_move);

		auto normalized_x = fmod(g::pCmd->viewangles.x + 180.0f, 360.0f) - 180.0f;
		auto normalized_y = fmod(g::pCmd->viewangles.y + 180.0f, 360.0f) - 180.0f;

		auto yaw = DEG2RAD(normalized_y - viewangles.y + angles_move.y);

		if (normalized_x >= 90.0f || normalized_x <= -90.0f || g::pCmd->viewangles.x >= 90.0f && g::pCmd->viewangles.x <= 200.0f || g::pCmd->viewangles.x <= -90.0f && g::pCmd->viewangles.x <= 200.0f) //-V648
			g::pCmd->forwardmove = -cos(yaw) * speed;
		else
			g::pCmd->forwardmove = cos(yaw) * speed;

		g::pCmd->sidemove = sin(yaw) * speed;
	}
	else if (config.auto_strafe)
	{

		if (g::pCmd->buttons & IN_MOVELEFT ||
			g::pCmd->buttons & IN_MOVERIGHT ||
			g::pCmd->buttons & IN_BACK)
			return;
		
		static auto old_yaw = 0.0f;

		auto get_velocity_degree = [](float velocity)
		{
			auto tmp = RAD2DEG(atan(30.0f / velocity));

			if (CheckIfNonValidNumber(tmp) || tmp > 90.0f)
				return 90.0f;

			else if (tmp < 0.0f)
				return 0.0f;
			else
				return tmp;
		};

		if (g::pLocalPlayer->MoveType() != MOVETYPE_WALK)
			return;

		auto velocity = g::pLocalPlayer->Velocity();
		velocity.z = 0;

		static auto flip = false;
		flip = !flip;

		auto turn_direction_modifier = flip ? 1.0f : -1.0f;
		auto viewangles = g::pCmd->viewangles;

		auto strafe_angle = RAD2DEG(atan(15.0f / velocity.length_2d()));

		if (strafe_angle > 90.0f)
			strafe_angle = 90.0f;
		else if (strafe_angle < 0.0f)
			strafe_angle = 0.0f;

		auto temp = vec3_t(0.0f, viewangles.y - old_yaw, 0.0f);
		temp.y = math::NormalizeYaw(temp.y);

		auto yaw_delta = temp.y;
		old_yaw = viewangles.y;

		auto abs_yaw_delta = fabs(yaw_delta);

		if (abs_yaw_delta <= strafe_angle || abs_yaw_delta >= 30.0f)
		{
			vec3_t velocity_angles;
			math::vector_angles(velocity, velocity_angles);

			temp = vec3_t(0.0f, viewangles.y - velocity_angles.y, 0.0f);
			temp.y = math::NormalizeYaw(temp.y);

			auto velocityangle_yawdelta = temp.y;
			auto velocity_degree = get_velocity_degree(velocity.length_2d());

			if (velocityangle_yawdelta <= velocity_degree || velocity.length_2d() <= 15.0f)
			{
				if (-velocity_degree <= velocityangle_yawdelta || velocity.length_2d() <= 15.0f)
				{
					viewangles.y += strafe_angle * turn_direction_modifier;
					g::pCmd->sidemove = side_speed * turn_direction_modifier;
				}
				else
				{
					viewangles.y = velocity_angles.y - velocity_degree;
					g::pCmd->sidemove = side_speed;
				}
			}
			else
			{
				viewangles.y = velocity_angles.y + velocity_degree;
				g::pCmd->sidemove = -side_speed;
			}
		}
		else if (yaw_delta > 0.0f)
			g::pCmd->sidemove = -side_speed;
		else if (yaw_delta < 0.0f)
			g::pCmd->sidemove = side_speed;

		auto move = vec3_t(g::pCmd->forwardmove, g::pCmd->sidemove, 0.0f);
		auto speed = move.length();

		vec3_t angles_move;
		math::vector_angles(move, angles_move);

		auto normalized_x = fmod(g::pCmd->viewangles.x + 180.0f, 360.0f) - 180.0f;
		auto normalized_y = fmod(g::pCmd->viewangles.y + 180.0f, 360.0f) - 180.0f;

		auto yaw = DEG2RAD(normalized_y - viewangles.y + angles_move.y);

		if (normalized_x >= 90.0f || normalized_x <= -90.0f || g::pCmd->viewangles.x >= 90.0f && g::pCmd->viewangles.x <= 200.0f || g::pCmd->viewangles.x <= -90.0f && g::pCmd->viewangles.x <= 200.0f) //-V648
			g::pCmd->forwardmove = -cos(yaw) * speed;
		else
			g::pCmd->forwardmove = cos(yaw) * speed;

		g::pCmd->sidemove = sin(yaw) * speed;
	}

}


void misc::InfiniteDuck()
{
	if (!config.infinite_duck)
		return;

	g::pCmd->buttons |= IN_BULLRUSH;
}

void misc::EdgeJump(int iFlags)
{
	static bool bToggled = false;
	if (!config.edgejump ||
		!g_keyhandler.CheckKey(config.edgejumpkey, config.edgejumpkey_style))
		return;

	if (!(g::pLocalPlayer->Flags() & FL_ONGROUND) &&
		(iFlags & FL_ONGROUND))
		g::pCmd->buttons |= IN_JUMP;
}


void misc::JumpBug(int unFlags, int preFlags)
{
	if (config.jumpbug && !(unFlags & (1 << 0)) && preFlags & (1 << 0) && g_keyhandler.CheckKey(config.jumpbugkey, config.jumpbugkey_style))
	{
		g::pCmd->buttons |= (IN_DUCK);
	}

	if (config.jumpbug)
	{
		if (g_keyhandler.CheckKey(config.jumpbugkey, config.jumpbugkey_style))
		{
			if (preFlags & FL_ONGROUND) {
				g::pCmd->buttons &= ~IN_JUMP;
				misc::m_bIsJumpbugging = true;
			}
			else
			{
				misc::m_bIsJumpbugging = false;
			}
		}
	}

}

void misc::DisableInterpolation()
{
	for (auto entity : entity_cache::Groups[CGroupType::PLAYERS_ENEMIES])
	{
		if (!entity || !entity->IsAlive())
			continue;

		if (entity->Dormant())
			continue;

		auto varmap = reinterpret_cast<uintptr_t>(entity) + 36;
		auto varmap_size = *reinterpret_cast<int*>(varmap + 20);
		for (auto index = 0; index < varmap_size; index++)
			*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(varmap) + index * 12) = 0;
	}
}