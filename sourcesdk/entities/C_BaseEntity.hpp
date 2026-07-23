//
// Created by sh1rm4n on 5/24/26.
//

#ifndef CENTURY_C_BASEENTITY_HPP
#define CENTURY_C_BASEENTITY_HPP

#include "../../core/utils.hpp"

class C_CollisionProperty;
class IClientUnknown;
class IClientRenderable;

class ClientClass;
class C_GMOD_Player;
class C_BaseCombatWeapon;
class CMultiPlayerAnimState;

class C_BaseEntity {
public:
    constexpr C_CollisionProperty* GetCollideable() noexcept {
        return memory::Call<C_CollisionProperty*>(this, 6);
    }

    constexpr IClientRenderable* GetClientRenderable() noexcept {
        return memory::Call<IClientRenderable*>(this, 6);
    }

    constexpr void SetDormant(const bool& dormant) noexcept {
        memory::Call<void>(this, 9, dormant);
    }

    constexpr ClientClass* GetClientClass() noexcept {
        return memory::Call<ClientClass*>(this, 17);
    }

    constexpr IClientUnknown* GetClientUnknown() noexcept {
        return memory::Call<IClientUnknown*>(this, 43);
    }

    constexpr void Think() noexcept {
        memory::Call<void>(this, 174);
    }

    const bool IsAlive() noexcept {
        return GetHealth() > 0;
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
        return memory::Call<const bool>(this, 234);
    }

    constexpr const char* GetLuaScriptName() noexcept {
        return memory::Call<const char*>(this, 245);
    }

    constexpr C_BaseCombatWeapon* GetActiveWeapon() noexcept {
        return memory::Call<C_BaseCombatWeapon*>(this, 343);
    }

    constexpr void PreThink() noexcept {
        memory::Call<void>(this, 374);
    }

    constexpr void PostThink() noexcept {
        memory::Call<void>(this, 376);
    }

    constexpr void SelectItem(const char* name, int subType) noexcept {
        memory::Call<void>(this, 384, name, subType);
    }

    constexpr void UpdateButtonState(const int buttons) noexcept {
        memory::Call<void>(this, 390, buttons);
    }

    constexpr void SetMouseWheel(const int speed) noexcept {
        memory::Call<void>(this, 437, speed);
    }

    constexpr double& GetSimulationTime() noexcept {
        return *reinterpret_cast<double*>(std::uint64_t(this) + utils::NetVar("DT_BaseEntity", "m_flSimulationTime"));
    }

    constexpr double& GetOldSimulationTime() noexcept {
        return *reinterpret_cast<double*>(std::uint64_t(this) + (utils::NetVar("DT_BaseEntity", "m_flSimulationTime") + 0x8));
    }

    constexpr std::int32_t& GetHealth() noexcept {
        return *reinterpret_cast<std::int32_t*>(std::uint64_t(this) + utils::NetVar("DT_BaseEntity", "m_iHealth"));
    }

    constexpr std::int32_t& GetMaxHealth() noexcept {
        return *reinterpret_cast<std::int32_t*>(std::uint64_t(this) + utils::NetVar("DT_BaseEntity", "m_iMaxHealth"));
    }

    constexpr std::int32_t& GetTeamNumber() noexcept {
        return *reinterpret_cast<std::int32_t*>(std::uint64_t(this) + utils::NetVar("DT_BaseEntity", "m_iTeamNum"));
    }

    constexpr Vector& GetViewOffset() noexcept {
        return *reinterpret_cast<Vector*>(std::uint64_t(this) + utils::NetVar("DT_BaseFlex", "m_vecViewOffset[0]"));
    }

    constexpr Vector& GetVelocity() noexcept {
        return *reinterpret_cast<Vector*>(std::uint64_t(this) + utils::NetVar("DT_BaseEntity", "m_vecVelocity[0]"));
    }

    constexpr MoveType& GetMoveType() noexcept {
        return *reinterpret_cast<MoveType*>(std::uint64_t(this) + utils::NetVar("DT_BaseEntity", "m_MoveType"));
    }

    constexpr C_CollisionProperty* GetCollisionProp() noexcept {
        return reinterpret_cast<C_CollisionProperty*>(std::uint64_t(this) + utils::NetVar("DT_BaseEntity", "m_Collision"));
    }

    constexpr Vector& GetNetworkedOrigin() noexcept {
        return *reinterpret_cast<Vector*>(std::uint64_t(this) + utils::NetVar("DT_BaseEntity", "m_vecOrigin"));
    }

    constexpr Angle& GetNetworkedAngles() noexcept {
        return *reinterpret_cast<Angle*>(std::uint64_t(this) + utils::NetVar("DT_BaseEntity", "m_angRotation"));
    }

    constexpr int& GetFlags() noexcept {
        return *reinterpret_cast<int*>(std::uint64_t(this) + utils::NetVar("DT_BasePlayer", "m_fFlags"));
    }

    constexpr void SetLocalViewAngles(const Angle& angles) noexcept {
        *reinterpret_cast<Angle*>(std::uint64_t(this) + 0x2BFC) = angles;
        *reinterpret_cast<Angle*>(std::uint64_t(this) + 0x2D0C) = angles;
    }

    constexpr void SetClientSideAnimation(const bool value) noexcept {
        *reinterpret_cast<bool*>(std::uint64_t(this) + 0x19E1) = value;
    }

    constexpr Matrix3x4* GetCachedBones() noexcept {
        return *reinterpret_cast<Matrix3x4**>(std::uint64_t(this) + 0x1A58);
    }

