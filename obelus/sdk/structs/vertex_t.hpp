#pragma once
#include "../math/vector2d.hpp"

class vertex_t {
public:
	vec2_t m_pos;
	vec2_t m_coord;

public:
	__forceinline vertex_t() {}

	__forceinline vertex_t(float x, float y) : m_pos{ x, y }, m_coord{ 0.f, 0.f } {}
	__forceinline vertex_t(const vec2_t& pos) : m_pos{ pos }, m_coord{ 0.f, 0.f } {}
	__forceinline vertex_t(const vec2_t& pos, const vec2_t& coord) : m_pos{ pos }, m_coord{ coord } {}

	__forceinline void init(const vec2_t& pos, const vec2_t& coord = { 0, 0 }) {
		m_pos = pos;
		m_coord = coord;
	}
};