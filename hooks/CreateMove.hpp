//
// Created by sh1rm4n on 5/17/26.
//

#ifndef CENTURY_CREATEMOVE_HPP
#define CENTURY_CREATEMOVE_HPP

#include <polyhook2/Detour/x64Detour.hpp>

#include "../core/engineprediction.hpp"
#include "../core/globals.hpp"

#include "../sourcesdk/UserCmd.hpp"

#include "../hacks/BunnyHop.hpp"
#include "../hacks/Aimbot.hpp"

inline std::uint64_t ogCreateMove = 0;

bool dtCreateMove(ClientModeGModNormal* vmt, UserCmd* cmd, float inputSampleTime) {
    static auto ogFunction = PLH::FnCast(ogCreateMove, dtCreateMove);

    const auto result = ogFunction(vmt, cmd, inputSampleTime);

    if (!cmd || cmd->commandNumber == 0)
        return result;

    cmd->isTyping = true;

    bhop->Run(cmd);

    enginePrediction->Begin(cmd);

    if (globals::hacks::aimbot::enabled)
        aimbot->Run(cmd);

    enginePrediction->End();

    return false;
};

#endif //CENTURY_CREATEMOVE_HPP