    constexpr void SetCachedBones(const Matrix3x4* bones) noexcept {
        *reinterpret_cast<const Matrix3x4**>(std::uint64_t(this) + 0x1A58) = bones;
    }

    constexpr int& GetButtons() noexcept {
        return *reinterpret_cast<int*>(std::uint64_t(this) + 0x2C28);
    }

    constexpr int& GetButtonsPressed() noexcept {
        return *reinterpret_cast<int*>(std::uint64_t(this) + 0x2C2C);
    }

    constexpr int& GetButtonsReleased() noexcept {
        return *reinterpret_cast<int*>(std::uint64_t(this) + 0x2C30);
    }

    constexpr int& GetLastButtons() noexcept {
        return *reinterpret_cast<int*>(std::uint64_t(this) + 0x2C34);
    }

    constexpr void SetCurrentUserCmd(CUserCmd* cmd) noexcept {
        *reinterpret_cast<CUserCmd**>(std::uint64_t(this) + 0x2C38) = cmd;
    }

    constexpr CUserCmd* GetCurrentUserCmd() noexcept {
        return *reinterpret_cast<CUserCmd**>(std::uint64_t(this) + 0x2C38);
    }

    constexpr void SetImpulse(const char impulse) noexcept {
        *reinterpret_cast<char*>(std::uint64_t(this) + 0x2CF8) = impulse;
    }

    constexpr int& GetTickBase() noexcept {
        return *reinterpret_cast<int*>(std::uint64_t(this) + utils::NetVar("DT_LocalPlayerExclusive", "m_nTickBase"));
    }

    constexpr CMultiPlayerAnimState* GetAnimState() noexcept {
        return *reinterpret_cast<CMultiPlayerAnimState**>(std::uint64_t(this) + 0x35B8);
    }

    constexpr Vector GetEyePosition() noexcept {
        return GetNetworkedOrigin() + GetViewOffset();
    }

    constexpr void SetAbsOrigin(const Vector& pos) noexcept {
        using _CBaseEntity__SetAbsOrigin = void(*)(C_BaseEntity*, const Vector&);
        static auto CBaseEntity__SetAbsOrigin = reinterpret_cast<_CBaseEntity__SetAbsOrigin>(memory::FindPattern("client_client.so", "55 48 89 E5 41 55 41 54 49 89 F4 53 48 89 FB 48 83 EC ? E8 ? ? ? ? ? ? ? ? ? ? 0F 2F 83"));

        CBaseEntity__SetAbsOrigin(this, pos);
    }

    constexpr void SetAbsAngles(const Angle& angles) noexcept {
        using _CBaseEntity__SetAbsAngles = void(*)(C_BaseEntity*, const Angle&);
        static auto CBaseEntity__SetAbsAngles = reinterpret_cast<_CBaseEntity__SetAbsAngles>(memory::FindPattern("client_client.so", "55 48 89 E5 41 57 41 56 41 55 41 54 49 89 F4 53 48 89 FB 48 81 EC ? ? ? ? E8"));

        CBaseEntity__SetAbsAngles(this, angles);
    }

    constexpr void SetAbsVelocity(const Vector& velocity) noexcept {
        using _CBaseEntity__SetAbsVelocity = void(*)(C_BaseEntity*, const Vector&);
        static auto CBaseEntity__SetAbsVelocity = reinterpret_cast<_CBaseEntity__SetAbsVelocity>(memory::FindPattern("client_client.so", "55 48 89 E5 41 55 41 54 49 89 F4 53 48 89 FB 48 83 EC ? ? ? ? ? 0F 2F 87"));

        CBaseEntity__SetAbsVelocity(this, velocity);
    }

    constexpr bool PhysicsRunThink(ThinkMethods thinkMethod) noexcept {
        using _CBaseEntity__PhysicsRunThink = bool(*)(C_BaseEntity*, ThinkMethods);
        static auto CBaseEntity__PhysicsRunThink = reinterpret_cast<_CBaseEntity__PhysicsRunThink>(memory::FindPattern("client_client.so", "55 48 8B 05 ? ? ? ? 48 89 E5 41 56 41 55 41 54 49 89 F4"));

        return CBaseEntity__PhysicsRunThink(this, thinkMethod);
    }

    constexpr void SetNextThink(const double nextThink) noexcept {
        using _CBaseEntity__SetNextThink = void(*)(C_BaseEntity*, int, double);
        static auto CBaseEntity__SetNextThink = reinterpret_cast<_CBaseEntity__SetNextThink>(memory::FindPattern("client_client.so", "55 48 89 E5 41 56 41 BE ? ? ? ? 41 55 45 31 ED"));

        CBaseEntity__SetNextThink(this, 0, nextThink);
    }

    // IClientRenderable (0x8)

    constexpr bool SetupBones(Matrix3x4* boneToWorldOut, const std::int32_t& maxBones, const std::int32_t& boneMask, double currentTime) noexcept {
        return memory::Call<const bool>(this + 0x8, 16, boneToWorldOut, maxBones, boneMask, currentTime);
    }

    constexpr model_t* GetModel() noexcept {
        return memory::Call<model_t*>(this + 0x8, 9);
    }

    // IClientNetworkable (0x10)

    constexpr bool IsDormant() noexcept {
        return memory::Call<bool>(this + 0x10, 8);
    }

    constexpr int EntIndex() noexcept {
        return memory::Call<int>(this + 0x10, 9);
    }

    constexpr C_GMOD_Player* ToPlayer() noexcept {
        return reinterpret_cast<C_GMOD_Player*>(this);
    }
};

#endif //CENTURY_C_BASEENTITY_HPP
