//
// Created by sh1rm4n on 6/3/26.
//

#ifndef CENTURY_OVERRIDEVIEW_HPP
#define CENTURY_OVERRIDEVIEW_HPP

#include <polyhook2/Detour/x64Detour.hpp>

#include "../core/globals.hpp"
#include "../sourcesdk/entities/C_GMOD_Player.hpp"

class CViewSetup;

inline std::uint64_t ogOverrideView = 0;

void dtOverrideView(ClientModeGModNormal* vmt, CViewSetup* viewSetup) {
    static auto ogFunction = PLH::FnCast(ogOverrideView, dtOverrideView);
    static auto init = false;

    if (!globals::core::localPlayer)
        return ogFunction(vmt, viewSetup);

    if (!init) {
        globals::hacks::visuals_other::fov = viewSetup->fov;
        globals::hacks::visuals_other::viewmodelFov = viewSetup->fovViewmodel;

        init = true;
    }

    viewSetup->fov = globals::hacks::visuals_other::fov;
    viewSetup->fovViewmodel = globals::hacks::visuals_other::viewmodelFov;

    Angle angles;
    interfaces->engineClient->GetViewAngles(angles);
    viewSetup->angles = angles;

    Vector forward, right, up;
    angles.Vectors(forward, right, up);

    viewSetup->origin = globals::core::localPlayer->GetEyePosition() - ((forward * 100) / 2);

    ogFunction(vmt, viewSetup);
}

#endif //CENTURY_OVERRIDEVIEW_HPP
