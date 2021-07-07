#pragma once
#include <windows.h>
#include <SDKDDKVer.h>
#include <string.h>
#include <vadefs.h>
#include <stdio.h>
#include <xstring>
#include <Psapi.h>
#include <thread>
#include <iostream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <string>
#include <vector>
#include <time.h>
#include <winuser.h>
#include <random>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <set>
#include <map>
#include <unordered_map>
#include <memory>
#include <thread>
#include <array>
#include <assert.h>
#include <deque>
#include <intrin.h>
#include <atomic>
#include <mutex>
#include <stdint.h>
#include <stdlib.h>
#include <process.h>
#include <Wincon.h>
#include <cstdint>
#include <chrono>
#include <Shlobj.h>
#include <future>
#include <Lmcons.h>
#include <tchar.h>
#include "../../../sdk/classes/recv_props.hpp"
#include "../../../sdk/classes/client_class.hpp"
#include "../../utilities/fnv.hpp"

typedef unsigned int       uint32_t;
using hash32_t = uint32_t;
class datamap_t;
using inputfunc_t = void(__cdecl*)(void* data);

enum fieldtype_t {
    FIELD_VOID = 0,			// No type or value
    FIELD_FLOAT,			// Any floating point value
    FIELD_STRING,			// A string ID (return from ALLOC_STRING)
    FIELD_VECTOR,			// Any vector, QAngle, or AngularImpulse
    FIELD_QUATERNION,		// A quaternion
    FIELD_INTEGER,			// Any integer or enum
    FIELD_BOOLEAN,			// boolean, implemented as an int, I may use this as a hint for compression
    FIELD_SHORT,			// 2 byte integer
    FIELD_CHARACTER,		// a byte
    FIELD_COLOR32,			// 8-bit per channel r,g,b,a (32bit color)
    FIELD_EMBEDDED,			// an embedded object with a datadesc, recursively traverse and embedded class/structure based on an additional typedescription
    FIELD_CUSTOM,			// special type that contains function pointers to it's read/write/parse functions
    FIELD_CLASSPTR,			// CBaseEntity *
    FIELD_EHANDLE,			// Entity handle
    FIELD_EDICT,			// edict_t *
    FIELD_POSITION_VECTOR,	// A world coordinate (these are fixed up across level transitions automagically)
    FIELD_TIME,				// a floating point time (these are fixed up automatically too!)
    FIELD_TICK,				// an integer tick count( fixed up similarly to time)
    FIELD_MODELNAME,		// Engine string that is a model name (needs precache)
    FIELD_SOUNDNAME,		// Engine string that is a sound name (needs precache)
    FIELD_INPUT,			// a list of inputed data fields (all derived from CMultiInputVar)
    FIELD_FUNCTION,			// A class function pointer (Think, Use, etc)
    FIELD_VMATRIX,			// a vmatrix (output coords are NOT worldspace)
    FIELD_VMATRIX_WORLDSPACE,// A VMatrix that maps some local space to world space (translation is fixed up on level transitions)
    FIELD_MATRIX3X4_WORLDSPACE,	// matrix3x4_t that maps some local space to world space (translation is fixed up on level transitions)
    FIELD_INTERVAL,			// a start and range floating point interval ( e.g., 3.2->3.6 == 3.2 and 0.4 )
    FIELD_MODELINDEX,		// a model index
    FIELD_MATERIALINDEX,	// a material index (using the material precache string table)
    FIELD_VECTOR2D,			// 2 floats
    FIELD_TYPECOUNT,		// MUST BE LAST
};
class Address;

enum 
{
    TD_OFFSET_NORMAL = 0,
    TD_OFFSET_PACKED = 1,
    TD_OFFSET_COUNT,
};

#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )
#define PAD( size ) uint8_t MACRO_CONCAT( _pad, __COUNTER__ )[ size ];

