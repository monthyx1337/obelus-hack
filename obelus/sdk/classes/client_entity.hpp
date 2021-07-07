#pragma once
#pragma once
#pragma once
#include "entities.hpp"
#include "../sdk.hpp"


enum CSRoundEndReason {
	UNKNOWN1 = 0,
	BOMB_DETONATED,			// terrorists planted bomb and it detonated.
	UNKNOWN2,
	UNKNOWN3,
	T_ESCAPED,				// dunno if used.
	CT_STOPPED_ESCAPE,		// dunno if used.
	T_STOPPED,				// dunno if used
	BOMB_DEFUSED,			// counter-terrorists defused the bomb.
	CT_WIN,					// counter-terrorists killed all terrorists.
	T_WIN,					// terrorists killed all counter-terrorists.
	ROUND_DRAW,				// draw ( likely due to time ).
	HOSTAGE_RESCUED,		// counter-terrorists rescued a hostage.
	CT_WIN_TIME,
	T_WIN_TIME,
	T_NOT_ESACPED,
	UNKNOWN4,
	GAME_START,
	T_SURRENDER,
	CT_SURRENDER,
};

enum CScoreType {
	NONE = 0,
	DRAW = 1,
	TLEAD = 2,
	CTLEAD = 3

};
class CFlashLightEffect
{
public:
	bool m_bIsOn; //0x0000 
	char pad_0x0001[0x3]; //0x0001
	int m_nEntIndex; //0x0004 
	WORD m_FlashLightHandle; //0x0008 
	char pad_0x000A[0x2]; //0x000A
	float m_flMuzzleFlashBrightness; //0x000C 
	float m_flFov; //0x0010 
	float m_flFarZ; //0x0014 
	float m_flLinearAtten; //0x0018 
	bool m_bCastsShadows; //0x001C 
	char pad_0x001D[0x3]; //0x001D
	float m_flCurrentPullBackDist; //0x0020 
	DWORD m_MuzzleFlashTexture; //0x0024 
	DWORD m_FlashLightTexture; //0x0028 
	char m_szTextureName[64]; //0x1559888 
}; //Size=0x006C

struct RenderableInfo_t
{
	i_client_renderable* renderable;
	void* alpha_property;
	int enum_count;
	int render_frame;
	unsigned short first_shadow;
	unsigned short leaf_list;
	short area;
	std::uint16_t flags;
	std::uint16_t flags2;
	vec3_t bloated_abs_mins;
	vec3_t bloated_abs_maxs;
	vec3_t abs_mins;
	vec3_t abs_maxs;
	int pad;
};

template<class T> struct CUtlReference {
	CUtlReference* m_pNext;
	CUtlReference* m_pPrev;
	T* m_pObject;
};
template<class T> struct CUtlIntrusiveList {
	T* m_pHead;
};
template<class T> struct CUtlIntrusiveDList : public CUtlIntrusiveList<T> {};
template<class T> struct CUtlReferenceList : public CUtlIntrusiveDList< CUtlReference<T> > {};

enum EAttributeDataType {
	ATTRDATATYPE_NONE = -1,
	ATTRDATATYPE_FLOAT = 0,
	ATTRDATATYPE_4V,
	ATTRDATATYPE_INT,
	ATTRDATATYPE_POINTER,

	ATTRDATATYPE_COUNT,
};

#define MAX_PARTICLE_ATTRIBUTES 24

#define DEFPARTICLE_ATTRIBUTE( name, bit, datatype )			\
	const int PARTICLE_ATTRIBUTE_##name##_MASK = (1 << bit);	\
	const int PARTICLE_ATTRIBUTE_##name = bit;					\
	const EAttributeDataType PARTICLE_ATTRIBUTE_##name##_DATATYPE = datatype;

DEFPARTICLE_ATTRIBUTE(TINT_RGB, 6, ATTRDATATYPE_4V);

DEFPARTICLE_ATTRIBUTE(ALPHA, 7, ATTRDATATYPE_FLOAT);

struct CParticleAttributeAddressTable {
	float* m_pAttributes[MAX_PARTICLE_ATTRIBUTES];
	size_t m_nFloatStrides[MAX_PARTICLE_ATTRIBUTES];

	FORCEINLINE float* FloatAttributePtr(int nAttribute, int nParticleNumber) const {
		int block_ofs = nParticleNumber / 4;
		return m_pAttributes[nAttribute] +
			m_nFloatStrides[nAttribute] * block_ofs +
			(nParticleNumber & 3);
	}

};

