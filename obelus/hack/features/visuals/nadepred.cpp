#include "../features.hpp"

class CTraceFilterSimple_game {
public:
	void* m_vmt;
	const BaseEntity* m_pass_ent1;
	int             m_collision_group;
	ShouldHitFunc_t m_shouldhit_check_fn;

public:
	__forceinline CTraceFilterSimple_game() :
		m_vmt{ g::CTraceFilterSimple_vmt.as< void* >() },
		m_pass_ent1{ },
		m_collision_group{ },
		m_shouldhit_check_fn{ } {}

	__forceinline CTraceFilterSimple_game(const BaseEntity* pass_ent1, int collision_group = 0, ShouldHitFunc_t shouldhit_check_fn = nullptr) :
		m_vmt{ g::CTraceFilterSimple_vmt.as< void* >() },
		m_pass_ent1{ pass_ent1 },
		m_collision_group{ collision_group },
		m_shouldhit_check_fn{ shouldhit_check_fn } {}

	__forceinline bool ShouldHitEntity(BaseEntity* entity, int contents_mask) {

		void* real_vmt = *(void**)m_vmt;

		return utilities::GetMethod< bool(__thiscall*)(void*, BaseEntity*, int) >(real_vmt, 0)(real_vmt, entity, contents_mask);
	}

	__forceinline void SetPassEntity(BaseEntity* pass_ent1) {
		m_pass_ent1 = pass_ent1;

	}

	__forceinline void SetCollisionGroup(int collision_group) {
		m_collision_group = collision_group;
	}
};