class typedescription_t {
public:
    fieldtype_t				m_type;
    const char* m_name;
    int						m_offset[TD_OFFSET_COUNT];
    unsigned short			m_size;
    short					iFlags;
    const char* m_ext_name;
    void* m_save_restore_ops;
    inputfunc_t				m_input_func;
    datamap_t* m_td;
    int						m_bytes;
    typedescription_t* m_override_field;
    int						m_override_count;
    float					m_tolerance;
private:
    PAD(0x8);
};


class datamap_t {
public:
    typedescription_t* m_desc;
    int					m_size;
    char const* m_name;
    datamap_t* m_base;
    bool		        m_chains_validated;
    bool				m_packed_offsets_computed;
    int			        m_packed_size;
};

class Netvars 
{
private:
    struct NetvarData {
        bool        m_datamap_var; // we can't do proxies on stuff from datamaps :).
        RecvProp* m_prop_ptr;
        size_t      m_offset;

        __forceinline NetvarData() : m_datamap_var{}, m_prop_ptr{}, m_offset{} { }
    };

    std::unordered_map< hash32_t, int > m_client_ids;

    // netvar container.
    std::unordered_map< hash32_t,		// hash key + tables
        std::unordered_map< hash32_t,	// hash key + props
        NetvarData						// prop offset / prop ptr
        > > m_offsets;

public:
    // ctor.
    Netvars() : m_offsets{} {}

    void initialize();
    uintptr_t GetNetVar(uint32_t table, uint32_t prop);

    // dtor.
    ~Netvars() {
        m_offsets.clear();
    }

    // gather all classids dynamically on runtime.
    void SetupClassData();

    void StoreDataTable(const std::string& name, RecvTable* table, size_t offset = 0);

    // iterate client module and find all datamaps.
    void FindAndStoreDataMaps();
    void StoreDataMap(Address ptr);

    // get client id.
    __forceinline int GetClientID(hash32_t network_name) {
        return m_client_ids[network_name];
    }

    // get netvar offset.
    __forceinline size_t get(hash32_t table, hash32_t prop) {
        return m_offsets[table][prop].m_offset;
    }

    // get netvar proxy.
    __forceinline RecvVarProxy_t GetProxy(hash32_t table, hash32_t prop, RecvVarProxy_t proxy) {
        // todo - ; should we return null here if m_is_valid_netvar is false? probably doesn't matter anyway.
        return m_offsets[table][prop].m_prop_ptr->m_ProxyFn;
    }

    // set netvar proxy.
    __forceinline void SetProxy(hash32_t table, hash32_t prop, void* proxy, RecvVarProxy_t& original) {
        auto netvar_entry = m_offsets[table][prop];

        // we can't set a proxy on a datamap.
        if (netvar_entry.m_datamap_var)
            return;

        // save original.
        original = netvar_entry.m_prop_ptr->m_ProxyFn;

        // redirect.
        netvar_entry.m_prop_ptr->m_ProxyFn = (RecvVarProxy_t)proxy;
    }

};

inline Netvars netvars;


#define MEMEBR_FUNC_ARGS(...) ( this, __VA_ARGS__ ); }
#define VFUNC( index, func, sig ) auto func { return utilities::CallVirtual< sig >( this, index ) MEMEBR_FUNC_ARGS
#define NETVAR( table, prop, func_name, type ) type& func_name() { \
    static uintptr_t offset = 0; \
    if( !offset ) { \
        offset = netvars.GetNetVar( fnv::hash( table ), fnv::hash( prop ) ); \
    } \
    return *reinterpret_cast< type* >( uintptr_t( this ) + offset ); \
}

#define NETVAR_PTR( table, prop, func_name, type ) type* func_name() { \
    static uintptr_t offset = 0; \
    if( !offset ) { \
        offset = netvars.GetNetVar( fnv::hash( table ), fnv::hash( prop ) ); \
    } \
    return reinterpret_cast< type* >( uintptr_t( this ) + offset ); \
}


#define OFFSET( type, var, offset ) type& var() { \
    return *( type* )( uintptr_t( this ) + offset ); \
}

