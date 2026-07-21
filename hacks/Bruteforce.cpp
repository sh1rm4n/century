//
// Created by sh1rm4n on 7/9/26.
//

#include "Bruteforce.hpp"

#include "../core/globals.hpp"
#include "../core/interfaces.hpp"
#include "../core/utils.hpp"

#include "../sourcesdk/math.hpp"
#include "../sourcesdk/entities/C_BaseEntity.hpp"
#include "../sourcesdk/interfaces/CGlobalVars.hpp"

Vector Bruteforce::Brute(C_BaseEntity* player, Vector bonePos) noexcept {
    if (!globals::Hacks::Aimbot::bruteforce)
        return bonePos;

    static std::array<float, 8> possibleYaws{
        45.f, -45.f, 90.f, -90.f, 135.f, -135.f, 180.f
    };

    const auto eyePos = player->GetEyePosition() - Vector{0,0,5};
    const auto randomYaw = possibleYaws[utils::Random<float>(0, possibleYaws.size())];

    float rad = math::Deg2Rad(randomYaw);

    float c = std::cos(rad);
    float s = std::sin(rad);

    Vector offset = bonePos - eyePos;

    Vector rotated{
        offset.x * c - offset.y * s,
        offset.x * s + offset.y * c,
        offset.z
    };

    Vector result = bonePos + rotated;

    return result;
}