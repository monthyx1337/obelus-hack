#pragma once
#include <cstdint>
#include <d3d9.h>
#include <array>

class Color {
public:
	int a, r, g, b;
	Color( ) = default;
	Color( const int r, const int g, const int b, const int a = 255 ) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	Color(Color col, const int a) {
		this->r = col.r;
		this->g = col.g;
		this->b = col.b;
		this->a = a;
	}
	Color(float col, const int a) {
		this->r = col;
		this->g = col;
		this->b = col;
		this->a = a;
	}

	explicit Color( const uint32_t color ) {
		this->a = ( color >> 24 ) & 0xff;
		this->r = ( color >> 16 ) & 0xff;
		this->g = ( color >> 8 ) & 0xff;
		this->b = ( color & 0xff );
	}

	static Color from_uint( const uint32_t uint ) {
		return Color( uint );
	}

	static D3DCOLOR from_color( const Color col ) {
		return D3DCOLOR_ARGB( col.a, col.r, col.g, col.b );
	}

	static Color black( const int a = 255 ) { return { 0, 0, 0, a }; }
	static Color white( const int a = 255 ) { return { 255, 255, 255, a }; }
	static Color red( const int   a = 255 ) { return { 255, 0, 0, a }; }
	static Color green( const int a = 255 ) { return { 0, 255, 0, a }; }
	static Color blue( const int  a = 255 ) { return { 0, 0, 255, a }; }

	inline float rBase() const { return r / 255.f; }
	inline float gBase() const { return g / 255.f; }
	inline float bBase() const { return b / 255.f; }
	inline float aBase() const { return a / 255.f; }

	std::array<float, 3U> Base()
	{
		std::array<float, 3U> arrBaseColor = { };
		arrBaseColor.at(0) = r / 255.f;
		arrBaseColor.at(1) = g / 255.f;
		arrBaseColor.at(2) = b / 255.f;
		return arrBaseColor;
	}
	__forceinline Color OverrideAlpha(int alpha, bool adaptive = false) {
		return Color(r, g, b, adaptive ? static_cast<int>(static_cast<float>(alpha) * (a / 255.f)) : alpha);
	}
	static Color HSLToRGB(float h, float s, float l) {
		float q;

		if (l < 0.5f)
			q = l * (s + 1.f);

		else
			q = l + s - (l * s);

		float p = 2 * l - q;

		float rgb[3];
		rgb[0] = h + (1.f / 3.f);
		rgb[1] = h;
		rgb[2] = h - (1.f / 3.f);

		for (int i = 0; i < 3; ++i) {
			if (rgb[i] < 0)
				rgb[i] += 1.f;

			if (rgb[i] > 1)
				rgb[i] -= 1.f;

			if (rgb[i] < (1.f / 6.f))
				rgb[i] = p + ((q - p) * 6 * rgb[i]);
			else if (rgb[i] < 0.5f)
				rgb[i] = q;
			else if (rgb[i] < (2.f / 3.f))
				rgb[i] = p + ((q - p) * 6 * ((2.f / 3.f) - rgb[i]));
			else
				rgb[i] = p;
		}

		return {
			int(rgb[0] * 255.f),
			int(rgb[1] * 255.f),
			int(rgb[2] * 255.f)
		};
	}
};

namespace colors {
	static Color white{ 255, 255, 255, 255 };
	static Color black{ 0, 0, 0, 255 };
	static Color red{ 255, 0, 0, 255 };
	static Color green{ 0, 255, 0, 255 };
	static Color yellow{ 255, 255, 0, 255 };
	static Color burgundy{ 0xff2d00b3 };
	static Color light_blue{ 95, 174, 227, 255 };
	static Color orange{ 243, 156, 18, 255 };
	static Color transparent_white{ 255, 255, 255, 210 };
	static Color transparent_green{ 0, 255, 0, 210 };
	static Color transparent_yellow{ 255, 255, 0, 210 };
}
//-----------------------------------------------------------------------------
// Purpose: Basic handler for an rgb set of colors
//			This class is fully inline
//-----------------------------------------------------------------------------
class ValveColor
{
public:
	// constructors
	ValveColor()
	{
		*((int*)this) = 0;
	}
	ValveColor(int _r, int _g, int _b)
	{
		SetValveColor(_r, _g, _b, 0);
	}
	ValveColor(int _r, int _g, int _b, int _a)
	{
		SetValveColor(_r, _g, _b, _a);
	}

	// set the color
	// r - red component (0-255)
	// g - green component (0-255)
	// b - blue component (0-255)
	// a - alpha component, controls transparency (0 - transparent, 255 - opaque);
	void SetValveColor(int _r, int _g, int _b, int _a = 0)
	{
		_color[0] = (unsigned char)_r;
		_color[1] = (unsigned char)_g;
		_color[2] = (unsigned char)_b;
		_color[3] = (unsigned char)_a;
	}

	void GetValveColor(int& _r, int& _g, int& _b, int& _a) const
	{
		_r = _color[0];
		_g = _color[1];
		_b = _color[2];
		_a = _color[3];
	}

	void SetRawValveColor(int color32)
	{
		*((int*)this) = color32;
	}

	int GetRawValveColor() const
	{
		return *((int*)this);
	}

	inline int r() const { return _color[0]; }
	inline int g() const { return _color[1]; }
	inline int b() const { return _color[2]; }
	inline int a() const { return _color[3]; }

	unsigned char& operator[](int index)
	{
		return _color[index];
	}

	const unsigned char& operator[](int index) const
	{
		return _color[index];
	}

	bool operator == (const ValveColor& rhs) const
	{
		return (*((int*)this) == *((int*)&rhs));
	}

	bool operator != (const ValveColor& rhs) const
	{
		return !(operator==(rhs));
	}

	ValveColor& operator=(const ValveColor& rhs)
	{
		SetRawValveColor(rhs.GetRawValveColor());
		return *this;
	}

private:
	unsigned char _color[4];
};
