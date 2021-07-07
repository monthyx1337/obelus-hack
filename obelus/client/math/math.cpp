#include "../utilities/csgo.hpp"
#include "../../hack/features/visuals/esp.h"


bool math::GetBoundingBox(BaseEntity* entity, bbox_t& box)
{
	const auto collideable = entity->Collideables();

	if (collideable == nullptr)
		return false;
	vec3_t top, down, air, s[2];

	vec3_t adjust = vec3_t(0, 0, -15) * entity->DuckAmount();

	if (!(entity->Flags() & FL_ONGROUND) && (entity->MoveType() != MOVETYPE_LADDER))
		air = vec3_t(0, 0, 10);
	else
		air = vec3_t(0, 0, 0);

	down = entity->AbsOrigin() + air;
	top = down + vec3_t(0, 0, 72) + adjust;

	if (visuals::WorldToScreen(top, s[1]) && visuals::WorldToScreen(down, s[0]))
	{
		vec3_t delta = s[1] - s[0];

		box.h = fabsf(delta.y) + 6;
		box.w = box.h / 2 + 5;

		box.x = s[1].x - (box.w / 2) + 2;
		box.y = s[1].y - 1;

		return true;
	}

	return false;
}
vec3_t math::calculate_angle(const vec3_t& source, const vec3_t& destination, const vec3_t& viewAngles) {
	vec3_t delta = source - destination;
	auto radians_to_degrees = [](float radians) { return radians * 180 / static_cast<float>(M_PI); };
	vec3_t angles;
	angles.x = radians_to_degrees(atanf(delta.z / std::hypotf(delta.x, delta.y))) - viewAngles.x;
	angles.y = radians_to_degrees(atanf(delta.y / delta.x)) - viewAngles.y;
	angles.z = 0.0f;

	if (delta.x >= 0.0)
		angles.y += 180.0f;

	angles.normalize_aimbot();
	return angles;
}
void math::FixMove(UserCmd* m_pcmd, vec3_t wish_angle) {
	vec3_t view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
	auto viewangles = m_pcmd->viewangles;
	viewangles.normalized();

	math::angle_vectors(wish_angle, &view_fwd, &view_right, &view_up);
	math::angle_vectors(viewangles, &cmd_fwd, &cmd_right, &cmd_up);

	float v8 = sqrtf((view_fwd.x * view_fwd.x) + (view_fwd.y * view_fwd.y));
	float v10 = sqrtf((view_right.x * view_right.x) + (view_right.y * view_right.y));
	float v12 = sqrtf(view_up.z * view_up.z);

	vec3_t norm_view_fwd((1.f / v8) * view_fwd.x, (1.f / v8) * view_fwd.y, 0.f);
	vec3_t norm_view_right((1.f / v10) * view_right.x, (1.f / v10) * view_right.y, 0.f);
	vec3_t norm_view_up(0.f, 0.f, (1.f / v12) * view_up.z);

	float v14 = sqrtf((cmd_fwd.x * cmd_fwd.x) + (cmd_fwd.y * cmd_fwd.y));
	float v16 = sqrtf((cmd_right.x * cmd_right.x) + (cmd_right.y * cmd_right.y));
	float v18 = sqrtf(cmd_up.z * cmd_up.z);

	vec3_t norm_cmd_fwd((1.f / v14) * cmd_fwd.x, (1.f / v14) * cmd_fwd.y, 0.f);
	vec3_t norm_cmd_right((1.f / v16) * cmd_right.x, (1.f / v16) * cmd_right.y, 0.f);
	vec3_t norm_cmd_up(0.f, 0.f, (1.f / v18) * cmd_up.z);

	float v22 = norm_view_fwd.x * m_pcmd->forwardmove;
	float v26 = norm_view_fwd.y * m_pcmd->forwardmove;
	float v28 = norm_view_fwd.z * m_pcmd->forwardmove;
	float v24 = norm_view_right.x * m_pcmd->sidemove;
	float v23 = norm_view_right.y * m_pcmd->sidemove;
	float v25 = norm_view_right.z * m_pcmd->sidemove;
	float v30 = norm_view_up.x * m_pcmd->upmove;
	float v27 = norm_view_up.z * m_pcmd->upmove;
	float v29 = norm_view_up.y * m_pcmd->upmove;

	m_pcmd->forwardmove = ((((norm_cmd_fwd.x * v24) + (norm_cmd_fwd.y * v23)) + (norm_cmd_fwd.z * v25))
		+ (((norm_cmd_fwd.x * v22) + (norm_cmd_fwd.y * v26)) + (norm_cmd_fwd.z * v28)))
		+ (((norm_cmd_fwd.y * v30) + (norm_cmd_fwd.x * v29)) + (norm_cmd_fwd.z * v27));
	m_pcmd->sidemove = ((((norm_cmd_right.x * v24) + (norm_cmd_right.y * v23)) + (norm_cmd_right.z * v25))
		+ (((norm_cmd_right.x * v22) + (norm_cmd_right.y * v26)) + (norm_cmd_right.z * v28)))
		+ (((norm_cmd_right.x * v29) + (norm_cmd_right.y * v30)) + (norm_cmd_right.z * v27));
	m_pcmd->upmove = ((((norm_cmd_up.x * v23) + (norm_cmd_up.y * v24)) + (norm_cmd_up.z * v25))
		+ (((norm_cmd_up.x * v26) + (norm_cmd_up.y * v22)) + (norm_cmd_up.z * v28)))
		+ (((norm_cmd_up.x * v30) + (norm_cmd_up.y * v29)) + (norm_cmd_up.z * v27));

	static auto cl_forwardspeed = interfaces::console->FindVar(XOR("cl_forwardspeed"));
	static auto cl_sidespeed = interfaces::console->FindVar(XOR("cl_sidespeed"));
	static auto cl_upspeed = interfaces::console->FindVar(XOR("cl_upspeed"));

	m_pcmd->forwardmove = std::clamp(m_pcmd->forwardmove, -cl_forwardspeed->GetFloat(), cl_forwardspeed->GetFloat());
	m_pcmd->sidemove = std::clamp(m_pcmd->sidemove, -cl_sidespeed->GetFloat(), cl_sidespeed->GetFloat());
	m_pcmd->upmove = std::clamp(m_pcmd->upmove, -cl_upspeed->GetFloat(), cl_upspeed->GetFloat());
}

