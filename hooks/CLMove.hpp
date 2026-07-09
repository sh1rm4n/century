//
// Created by sh1rm4n on 6/20/26.
//

#ifndef CENTURY_CLMOVE_HPP
#define CENTURY_CLMOVE_HPP

#include <polyhook2/Detour/x64Detour.hpp>

#include "../core/globals.hpp"

inline std::uint64_t ogCLMove = 0;

inline std::once_flag workerInit;

void dtCLMove(float accumulatedExtraSamples, bool finalTick)
{
    static auto ogFunction = PLH::FnCast(ogCLMove, dtCLMove);

    static bool shouldTick = false;

    if (interfaces->engineClient->IsInGame()) {
        static auto update = 0;
        if (update == 1) {
            shouldTick = true;
            update = 0;
        }
        else {
            shouldTick = true;
            update++;
            //return;
        }
    }
    else {
        shouldTick = true;
    }

    ogFunction(accumulatedExtraSamples, shouldTick);
}

#endif //CENTURY_CLMOVE_HPP
