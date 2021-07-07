#pragma once

namespace grenades 
{
	enum GrenadeFlags : size_t {
		NONE = 0,
		DETONATE,
		BOUNCE,
	};

	struct bounce_t {
		vec3_t point;
		Color  color;
	};

	using path_t = std::vector< vec3_t >;
	using bounces_t = std::vector< bounce_t >;

	inline int       m_id;
	inline path_t    m_path;
	inline bounces_t m_bounces;
	inline float     m_vel, m_power;
	inline vec3_t    m_start, m_velocity, m_move;

	void   reset();
	void   PaintNadePred();
	void   NadePred();
	void   simulate();
	void   setup();
	size_t advance(size_t tick);
	bool   detonate(size_t tick, trace_t& trace);
	void   ResolveFlyCollisionBounce(trace_t& trace);
	void   PhysicsPushEntity(vec3_t& start, const vec3_t& move, trace_t& trace, BaseEntity* ent);
	void   TraceHull(vec3_t& start, vec3_t& end, trace_t& trace, BaseEntity* ent);
	void   PhysicsAddGravityMove(vec3_t& move);
	void   PhysicsClipVelocity(const vec3_t& in, const vec3_t& normal, vec3_t& out, float overbounce);
};