float scale(BaseEntity* player, float damage, float armor_ratio, int hitgroup) {
	bool  has_heavy_armor;
	int   armor;
	float heavy_ratio, bonus_ratio, ratio, new_damage;

	static auto is_armored = [](BaseEntity* player, int armor, int hitgroup) {
		if (armor <= 0)
			return false;

		if (hitgroup == hitgroup_head && player->HasHelmet())
			return true;

		else if (hitgroup >= hitgroup_chest && hitgroup <= hitgroup_rightarm)
			return true;

		return false;
	};

	has_heavy_armor = player->HasHeavyArmor();

	switch (hitgroup) {
	case hitgroup_head:
		if (has_heavy_armor)
			damage = (damage * 4.f) * 0.5f;
		else
			damage *= 4.f;
		break;

	case hitgroup_stomach:
		damage *= 1.25f;
		break;

	case hitgroup_leftleg:
	case hitgroup_rightleg:
		damage *= 0.75f;
		break;

	default:
		break;
	}

	armor = player->Armor();

	if (is_armored(player, armor, hitgroup)) {
		heavy_ratio = 1.f;
		bonus_ratio = 0.5f;
		ratio = armor_ratio * 0.5f;

		if (has_heavy_armor) {
			bonus_ratio = 0.33f;
			ratio = armor_ratio * 0.25f;
			heavy_ratio = 0.33f;
			new_damage = (damage * ratio) * 0.85f;
		}

		else
			new_damage = damage * ratio;

		if (((damage - new_damage) * (heavy_ratio * bonus_ratio)) > armor)
			new_damage = damage - (armor / bonus_ratio);

		damage = new_damage;
	}

	return std::floor(damage);
}
void grenades::reset() {
	m_start = vec3_t{ };
	m_move = vec3_t{ };
	m_velocity = vec3_t{ };
	m_vel = 0.f;
	m_power = 0.f;

	m_path.clear();
	m_bounces.clear();
}
__forceinline bool IsValidTarget(BaseEntity* player) {
	if (!player)
		return false;

	if (!player->IsPlayer())
		return false;

	if (!player->IsAlive())
		return false;

	if (player == g::pLocalPlayer)
		return false;

	if (!player->Enemy(g::pLocalPlayer))
		return false;

	return true;
}
void grenades::PaintNadePred() {
	static CTraceFilterSimple_game filter{ };
	trace_t	                   trace;
	std::pair< float, BaseEntity* >    target{ 0.f, nullptr };

	if (!config.nadepred)
		return;

	if (!g::pLocalPlayer->IsAlive())
		return;

	if (m_path.size() < 2)
		return;

	filter.SetPassEntity(g::pLocalPlayer);

	vec3_t prev = m_path.front();

	for (const auto& cur : m_path) {
		vec2_t screen0, screen1;

		if (render::WorldToScreen(prev, screen0) && render::WorldToScreen(cur, screen1))
			render::line(screen0, screen1, g_menu.ConvertColor(config.nadepred_col));
		prev = cur;
	}

	Color dumbo_dagadt;

	for (auto player : entity_cache::Groups[CGroupType::PLAYERS_ENEMIES])
	{
		if (!IsValidTarget(player))
			continue;

		vec3_t center = player->WorldSpaceCenter();

		vec3_t delta = center - prev;

		if (m_id == WEAPON_HEGRENADE) {

			if (delta.length() > 350.f)
				continue;

			interfaces::trace_ray->trace_ray(ray_t(prev, center), MASK_SHOT, (trace_filter*)&filter, &trace);

			if (!trace.entity || trace.entity != player)
				continue;

			float d = (delta.length() - 25.f) / 140.f;
			float damage = 105.f * std::exp(-d * d);

			damage = scale(player, damage, 1.f, hitgroup_chest);

			damage = std::min(damage, (player->Armor() > 0) ? 57.f : 98.f);

			// idiota mocskos dagadt kurva anyjat dumbonak
			if (player->Health() < damage)
				dumbo_dagadt = Color(50, 255, 50, 0xb4);
			else
				dumbo_dagadt = Color(255, 255, 255, 0xb4);

			if (damage > target.first) {
				target.first = damage;
				target.second = player;
			}
		}
	}

	if (target.second) {
		vec2_t screen;

		if (!m_bounces.empty())
			m_bounces.back().color = { 0, 255, 0, 255 };

		if (render::WorldToScreen(prev, screen))
			render::esp.string(screen.x, screen.y + 5, dumbo_dagadt, tfm::format("%i", (int)target.first) + " dmg", render::ALIGN_CENTER);
	}

	for (const auto& b : m_bounces) {
		vec2_t screen;

		if (render::WorldToScreen(b.point, screen)) {
			render::rect_filled(screen.x - 2, screen.y - 2, 4, 4, b.color);
		}
	}
}

void grenades::NadePred() {
	bool attack, attack2;

	reset();

	if (!config.nadepred)
		return;

	if (!g::pLocalPlayer->IsAlive() || !g::pLocalWeapon || !g::pLocalWeaponData)
		return;

	if (g::pLocalWeapon && !g::pLocalWeapon->IsGrenade())
		return;

	attack = (g::pCmd->buttons & IN_ATTACK);
	attack2 = (g::pCmd->buttons & IN_ATTACK2);

	if (!attack && !attack2)
		return;

	m_id = g::pLocalWeapon->ItemDefinitionIndex();
	m_power = g::pLocalWeapon->ThrowStrength();
	m_vel = g::pLocalWeaponData->weapon_throw_velocity;

	simulate();
}

void grenades::simulate() {
	setup();

	size_t step = (size_t)utilities::TIME_TO_TICKS(0.05f), timer{ 0u };

	for (size_t i{ 0u }; i < 4096u; ++i) {

		if (!timer)
			m_path.push_back(m_start);

		size_t flags = advance(i);


		if ((flags & DETONATE))
			break;

		if ((flags & BOUNCE) || timer >= step)
			timer = 0;

		else
			++timer;

		if (m_velocity == vec3_t{ })
			break;
	}

	if (m_id == WEAPON_MOLOTOV || m_id == WEAPON_INCGRENADE) {
		trace_t trace;
		PhysicsPushEntity(m_start, { 0.f, 0.f, -131.f }, trace, g::pLocalPlayer);

		if (trace.flFraction < 0.9f)
			m_start = trace.end;
	}

	m_path.push_back(m_start);
	m_bounces.push_back({ m_start, colors::red });
}

