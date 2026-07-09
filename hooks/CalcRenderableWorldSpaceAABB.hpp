//
// Created by sh1rm4n on 7/7/26.
//

#ifndef CENTURY_CALCRENDERABLEWORLDSPACEAABB_HPP
#define CENTURY_CALCRENDERABLEWORLDSPACEAABB_HPP

#include <polyhook2/Detour/x64Detour.hpp>

#include "../sourcesdk/math.hpp"

class IClientRenderable;

inline std::uint64_t ogCalcRenderableWorldSpaceAABB = 0;

void dtCalcRenderableWorldSpaceAABB(IClientRenderable* renderable, Vector& absMin, Vector& absMax) {
    static auto ogFunction = PLH::FnCast(ogCalcRenderableWorldSpaceAABB, dtCalcRenderableWorldSpaceAABB);

    //absMin = {-16384, -16384, -16384};
    //absMax = {16384, 16384, 16384};

    ogFunction(renderable, absMin, absMax);
}

#endif //CENTURY_CALCRENDERABLEWORLDSPACEAABB_HPP
