#pragma once

enum font_flags {
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

using ulong_t = unsigned long;
using HFont = ulong_t;
struct Point {
	int x;
	int y;
};

struct Rect {
	int x;
	int y;
	int w;
	int h;
};

namespace render {
	struct FontSize_t {
		int iWidth;
		int iHeight;
	};

	enum StringFlags_t {
		ALIGN_LEFT = 0,
		ALIGN_RIGHT,
		ALIGN_CENTER
	};

	class Font {
	public:
		HFont      m_handle;
		FontSize_t m_size;

	public:
		__forceinline Font() : m_handle{}, m_size{} {};

		// ctor.
		__forceinline Font(const std::string& name, int s, int w, int flags) {
			m_handle = interfaces::surface->font_create();
			interfaces::surface->set_font_glyph(m_handle, name.data(), s, w, 0, 0, flags);
			m_size = size(XOR("A"));
		}

		// ctor.
		__forceinline Font(HFont font) {
			m_handle = font;
			m_size = size(XOR("A"));
		}

		void string(int x, int y, Color color, const std::string& text, StringFlags_t flags = ALIGN_LEFT);
		void string(int x, int y, Color color, const std::stringstream& text, StringFlags_t flags = ALIGN_LEFT);
		void wstring(int x, int y, Color color, const std::wstring& text, StringFlags_t flags = ALIGN_LEFT);
		render::FontSize_t size(const std::string& text);
		FontSize_t wsize(const std::wstring& text);
	};
	static vertex_t RotateVertex(const vec2_t& p, const vertex_t& v, float angle) {
		// convert theta angle to sine and cosine representations.
		float c = std::cos(DEG2RAD(angle));
		float s = std::sin(DEG2RAD(angle));

		return {
			p.x + (v.m_pos.x - p.x) * c - (v.m_pos.y - p.y) * s,
			p.y + (v.m_pos.x - p.x) * s + (v.m_pos.y - p.y) * c
		};
	}
	extern Font esp;
	extern Font pixel;
	extern Font console;
	extern Font hud;
	extern Font icon;
	extern Font icon2;
	extern Font indicator;

	void initialize();
	bool WorldToScreen(const vec3_t& world, vec2_t& screen);
	void line(vec2_t v0, vec2_t v1, Color color);
	void world_circle(vec3_t origin, float radius, Color color);
	void line(int x0, int y0, int x1, int y1, Color color);
	void rect(int x, int y, int w, int h, Color color);
	void rect_filled(int x, int y, int w, int h, Color color);
	void rect_filled_fade(int x, int y, int w, int h, Color color, int a1, int a2);
	void rect_outlined(int x, int y, int w, int h, Color color, Color color2);
	void circle(int x, int y, int radius, int segments, Color color);
	void gradient(int x, int y, int w, int h, Color color1, Color color2);
	void sphere(vec3_t origin, float radius, float angle, float scale, Color color);
}