void grenades::setup() {
	vec3_t angle = g::pCmd->viewangles;

	float pitch = angle.x;

	// correct the pitch.
	if (pitch < -90.f)
		pitch += 360.f;

	else if (pitch > 90.f)
		pitch -= 360.f;

	angle.x = pitch - (90.f - std::abs(pitch)) * 10.f / 90.f;

	float vel = m_vel * 0.9f;

	math::clamp(vel, 15.f, 750.f);

	vel *= ((m_power * 0.7f) + 0.3f);


	vec3_t forward;
	math::angle_vectors(angle, forward);

	m_start = g::pLocalPlayer->EyePos();

	m_start.z += (m_power * 12.f) - 12.f;


	vec3_t end = m_start + (forward * 22.f);

	trace_t trace;
	TraceHull(m_start, end, trace, g::pLocalPlayer);

	m_start = trace.end - (forward * 6.f);


	m_velocity = g::pLocalPlayer->Velocity();
	m_velocity *= 1.25f;
	m_velocity += (forward * vel);
}

size_t grenades::advance(size_t tick) {
	size_t     flags{ NONE };
	trace_t trace;

	PhysicsAddGravityMove(m_move);

	PhysicsPushEntity(m_start, m_move, trace, g::pLocalPlayer);

	if (detonate(tick, trace))
		flags |= DETONATE;


	if (trace.flFraction != 1.f) {

		flags |= BOUNCE;


		ResolveFlyCollisionBounce(trace);
	}


	m_start = trace.end;

	return flags;
}

bool grenades::detonate(size_t tick, trace_t& trace) {
	// convert current simulation tick to time.
	float time = utilities::TICKS_TO_TIME(tick);

	// CSmokeGrenadeProjectile::Think_Detonate
	// speed <= 0.1
	// checked every 0.2s

	// CDecoyProjectile::Think_Detonate
	// speed <= 0.2
	// checked every 0.2s

	// CBaseCSGrenadeProjectile::SetDetonateTimerLength
	// auto detonate at 1.5s
	// checked every 0.2s
	static auto cv = interfaces::console->FindVar("weapon_molotov_maxdetonateslope");
	switch (m_id) {
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
		return time >= 1.5f && !(tick % utilities::TIME_TO_TICKS(0.2f));

	case WEAPON_SMOKEGRENADE:
		return m_velocity.length() <= 0.1f && !(tick % utilities::TIME_TO_TICKS(0.2f));

	case WEAPON_DECOY:
		return m_velocity.length() <= 0.2f && !(tick % utilities::TIME_TO_TICKS(0.2f));

	case WEAPON_MOLOTOV:
	case WEAPON_FIREBOMB:
		// detonate when hitting the floor.
		if (trace.flFraction != 1.f && (std::cos(DEG2RAD(cv->GetFloat())) <= trace.plane.normal.z))
			return true;

		// detonate if we have traveled for too long.
		// checked every 0.1s
		return time >= cv->GetFloat() && !(tick % utilities::TIME_TO_TICKS(0.1f));

	default:
		return false;
	}

	return false;
}


