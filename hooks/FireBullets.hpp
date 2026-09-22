//
// Created by sh1rm4n on 7/30/26.
//

#ifndef CENTURY_FIREBULLETS_HPP
#define CENTURY_FIREBULLETS_HPP

#include <polyhook2/Detour/x64Detour.hpp>

#include "../sourcesdk/typedefs.hpp"

class C_BaseEntity;

#if defined(__linux__)
#define FIREBULLETS_INDEX 19
#else
#define FIREBULLETS_INDEX 16
#endif

inline std::unique_ptr<PLH::x64Detour> hkFireBullets = nullptr;
inline std::uint64_t ogFireBullets = 0;

void dtFireBullets(C_BaseEntity* vmt, FireBulletsInfo_t& info) {
    static auto ogFunction = PLH::FnCast(ogFireBullets, dtFireBullets);

    if (info.spread.x != 0 && info.spread.y != 0)
        noSpread->SetSpread(info.spread);

    utils::Warn("spread.x: {}; spread.y: {}", info.spread.x, info.spread.y);

    ogFunction(vmt, info);
}

#endif //CENTURY_FIREBULLETS_HPP
