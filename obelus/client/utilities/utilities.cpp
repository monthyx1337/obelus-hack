#include "../utilities/csgo.hpp"
#include <psapi.h>
#include "../../menu/menu.h"

int utilities::TIME_TO_TICKS(float time) 
{
    return (int)(0.5f + time / interfaces::globals->interval_per_tick);
}

float utilities::TICKS_TO_TIME(int ticks) 
{
    return interfaces::globals->interval_per_tick * (float)(ticks);
}

void utilities::Print(const std::string text, ...) {
    va_list     list;
    int         size;
    std::string buf;

    if (text.empty())
        return;

    va_start(list, text);

    // count needed size.
    size = std::vsnprintf(0, 0, text.c_str(), list);

    // allocate.
    buf.resize(size);

    // print to buffer.
    std::vsnprintf(buf.data(), size + 1, text.c_str(), list);

    va_end(list);

	Color clr = g_menu.ConvertColor(config.menu_color);

	// print to console.
	interfaces::console->ConsoleColorPrintf(ValveColor(clr.r, clr.g, clr.b, 255), XOR("[ obelus ] "));
	interfaces::console->ConsoleColorPrintf(ValveColor(240, 240, 240, 255), buf.append(XOR("\n")).c_str());
}

std::string utilities::WideToMultiByte(const std::wstring& str)
{
    std::string ret;
    int str_len;
    if (str.empty())
        return {};


    str_len = WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), 0, 0, 0, 0);
    ret = std::string(str_len, 0);
    WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), &ret[0], str_len, 0, 0);

    return ret;
}

std::wstring utilities::MultiByteToWide(const std::string& str) 
{
    std::wstring ret;
    int str_len;

    if (str.empty())
        return {};

    str_len = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0);
    ret = std::wstring(str_len, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), &ret[0], str_len);

    return ret;
}

struct hud_weapons_t {
    std::int32_t* get_weapon_count() {
        return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
    }
};

template <class C>
C* FindHudElement(const char* szName)
{
    static auto fn = *reinterpret_cast<DWORD**>(pattern::Scan(XOR("client.dll"), ("B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08")) + 1);
    static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(pattern::Scan(XOR("client.dll"), ("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28")));
    return (C*)find_hud_element(fn, szName);
}

void utilities::ForceFullUpdate()
{
    const auto fn = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(pattern::Scan(XOR("client.dll"), XOR("55 8B EC 51 53 56 8B 75 08 8B D9 57 6B")));
    const auto element = FindHudElement<std::uintptr_t*>(("CCSGO_HudWeaponSelection"));
    if (!element || !fn) { return; }

    const auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0);
    if (!hud_weapons || !*hud_weapons->get_weapon_count()) { return; }

    for (auto i = 0; i < *hud_weapons->get_weapon_count(); i++) {
        i = fn(hud_weapons, i);
    }

    //interfaces::clientstate->delta_tick = -1;
}

void utilities::WavHeader_t::ParseWavHeader(unsigned char* wavData) {
	unsigned char buffer4[4];
	unsigned char buffer2[2];

	unsigned char* walker = wavData;

	// Retreive riff
	memcpy(riff, walker, sizeof(riff));
	walker += sizeof(riff);

	// overall_size
	memcpy(buffer4, walker, sizeof(buffer4));
	walker += sizeof(buffer4);

	// Convert little endian to big endian 4 byte int
	overall_size = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

	// wave
	memcpy(wave, walker, sizeof(wave));
	walker += sizeof(wave);

	// fmt_chunk_marker
	memcpy(fmt_chunk_marker, walker, sizeof(fmt_chunk_marker));
	walker += sizeof(fmt_chunk_marker);

	// length_of_fmt
	memcpy(buffer4, walker, sizeof(buffer4));
	walker += sizeof(buffer4);

	length_of_fmt = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

	// format_type
	memcpy(buffer2, walker, sizeof(buffer2));
	walker += sizeof(buffer2);

	format_type = buffer2[0] | (buffer2[1] << 8);

	// channels
	memcpy(buffer2, walker, sizeof(buffer2));
	walker += sizeof(buffer2);

	channels = buffer2[0] | (buffer2[1] << 8);

	// sample_rate
	memcpy(buffer4, walker, sizeof(buffer4));
	walker += sizeof(buffer4);

	sample_rate = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

	// byterate
	memcpy(buffer4, walker, sizeof(buffer4));
	walker += sizeof(buffer4);

	byterate = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

	// block_align
	memcpy(buffer2, walker, sizeof(buffer2));
	walker += sizeof(buffer2);

	block_align = buffer2[0] | (buffer2[1] << 8);

	// bits_per_sample
	memcpy(buffer2, walker, sizeof(buffer2));
	walker += sizeof(buffer2);

	bits_per_sample = buffer2[0] | (buffer2[1] << 8);

	// data_chunk_header
	memcpy(data_chunk_header, walker, sizeof(data_chunk_header));
	walker += sizeof(data_chunk_header);

	// data_size
	memcpy(buffer4, walker, sizeof(buffer4));
	walker += sizeof(buffer4);

	data_size = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

	data = walker;
}

BYTE* utilities::ReadWavFileIntoMemory(std::string fname) {
	BYTE* pb = nullptr;
	std::ifstream f(fname, std::ios::binary);

	f.seekg(0, std::ios::end);
	size_t lim = f.tellg();

	pb = new BYTE[lim];
	f.seekg(0, std::ios::beg);
	f.read((char*)pb, lim);
	f.close();

	return pb;
}

bool utilities::AdjustHitsoundVolume(WavHeader_t& header, float volume)
{
	// We can only adjust PMC (Uncompressed) WAV files
	if (header.format_type != 1)
		return false;

	// Adjust volume to correct ratio
	float finalVolume = std::clamp(volume, 0.f, 1.f);

	// Loop through all samples and adjust their volumes
	int numOfSamples = header.data_size / header.channels;
	unsigned char* walker = header.data;

	for (int i = 0; i < numOfSamples; i++) {
		switch (header.channels) {
			case sizeof(unsigned char) :
				// Mono
				*walker = (unsigned char)(*walker * finalVolume);
				walker += sizeof(unsigned char);
				break;
				case sizeof(short) :
					// Stereo
					*(short*)walker = (short)(*(short*)walker * finalVolume);
					walker += sizeof(short);
					break;
					case sizeof(int) :
						// Surround?
						*(int*)walker = (int)(*(int*)walker * finalVolume);
						walker += sizeof(int);
						break;
					default:
						return false;
		}
	}

	return true;
}