struct CUtlString_simple {
	char* buffer;
	int capacity;
	int grow_size;
	int length;
};

class CParticleSystemDefinition {
	BYTE pad_0[308];
public:
	CUtlString_simple m_Name;
};

class CParticleCollection {
	BYTE pad_0[48];//0
public:
	int m_nActiveParticles;//48
private:
	BYTE pad_1[12];//52
public:
	CUtlReference<CParticleSystemDefinition> m_pDef;//64
private:
	BYTE pad_2[60];//80
public:
	CParticleCollection* m_pParent;//136
private:
	BYTE pad_3[84];//140
public:
	CParticleAttributeAddressTable m_ParticleAttributes;//224
};
enum struct CGroupType
{
	PLAYERS_ALL,
	PLAYERS_ENEMIES,
	PLAYERS_TEAMMATES,
	WORLD_ALL,
};
namespace entity_cache
{
	inline std::map<CGroupType, std::vector<BaseEntity*>> Groups;

	void Fill();
	void Clear();
	void Update();

	struct InfernoData
	{
		InfernoData(BaseEntity* inferno) noexcept;

		std::vector<vec3_t> points;
	};

	inline std::vector<InfernoData> pInfernoData;
	const std::vector<BaseEntity*>& GetEntityGroup(const CGroupType& group);
}
enum data_update_type_t {
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED,
};

enum cs_weapon_type {
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

enum client_frame_stage_t {
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

enum MoveType {
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4,
	MAX_MOVETYPE
};

enum entity_flags {
	FL_ONGROUND = (1 << 0),
	FL_DUCKING = (1 << 1),
	FL_WATERJUMP = (1 << 2),
	FL_ONTRAIN = (1 << 3),
	FL_INRAIN = (1 << 4),
	FL_FROZEN = (1 << 5),
	FL_ATCONTROLS = (1 << 6),
	FL_CLIENT = (1 << 7),
	FL_FAKECLIENT = (1 << 8),
	FL_INWATER = (1 << 9),
	FL_FLY = (1 << 10),
	FL_SWIM = (1 << 11),
	FL_CONVEYOR = (1 << 12),
	FL_NPC = (1 << 13),
	FL_GODMODE = (1 << 14),
	FL_NOTARGET = (1 << 15),
	FL_AIMTARGET = (1 << 16),
	FL_PARTIALGROUND = (1 << 17),
	FL_STATICPROP = (1 << 18),
	FL_GRAPHED = (1 << 19),
	FL_GRENADE = (1 << 20),
	FL_STEPMOVEMENT = (1 << 21),
	FL_DONTTOUCH = (1 << 22),
	FL_BASEVELOCITY = (1 << 23),
	FL_WORLDBRUSH = (1 << 24),
	FL_OBJECT = (1 << 25),
	FL_KILLME = (1 << 26),
	FL_ONFIRE = (1 << 27),
	FL_DISSOLVING = (1 << 28),
	FL_TRANSRAGDOLL = (1 << 29),
	FL_UNBLOCKABLE_BY_PLAYER = (1 << 30)
};
enum ItemDefinitionIndexes {
	WEAPON_NONE = 0,
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SHIELD = 37,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFEGG = 41,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS = 69,
	WEAPON_BREACHCHARGE = 70,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE = 75,
	WEAPON_HAMMER = 76,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB = 81,
	WEAPON_DIVERSION = 82,
	WEAPON_FRAG_GRENADE = 83,
	WEAPON_SNOWBALL = 84,
	WEAPON_BUMPMINE = 85,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	WEAPON_KNIFE_CORD = 517,
	WEAPON_KNIFE_CANIS = 518,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520,
	WEAPON_KNIFE_OUTDOOR = 521,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER = 523,
	WEAPON_KNIFE_SKELETON = 525,
	GLOVE_STUDDED_BROKENFANG = 4725,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};
enum Effects_t : int {
	EF_BONEMERGE = 0x001,	// Performs bone merge on client side
	EF_BRIGHTLIGHT = 0x002,	// DLIGHT centered at entity origin
	EF_DIMLIGHT = 0x004,	// player flashlight
	EF_NOINTERP = 0x008,	// don't interpolate the next frame
	EF_NOSHADOW = 0x010,	// Don't cast no shadow
	EF_NODRAW = 0x020,	// don't draw entity
	EF_NORECEIVESHADOW = 0x040,	// Don't receive no shadow
	EF_BONEMERGE_FASTCULL = 0x080,	// For use with EF_BONEMERGE. If this is set, then it places this ent's origin at its
	EF_ITEM_BLINK = 0x100,	// blink an item so that the user notices it.
	EF_PARENT_ANIMATES = 0x200,	// always assume that the parent entity is animating
	EF_MAX_BITS = 10
};

class player_t;
class IClientUnknown;
class bf_read;

class IClientNetworkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void             Release() = 0;
	virtual ClientClass* GetClientClass() = 0;
	virtual void             NotifyShouldTransmit(int state) = 0;
	virtual void             OnPreDataChanged(int updateType) = 0;
	virtual void             OnDataChanged(int updateType) = 0;
	virtual void             PreDataUpdate(int updateType) = 0;
	virtual void             PostDataUpdate(int updateType) = 0;
	virtual void             __unkn(void) = 0;
	virtual bool             unk2(void) = 0;
	virtual int              EntIndex(void) const = 0;
	virtual void             ReceiveMessage(int classID, bf_read& msg) = 0;
	virtual void* GetDataTableBasePtr() = 0;
	virtual void             SetDestroyedOnRecreateEntities(void) = 0;
};
class IClientUnknown;
class CClientThinkHandlePtr;
typedef CClientThinkHandlePtr* ClientThinkHandle_t;

class IClientThinkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void				ClientThink() = 0;
	virtual ClientThinkHandle_t	GetThinkHandle() = 0;
	virtual void				SetThinkHandle(ClientThinkHandle_t hThink) = 0;
	virtual void				Release() = 0;
};
class IClientAlphaProperty;
class C_BaseEntity;
class IClientEntity;

