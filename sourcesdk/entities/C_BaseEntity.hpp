//
// Created by sh1rm4n on 5/24/26.
//

#ifndef CENTURY_C_BASEENTITY_HPP
#define CENTURY_C_BASEENTITY_HPP

#include "../../core/memory.hpp"

#include "IClientRenderable.hpp"

class IClientUnknown;
class ClientClass;
class CCollisionProperty;
class C_GMOD_Player;

enum MoveType
{
    MOVETYPE_NONE		= 0,	// never moves
    MOVETYPE_ISOMETRIC,			// For players -- in TF2 commander view, etc.
    MOVETYPE_WALK,				// Player only - moving on the ground
    MOVETYPE_STEP,				// gravity, special edge handling -- monsters use this
    MOVETYPE_FLY,				// No gravity, but still collides with stuff
    MOVETYPE_FLYGRAVITY,		// flies through the air + is affected by gravity
    MOVETYPE_VPHYSICS,			// uses VPHYSICS for simulation
    MOVETYPE_PUSH,				// no clip to world, push and crush
    MOVETYPE_NOCLIP,			// No gravity, no collisions, still do velocity/avelocity
    MOVETYPE_LADDER,			// Used by players only when going onto a ladder
    MOVETYPE_OBSERVER,			// Observer movement, depends on player's observer mode
    MOVETYPE_CUSTOM,			// Allows the entity to describe its own physics

    // should always be defined as the last item in the list
    MOVETYPE_LAST		= MOVETYPE_CUSTOM,

    MOVETYPE_MAX_BITS	= 4
};

class C_BaseEntity {
public:
    constexpr IClientRenderable* GetClientRenderable() noexcept {
        return memory::Call<IClientRenderable*>(this, 6);
    }

    constexpr void SetDormant(const bool& dormant) noexcept {
        memory::Call<void>(this, 9, dormant);
    }

    constexpr Vector& GetAbsOrigin() noexcept {
        return memory::Call<Vector&>(this, 11);
    }

    constexpr Angle& GetAbsAngles() noexcept {
        return memory::Call<Angle&>(this, 12);
    }

    constexpr ClientClass* GetClientClass() noexcept {
        return memory::Call<ClientClass*>(this, 17);
    }

    constexpr IClientUnknown* GetClientUnknown() noexcept {
        return memory::Call<IClientUnknown*>(this, 43);
    }

    constexpr Vector& WorldAlignMins() noexcept {
        return memory::Call<Vector&>(this, 108);
    }

    constexpr Vector& WorldAlignMaxs() noexcept {
        return memory::Call<Vector&>(this, 109);
    }

    constexpr std::int32_t GetHealth() noexcept {
        return memory::Call<const std::int32_t>(this, 154);
    }

    constexpr std::int32_t GetMaxHealth() noexcept {
        return memory::Call<const std::int32_t>(this, 155);
    }

    constexpr bool IsAlive() noexcept {
        return memory::Call<const bool>(this, 183);
    }

    constexpr bool IsPlayer() noexcept {
        return memory::Call<const bool>(this, 184);
    }

    constexpr bool IsBaseCombatCharacter() noexcept {
        return memory::Call<const bool>(this, 185);
    }

    constexpr bool IsARagdoll() noexcept {
        return memory::Call<const bool>(this, 221);
    }

    constexpr bool IsWeapon() noexcept {
        return memory::Call<const bool>(this, 230);
    }

    constexpr bool UsesLua() noexcept {
        return memory::Call<const bool>(this, 232);
    }

    constexpr const char* GetLuaScriptName() noexcept {
        return memory::Call<const char*>(this, 244);
    }

    constexpr double GetSimulationTime() noexcept {
        return *reinterpret_cast<double*>(std::uint64_t(this) + 0xA0);
    }

    constexpr void SetRenderMode() noexcept {
        *reinterpret_cast<int*>(std::uint64_t(this) + 0xB2);
    }

    constexpr Vector& GetViewOffset() noexcept {
        return *reinterpret_cast<Vector*>(std::uint64_t(this) + 0x134);
    }

    constexpr Vector& GetVelocity() noexcept {
        return *reinterpret_cast<Vector*>(std::uint64_t(this) + 0x140);
    }

    constexpr MoveType GetMoveType() noexcept {
        return *reinterpret_cast<MoveType*>(std::uint64_t(this) + 0x1EC);
    }

    constexpr CCollisionProperty* GetCollisionProp() noexcept {
        return *reinterpret_cast<CCollisionProperty**>(std::uint64_t(this) + 0x220);
    }

    constexpr int& GetFlags() noexcept {
        return *reinterpret_cast<int*>(std::uint64_t(this) + 0x440);
    }

    constexpr Matrix3x4* GetCachedBones() noexcept {
        return *reinterpret_cast<Matrix3x4**>(std::uint64_t(this) + 0x1A70);
    }

    constexpr Vector GetEyePosition() noexcept {
        return GetAbsOrigin() + GetViewOffset();
    }

    // IClientRenderable (0x8)

    constexpr bool SetupBones(Matrix3x4* boneToWorldOut, const std::int32_t& maxBones, const std::int32_t& boneMask, double currentTime) noexcept {
        return memory::Call<const bool>(this + 0x8, 16, boneToWorldOut, maxBones, boneMask, currentTime);
    }

    constexpr model_t* GetModel() noexcept {
        return memory::Call<model_t*>(this + 0x8, 9);
    }

    constexpr C_GMOD_Player* ToPlayer() noexcept {
        return reinterpret_cast<C_GMOD_Player*>(this);
    }
};

#endif //CENTURY_C_BASEENTITY_HPP
