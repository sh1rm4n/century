//
// Created by sh1rm4n on 7/10/26.
//

#include "FakeLag.hpp"

#include "BackTracking.hpp"
#include "../core/globals.hpp"
#include "../core/utils.hpp"
#include "../sourcesdk/entities/C_BaseEntity.hpp"

void FakeLag::Run(const int ticks) noexcept {
    if (!globals::Core::localPlayer)
        return;

    static auto fakeLagCounter = 0;

    const auto netChan = interfaces->engineClient->GetNetChannelInfo();
    if (!netChan) {
        globals::Core::sendPacket = true;
        return;
    }

    if (fakeLagCounter >= ticks) {
        globals::Core::sendPacket = true;

        lastServerPos = globals::Core::localPlayer->GetNetworkedOrigin();
        lastServerSimulationTime = globals::Core::localPlayer->GetSimulationTime();
        globals::Core::localPlayer->SetupBones(lastServerMatrix.data(), 256, 0x7FF00, lastServerSimulationTime);

        fakeLagCounter = 0;
    }
    else {
        /*if (globals::Core::localPlayer->GetFlags() & FL_ONGROUND) {
            globals::Core::sendPacket = true;
            return;
        }*/

        globals::Core::sendPacket = false;

        //netChan->chokedPackets -= netChan->chokedPackets + 1;

        fakeLagCounter++;
    }
}

Vector FakeLag::GetLastServerPos() noexcept {
    return lastServerPos;
}

double FakeLag::GetLastServerSimulationTime() noexcept {
    return lastServerSimulationTime;
}

std::array<Matrix3x4, 256> FakeLag::GetLastServerMatrix() noexcept {
    return lastServerMatrix;
}