class ICollideable
{
public:
	virtual void pad0();
	virtual const vec3_t& OBBMins() const;
	virtual const vec3_t& OBBMaxs() const;
};




typedef unsigned short ClientShadowHandle_t;
typedef unsigned short ClientRenderHandle_t;
typedef unsigned short ModelInstanceHandle_t;
typedef unsigned char uint8;


struct model_t;


struct RenderableInstance_t
{
	uint8 m_nAlpha;
};

class IClientRenderable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual vec3_t const& GetRenderOrigin(void) = 0;
	virtual vec3_t const& GetRenderAngles(void) = 0;
	virtual bool                       ShouldDraw(void) = 0;
	virtual int                        GetRenderFlags(void) = 0; // ERENDERFLAGS_xxx
	virtual void                       Unused(void) const {}
	virtual ClientShadowHandle_t       GetShadowHandle() const = 0;
	virtual ClientRenderHandle_t& RenderHandle() = 0;
	virtual model_t* GetModel() const = 0;
	virtual int                        DrawModel(int flags, const int& instance) = 0;
	virtual int                        GetBody() = 0;
	virtual void                       GetColorModulation(float* color) = 0;
	virtual bool                       LODTest() = 0;
	virtual bool                       SetupBones(matrix_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;
	virtual void                       SetupWeights(const matrix_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) = 0;
	virtual void                       DoAnimationEvents(void) = 0;
	virtual void* /*IPVSNotify*/       GetPVSNotifyInterface() = 0;
	virtual void                       GetRenderBounds(vec3_t& mins, vec3_t& maxs) = 0;
	virtual void                       GetRenderBoundsWorldspace(vec3_t& mins, vec3_t& maxs) = 0;
	virtual void                       GetShadowRenderBounds(vec3_t& mins, vec3_t& maxs, int /*ShadowType_t*/ shadowType) = 0;
	virtual bool                       ShouldReceiveProjectedTextures(int flags) = 0;
	virtual bool                       GetShadowCastDistance(float* pDist, int /*ShadowType_t*/ shadowType) const = 0;
	virtual bool                       GetShadowCastDirection(vec3_t* pDirection, int /*ShadowType_t*/ shadowType) const = 0;
	virtual bool                       IsShadowDirty() = 0;
	virtual void                       MarkShadowDirty(bool bDirty) = 0;
	virtual IClientRenderable* GetShadowParent() = 0;
	virtual IClientRenderable* FirstShadowChild() = 0;
	virtual IClientRenderable* NextShadowPeer() = 0;
	virtual int /*ShadowType_t*/       ShadowCastType() = 0;
	virtual void                       CreateModelInstance() = 0;
	virtual ModelInstanceHandle_t      GetModelInstance() = 0;
	virtual const matrix_t& RenderableToWorldTransform() = 0;
	virtual int                        LookupAttachment(const char* pAttachmentName) = 0;
	virtual   bool                     GetAttachment(int number, vec3_t& origin, vec3_t& angles) = 0;
	virtual bool                       GetAttachment(int number, matrix_t& matrix) = 0;
	virtual float* GetRenderClipPlane(void) = 0;
	virtual int                        GetSkin() = 0;
	virtual void                       OnThreadedDrawSetup() = 0;
	virtual bool                       UsesFlexDelayedWeights() = 0;
	virtual void                       RecordToolMessage() = 0;
	virtual bool                       ShouldDrawForSplitScreenUser(int nSlot) = 0;
	virtual uint8                      OverrideAlphaModulation(uint8 nAlpha) = 0;
	virtual uint8                      OverrideShadowAlphaModulation(uint8 nAlpha) = 0;
};
using c_base_handle = unsigned long;
class CBaseHandle2;
class i_handle_entity
{
public:
	virtual ~i_handle_entity() = default;
	virtual void set_ref_handle(const c_base_handle& handle) = 0;
	virtual const CBaseHandle2& GetRefEHandle() const = 0;