vec3_t math::calculate_angle(vec3_t& a, vec3_t& b) {
	vec3_t angles;
	vec3_t delta;
	delta.x = (a.x - b.x);
	delta.y = (a.y - b.y);
	delta.z = (a.z - b.z);

	double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
	angles.x = (float)(atanf(delta.z / hyp) * 57.295779513082f);
	angles.y = (float)(atanf(delta.y / delta.x) * 57.295779513082f);

	angles.z = 0.0f;
	if (delta.x >= 0.0) { angles.y += 180.0f; }
	return angles;
}

void math::sin_cos(float r, float* s, float* c) {
	*s = sin(r);
	*c = cos(r);
}

vec3_t math::angle_vector(vec3_t angle) {
	auto sy = sin(angle.y / 180.f * static_cast<float>(M_PI));
	auto cy = cos(angle.y / 180.f * static_cast<float>(M_PI));

	auto sp = sin(angle.x / 180.f * static_cast<float>(M_PI));
	auto cp = cos(angle.x / 180.f * static_cast<float>(M_PI));

	return vec3_t(cp * cy, cp * sy, -sp);
}

void math::transform_vector(vec3_t & a, matrix_t & b, vec3_t & out) {
	out.x = a.dot(b.mat_val[0]) + b.mat_val[0][3];
	out.y = a.dot(b.mat_val[1]) + b.mat_val[1][3];
	out.z = a.dot(b.mat_val[2]) + b.mat_val[2][3];
}

