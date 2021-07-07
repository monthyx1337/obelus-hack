#pragma once
#include <cstdint>
class IClientEntity;
class IClientUnknown;
class IClientNetworkable;
class CBaseHandle2;
using CBaseHandle = unsigned long;
class IClientEntityList {
public:
	virtual IClientNetworkable* GetClientNetworkable(int iEnt) = 0;
	virtual IClientNetworkable* GetClientNetworkableFromHandle(CBaseHandle hEnt) = 0;
	virtual IClientUnknown* GetClientUnknownFromHandle(CBaseHandle hEnt) = 0;
	virtual IClientEntity* get_client_entity(int iEnt) = 0;
	virtual IClientEntity* get_client_entity_handle(CBaseHandle2 hEnt) = 0;
	virtual int NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int get_highest_index() = 0;
	virtual void SetMaxEntities(int iMax) = 0;
	virtual int GetMaxEntities() = 0;

	//void* get_client_entity(int index) {
	//	using original_fn = void* (__thiscall*)(IClientEntityList*, int);
	//	return (*(original_fn * *)this)[3](this, index);
	//}
	void* get_client_entity_simple_handle(uintptr_t handle) {
		using original_fn = void* (__thiscall*)(IClientEntityList*, uintptr_t);
		return (*(original_fn**)this)[4](this, handle);
	}
	//int get_highest_index() {
	//	using original_fn = int(__thiscall*)(IClientEntityList*);
	//	return (*(original_fn * *)this)[6](this);
	//}

	template <class T>
	inline T* get(const int index)
	{
		return (T*)(get_client_entity(index));
	}

	template <class T>
	inline T* get(const unsigned long index)
	{
		return (T*)(get_client_entity_handle(index));
	}
};