	c_base_handle& get_handle() {
		using original_fn = c_base_handle & (__thiscall*)(i_handle_entity*);
		return (*(original_fn**)this)[2](this);
	}

};
class IClientUnknown : public i_handle_entity
{
public:
	virtual ICollideable* GetCollideable() = 0;
	virtual IClientNetworkable* GetClientNetworkable() = 0;
	virtual IClientRenderable* GetClientRenderable() = 0;
	virtual IClientEntity* GetIClientEntity() = 0;
	virtual player_t* GetBaseEntity() = 0;
	virtual IClientThinkable* GetClientThinkable() = 0;
	//virtual IClientModelRenderable*  GetClientModelRenderable() = 0;
	virtual IClientAlphaProperty* GetClientAlphaProperty() = 0;
};

#define NUM_ENT_ENTRY_BITS         (11 + 2)
#define NUM_ENT_ENTRIES            (1 << NUM_ENT_ENTRY_BITS)
#define INVALID_EHANDLE_INDEX       0xFFFFFFFF
#define NUM_SERIAL_NUM_BITS        16 // (32 - NUM_ENT_ENTRY_BITS)
#define NUM_SERIAL_NUM_SHIFT_BITS (32 - NUM_SERIAL_NUM_BITS)
#define ENT_ENTRY_MASK             (( 1 << NUM_SERIAL_NUM_BITS) - 1)


class CBaseHandle2
{
public:
	CBaseHandle2();
	CBaseHandle2(const CBaseHandle2& other);
	CBaseHandle2(unsigned long value);
	CBaseHandle2(int iEntry, int iSerialNumber);

	void Init(int iEntry, int iSerialNumber);
	void Term();

	// Even if this returns true, Get() still can return return a non-null value.
	// This just tells if the handle has been initted with any values.
	bool IsValid() const;

	int GetEntryIndex() const;
	int GetSerialNumber() const;

	int ToInt() const;
	bool operator !=(const CBaseHandle2& other) const;
	bool operator ==(const CBaseHandle2& other) const;
	bool operator ==(const i_handle_entity* pEnt) const;
	bool operator !=(const i_handle_entity* pEnt) const;
	bool operator <(const CBaseHandle2& other) const;
	bool operator <(const i_handle_entity* pEnt) const;

	// Assign a value to the handle.
	const CBaseHandle2& operator=(const i_handle_entity* pEntity);
	const CBaseHandle2& Set(const i_handle_entity* pEntity);

	// Use this to dereference the handle.
	// Note: this is implemented in game code (ehandle.h)
	i_handle_entity* Get() const;

protected:
	// The low NUM_SERIAL_BITS hold the index. If this value is less than MAX_EDICTS, then the entity is networkable.
	// The high NUM_SERIAL_NUM_BITS bits are the serial number.
	unsigned long	m_Index;
};

inline CBaseHandle2::CBaseHandle2()
{
	m_Index = INVALID_EHANDLE_INDEX;
}

inline CBaseHandle2::CBaseHandle2(const CBaseHandle2& other)
{
	m_Index = other.m_Index;
}

inline CBaseHandle2::CBaseHandle2(unsigned long value)
{
	m_Index = value;
}

inline CBaseHandle2::CBaseHandle2(int iEntry, int iSerialNumber)
{
	Init(iEntry, iSerialNumber);
}