void math::vector_angles(vec3_t & forward, vec3_t & angles) {
	vec3_t view;

	if (!forward[0] && !forward[1])
	{
		view[0] = 0.0f;
		view[1] = 0.0f;
	}
	else
	{
		view[1] = atan2(forward[1], forward[0]) * 180.0f / M_PI;

		if (view[1] < 0.0f)
			view[1] += 360.0f;

		view[2] = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		view[0] = atan2(forward[2], view[2]) * 180.0f / M_PI;
	}

	angles[0] = -view[0];
	angles[1] = view[1];
	angles[2] = 0.f;
}
void math::angle_vectors(const vec3_t& angles, vec3_t& forward)
{
	float sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}
void math::angle_vectors(vec3_t & angles, vec3_t * forward, vec3_t * right, vec3_t * up) {
	float sp, sy, sr, cp, cy, cr;

	sin_cos(DEG2RAD(angles.x), &sp, &cp);
	sin_cos(DEG2RAD(angles.y), &sy, &cy);
	sin_cos(DEG2RAD(angles.z), &sr, &cr);

	if (forward) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up) {
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

vec3_t math::vector_add(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x + b.x,
		a.y + b.y,
		a.z + b.z);
}
void math::angle_matrix(const vec3_t& ang, const vec3_t& pos, matrix_t& out) {
	g::AngleMatrix(ang, out);
	out.set_origin(pos);
}

vec3_t math::vector_subtract(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x - b.x,
		a.y - b.y,
		a.z - b.z);
}

vec3_t math::vector_multiply(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x * b.x,
		a.y * b.y,
		a.z * b.z);
}

vec3_t math::vector_divide(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x / b.x,
		a.y / b.y,
		a.z / b.z);
}

bool math::screen_transform(const vec3_t & point, vec3_t & screen) {
	auto matrix = interfaces::engine->world_to_screen_matrix();

	float w = matrix[3][0] * point.x + matrix[3][1] * point.y + matrix[3][2] * point.z + matrix[3][3];
	screen.x = matrix[0][0] * point.x + matrix[0][1] * point.y + matrix[0][2] * point.z + matrix[0][3];
	screen.y = matrix[1][0] * point.x + matrix[1][1] * point.y + matrix[1][2] * point.z + matrix[1][3];
	screen.z = 0.0f;

	int inverse_width = static_cast<int>((w < 0.001f) ? -1.0f / w :
		1.0f / w);

	screen.x *= inverse_width;
	screen.y *= inverse_width;
	return (w < 0.001f);
}

bool math::world_to_screen(const vec3_t & origin, vec2_t & screen) {
	static std::uintptr_t view_matrix;
	if ( !view_matrix )
		view_matrix = *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( pattern::Scan( XOR("client.dll"), XOR("0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9") ) ) + 3 ) + 176;

	const auto& matrix = *reinterpret_cast< view_matrix_t* >( view_matrix );

	const auto w = matrix.m[ 3 ][ 0 ] * origin.x + matrix.m[ 3 ][ 1 ] * origin.y + matrix.m[ 3 ][ 2 ] * origin.z + matrix.m[ 3 ][ 3 ];
	if ( w < 0.001f )
		return false;

	int x, y;
	interfaces::engine->GetScreenSize( x, y );
	
	screen.x = static_cast<float>(x) / 2.0f;
	screen.y = static_cast<float>(y) / 2.0f;

	screen.x *= 1.0f + ( matrix.m[ 0 ][ 0 ] * origin.x + matrix.m[ 0 ][ 1 ] * origin.y + matrix.m[ 0 ][ 2 ] * origin.z + matrix.m[ 0 ][ 3 ] ) / w;
	screen.y *= 1.0f - ( matrix.m[ 1 ][ 0 ] * origin.x + matrix.m[ 1 ][ 1 ] * origin.y + matrix.m[ 1 ][ 2 ] * origin.z + matrix.m[ 1 ][ 3 ] ) / w;

	return true;
}
