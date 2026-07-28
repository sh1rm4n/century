//
// Created by sh1rm4n on 6/26/26.
//

#ifndef CENTURY_C_BASECOMBATWEAPON_HPP
#define CENTURY_C_BASECOMBATWEAPON_HPP

#include "C_BaseEntity.hpp"

enum WeaponProficiency_t
{
    WEAPON_PROFICIENCY_POOR = 0,
    WEAPON_PROFICIENCY_AVERAGE,
    WEAPON_PROFICIENCY_GOOD,
    WEAPON_PROFICIENCY_VERY_GOOD,
    WEAPON_PROFICIENCY_PERFECT,
};

class C_BaseCombatWeapon : public C_BaseEntity {
public:
    constexpr Vector GetBulletSpread() noexcept {
        return memory::Call<const Vector&>(this, 381);
    }

    /*constexpr const char* GetName() noexcept {
#if defined(__linux__)
        return memory::Call<const char*>(this, 417);
#else
        return memory::Call<const char*>(this, 354);
#endif
    }*/

    // gets print name
    constexpr const char* GetName() noexcept {
#if defined(__linux__)
        return memory::Call<const char*>(this, 418);
#else
        return memory::Call<const char*>(this, 355);
#endif
    }
};

#endif //CENTURY_C_BASECOMBATWEAPON_HPP