inline void CBaseHandle2::Init(int iEntry, int iSerialNumber)
{
	m_Index = iEntry | (iSerialNumber << NUM_ENT_ENTRY_BITS);
}

inline void CBaseHandle2::Term()
{
	m_Index = INVALID_EHANDLE_INDEX;
}

inline bool CBaseHandle2::IsValid() const
{
	return m_Index != INVALID_EHANDLE_INDEX;
}

inline int CBaseHandle2::GetEntryIndex() const
{
	return m_Index & ENT_ENTRY_MASK;
}

inline int CBaseHandle2::GetSerialNumber() const
{
	return m_Index >> NUM_ENT_ENTRY_BITS;
}

inline int CBaseHandle2::ToInt() const
{
	return (int)m_Index;
}

inline bool CBaseHandle2::operator !=(const CBaseHandle2& other) const
{
	return m_Index != other.m_Index;
}

inline bool CBaseHandle2::operator ==(const CBaseHandle2& other) const
{
	return m_Index == other.m_Index;
}

inline bool CBaseHandle2::operator ==(const i_handle_entity* pEnt) const
{
	return Get() == pEnt;
}

inline bool CBaseHandle2::operator !=(const i_handle_entity* pEnt) const
{
	return Get() != pEnt;
}

inline bool CBaseHandle2::operator <(const CBaseHandle2& other) const
{
	return m_Index < other.m_Index;
}

inline bool CBaseHandle2::operator <(const i_handle_entity* pEntity) const
{
	unsigned long otherIndex = (pEntity) ? pEntity->GetRefEHandle().m_Index : INVALID_EHANDLE_INDEX;
	return m_Index < otherIndex;
}

inline const CBaseHandle2& CBaseHandle2::operator=(const i_handle_entity* pEntity)
{
	return Set(pEntity);
}

inline const CBaseHandle2& CBaseHandle2::Set(const i_handle_entity* pEntity)
{
	if (pEntity) {
		*this = pEntity->GetRefEHandle();
	}
	else {
		m_Index = INVALID_EHANDLE_INDEX;
	}

	return *this;
}

template< class T >
class CHandle : public CBaseHandle2
{
public:

	CHandle();
	CHandle(int iEntry, int iSerialNumber);
	CHandle(const CBaseHandle2& handle);
	CHandle(T* pVal);

	// The index should have come from a call to ToInt(). If it hasn't, you're in trouble.
	static CHandle<T> FromIndex(int index);

	T* Get() const;
	void	Set(const T* pVal);

	operator T* ();
	operator T* () const;

	bool	operator !() const;
	bool	operator==(T* val) const;
	bool	operator!=(T* val) const;
	const CBaseHandle2& operator=(const T* val);

	T* operator->() const;
};


// ----------------------------------------------------------------------- //
// Inlines.
// ----------------------------------------------------------------------- //

template<class T>
CHandle<T>::CHandle()
{
}


template<class T>
CHandle<T>::CHandle(int iEntry, int iSerialNumber)
{
	Init(iEntry, iSerialNumber);
}


template<class T>
CHandle<T>::CHandle(const CBaseHandle2& handle)
	: CBaseHandle2(handle)
{
}


template<class T>
CHandle<T>::CHandle(T* pObj)
{
	Term();
	Set(pObj);
}


template<class T>
inline CHandle<T> CHandle<T>::FromIndex(int index)
{
	CHandle<T> ret;
	ret.m_Index = index;
	return ret;
}


template<class T>
inline T* CHandle<T>::Get() const
{
	return (T*)CBaseHandle2::Get();
}


template<class T>
inline CHandle<T>::operator T* ()
{
	return Get();
}

template<class T>
inline CHandle<T>::operator T* () const
{
	return Get();
}


template<class T>
inline bool CHandle<T>::operator !() const
{
	return !Get();
}

template<class T>
inline bool CHandle<T>::operator==(T* val) const
{
	return Get() == val;
}

template<class T>
inline bool CHandle<T>::operator!=(T* val) const
{
	return Get() != val;
}

template<class T>
void CHandle<T>::Set(const T* pVal)
{
	CBaseHandle2::Set(reinterpret_cast<const i_handle_entity*>(pVal));
}

template<class T>
inline const CBaseHandle2& CHandle<T>::operator=(const T* val)
{
	Set(val);
	return *this;
}

template<class T>
T* CHandle<T>::operator -> () const
{
	return Get();
}