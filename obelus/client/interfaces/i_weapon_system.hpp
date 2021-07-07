#pragma once
#include "../../sdk/structs/weaponinfo.hpp"

class IWeaponSystem {
	virtual void unused0() = 0;
	virtual void unused1() = 0;
public:
	virtual CWeaponData* get_weapon_data(unsigned int idx) const = 0;
};