bool IsBreakable(BaseEntity* pEntity)
{
	static auto	ptr_instruction = pattern::Scan(XOR("client.dll"), XOR("55 8B EC 51 56 8B F1 85 F6 74 68 83 BE"));

	using Fn = bool(__fastcall*)(BaseEntity*);
	static auto fn = reinterpret_cast<Fn>(ptr_instruction);

	if (!pEntity || !pEntity->Index()) {
		return false;
	}


	auto take_damage{ (char*)((uintptr_t)pEntity + *(size_t*)((uintptr_t)fn + 38)) };
	auto take_damage_backup{ *take_damage };

	auto* cclass = interfaces::client->get_client_classes();


	if ((cclass->m_pNetworkName[1] == 'B' && cclass->m_pNetworkName[9] == 'e' && cclass->m_pNetworkName[10] == 'S' && cclass->m_pNetworkName[16] == 'e')
		|| (cclass->m_pNetworkName[1] != 'B' || cclass->m_pNetworkName[5] != 'D'))
		*take_damage = 2;

	bool breakable = fn(pEntity);
	*take_damage = take_damage_backup;

	return breakable;
}

void grenades::ResolveFlyCollisionBounce(trace_t& trace) {

	float surface = 1.f;

	if (trace.entity) {
		if (IsBreakable(trace.entity)) {
			if (!trace.entity->is(FNV1A::Hash("CFuncBrush")) &&
				!trace.entity->is(FNV1A::Hash("CBaseDoor")) &&
				!trace.entity->is(FNV1A::Hash("CCSPlayer")) &&
				!trace.entity->is(FNV1A::Hash("CBaseEntity"))) {
				PhysicsPushEntity(m_start, m_move, trace, trace.entity);
				m_velocity *= 0.4f;
				return;
			}
		}
	}

	float elasticity = 0.45f * surface;

	math::clamp(elasticity, 0.f, 0.9f);

	vec3_t velocity;
	PhysicsClipVelocity(m_velocity, trace.plane.normal, velocity, 2.f);
	velocity *= elasticity;

	if (trace.plane.normal.z > 0.7f) {
		float speed = velocity.length_sqr();

		if (speed > 96000.f) {

			float len = velocity.normalized().dot(trace.plane.normal);
			if (len > 0.5f)
				velocity *= 1.5f - len;
		}

		if (speed < 400.f)
			m_velocity = vec3_t{ };

		else {
			m_velocity = velocity;

			float left = 1.f - trace.flFraction;

			PhysicsPushEntity(trace.end, velocity * (left * interfaces::globals->interval_per_tick), trace, g::pLocalPlayer);
		}
	}

	else {
		m_velocity = velocity;
		float left = 1.f - trace.flFraction;
		PhysicsPushEntity(trace.end, velocity * (left * interfaces::globals->interval_per_tick), trace, g::pLocalPlayer);
	}

	m_bounces.push_back({ trace.end, colors::white });
}

void grenades::PhysicsPushEntity(vec3_t& start, const vec3_t& move, trace_t& trace, BaseEntity* ent) {
	vec3_t end = start + move;
	TraceHull(start, end, trace, ent);
}

void grenades::TraceHull(vec3_t& start, vec3_t& end, trace_t& trace, BaseEntity* ent) {
	static CTraceFilterSimple_game filter{ };

	filter.SetPassEntity(ent);
	interfaces::trace_ray->trace_ray(ray_t(start, end, { -2.f, -2.f, -2.f }, { 2.f, 2.f, 2.f }), MASK_SOLID, (trace_filter*)&filter, &trace);
}

void grenades::PhysicsAddGravityMove(vec3_t& move) {

	float gravity = 800.f * 0.4f;

	move.x = m_velocity.x * interfaces::globals->interval_per_tick;
	move.y = m_velocity.y * interfaces::globals->interval_per_tick;

	float z = m_velocity.z - (gravity * interfaces::globals->interval_per_tick);
	move.z = ((m_velocity.z + z) / 2.f) * interfaces::globals->interval_per_tick;
	m_velocity.z = z;
}

void grenades::PhysicsClipVelocity(const vec3_t& in, const vec3_t& normal, vec3_t& out, float overbounce) {

	constexpr float STOP_EPSILON = 0.1f;

	float backoff = in.dot(normal) * overbounce;

	for (int i{ }; i < 3; ++i) {
		out[i] = in[i] - (normal[i] * backoff);

		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
			out[i] = 0.f;
	}
}