#pragma once

struct bbox_t
{
	float x = 0.0f;
	float y = 0.0f;
	float w = 0.0f;
	float h = 0.0f;
};

namespace math {
	
	constexpr float pi = 3.1415926535897932384f; 
	constexpr float pi_2 = pi * 2.f;             

	void FixMove(UserCmd* cmd, vec3_t old_angles);
	vec3_t calculate_angle(vec3_t& a, vec3_t& b);
	vec3_t calculate_angle(const vec3_t& source, const vec3_t& destination, const vec3_t& viewAngles);
	void sin_cos(float r, float* s, float* c);
	vec3_t angle_vector(vec3_t angle);
	void transform_vector(vec3_t&, matrix_t&, vec3_t&);
	void vector_angles(vec3_t&, vec3_t&);
	void angle_vectors(vec3_t&, vec3_t*, vec3_t*, vec3_t*);
	void angle_vectors(const vec3_t&, vec3_t&);
	vec3_t vector_add(vec3_t&, vec3_t&);
	vec3_t vector_subtract(vec3_t&, vec3_t&);
	vec3_t vector_multiply(vec3_t&, vec3_t&);
	vec3_t vector_divide(vec3_t&, vec3_t&);
	bool screen_transform(const vec3_t& point, vec3_t& screen);
	bool world_to_screen(const vec3_t& origin, vec2_t& screen);
	bool GetBoundingBox(BaseEntity* entity, bbox_t& box);
	void angle_matrix(const vec3_t& ang, const vec3_t& pos, matrix_t& out);
	inline float DistancePointToLine(vec3_t Point, vec3_t LineOrigin, vec3_t Dir)
	{
		auto PointDir = Point - LineOrigin;

		auto TempOffset = PointDir.dot(Dir) / (Dir.x * Dir.x + Dir.y * Dir.y + Dir.z * Dir.z);
		if (TempOffset < 0.000001f)
			return FLT_MAX;

		auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

		return (Point - PerpendicularPoint).length();
	}

	__forceinline float NormalizeYaw(float f) 
	{
		while (f < -180.0f)
			f += 360.0f;

		while (f > 180.0f)
			f -= 360.0f;

		return f;
	}

	__forceinline void NormalizeAngle(float& angle) 
	{
		float rot;

		if (!std::isfinite(angle)) {
			angle = 0.f;
			return;
		}

		if (angle >= -180.f && angle <= 180.f)
			return;

		rot = std::round(std::abs(angle / 360.f));

		angle = (angle < 0.f) ? angle + (360.f * rot) : angle - (360.f * rot);
	}

	__forceinline float NormalizedAngle(float angle) 
	{
		NormalizeAngle(angle);
		return angle;
	}

	__forceinline float blend(float a, float b, float multiplier) 
	{
		return a + static_cast<int>(multiplier * (b - a));
	}

	template < typename t >
	__forceinline void clamp(t& n, const t& lower, const t& upper) 
	{
		n = std::max(lower, std::min(n, upper));
	}
};


namespace random
{
	constexpr auto time = __TIME__;
	constexpr auto seed = static_cast<unsigned>(time[7]) + static_cast<unsigned>(time[6]) * 10 + static_cast<unsigned>(time[4]) * 60 + static_cast<unsigned>(time[3]) * 600 + static_cast<unsigned>(time[1]) * 3600 + static_cast<unsigned>(time[0]) * 36000;

	template <int n>
	struct gen
	{
	private:
		static constexpr unsigned a = 16807;
		static constexpr unsigned m = 2147483647;

		static constexpr unsigned s = gen<n - 1>::value;
		static constexpr unsigned lo = a * (s & 0xFFFFu);
		static constexpr unsigned hi = a * (s >> 16u);
		static constexpr unsigned lo2 = lo + ((hi & 0x7FFFu) << 16u);
		static constexpr unsigned hi2 = hi >> 15u;
		static constexpr unsigned lo3 = lo2 + hi;

	public:
		static constexpr unsigned max = m;
		static constexpr unsigned value = lo3 > m ? lo3 - m : lo3;
	};

	template <>
	struct gen<0>
	{
		static constexpr unsigned value = seed;
	};

	template <int n, int m>
	struct _int
	{
		static constexpr auto value = gen<n + 1>::value % m;
	};

	template <int n>
	struct _char
	{
		static const char value = static_cast<char>(1 + _int<n, 0x7F - 1>::value);
	};
}
