//
// Created by sh1rm4n on 6/20/26.
//

#ifndef CENTURY_CLMOVE_HPP
#define CENTURY_CLMOVE_HPP

#pragma once

#include <polyhook2/Detour/x64Detour.hpp>

#include "../core/globals.hpp"
#include "../core/interfaces.hpp"

inline std::uint64_t ogCLMove = 0;

void dtCLMove(float accumulatedExtraSamples, bool finalTick) {
    static auto ogFunction = PLH::FnCast(ogCLMove, dtCLMove);

    if (!interfaces->engineClient->IsInGame())
        return ogFunction(accumulatedExtraSamples, finalTick);

    if (globals::Hacks::Slowmotion::enabled) {
        static auto slowmoTicks = 0;
        if (slowmoTicks >= globals::Hacks::Slowmotion::ticks) {
            slowmoTicks = 0;
        }
        else {
            slowmoTicks++;
            return;
        }
    }

    ogFunction(accumulatedExtraSamples, globals::Core::sendPacket);
}

#endif //CENTURY_CLMOVE_HPP
