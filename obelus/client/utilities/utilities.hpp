#pragma once
#include <cstdint>
#include "xor.h"
#include "module.h"
#include "../../sdk/math/vector3d.hpp"
#include "../../client/utilities/pattern.h"

namespace utilities 
{
	template< typename t >
	__forceinline static t CallVirtual( void *name, int offset ) 
	{
		int *table = *( int ** ) name;
		int address = table[ offset ];
		return ( t ) ( address );
	}
	template< typename o = void*, typename i = void* >
	__forceinline o ForceCast(i in) {
		union { i in; o out; }
		u = { in };
		return u.out;
	};
	template < typename t = Address >
	__forceinline static t GetMethod(Address this_ptr, size_t index) {
		return (t)this_ptr.to< t* >()[index];
	}    
	template <typename T, typename ... args_t>
	inline constexpr T CallVFunc(void* thisptr, std::size_t nIndex, args_t... argList)
	{
		using VirtualFn = T(__thiscall*)(void*, decltype(argList)...);
		return (*(VirtualFn**)thisptr)[nIndex](thisptr, argList...);
	}


	struct WavHeader_t {
		unsigned char riff[4];                     
		unsigned int overall_size;                 
		unsigned char wave[4];                     
		unsigned char fmt_chunk_marker[4];         
		unsigned int length_of_fmt;                
		unsigned int format_type;                  
		unsigned int channels;                     
		unsigned int sample_rate;                  
		unsigned int byterate;                     
		unsigned int block_align;                  
		unsigned int bits_per_sample;              
		unsigned char data_chunk_header[4];        
		unsigned int data_size;                    
		unsigned char* data;					

		void ParseWavHeader(unsigned char* wavData);
	};

	BYTE* ReadWavFileIntoMemory(std::string fname); 
	bool AdjustHitsoundVolume(WavHeader_t& header, float volume);

	std::string WideToMultiByte(const std::wstring& str);
	std::wstring MultiByteToWide(const std::string& str);
	void Print(const std::string text, ...);
	int TIME_TO_TICKS(float time);
	float TICKS_TO_TIME(int ticks); 
	void ForceFullUpdate();



}
