//
// Created by sh1rm4n on 7/10/26.
//

#include "FakeLag.hpp"

#include "BackTracking.hpp"
#include "../core/globals.hpp"
#include "../core/utils.hpp"
#include "../sourcesdk/entities/C_BaseEntity.hpp"

void FakeLag::Run(const int ticks) noexcept {
    static auto fakeLagCounter = 0;

    const auto netChan = interfaces->engineClient->GetNetChannelInfo();
    if (!netChan)
        return;

    if (fakeLagCounter >= ticks) {
        globals::Core::sendPacket = true;

        netChan->chokedPackets = 10;

        fakeLagCounter = 0;
    }
    else {
        globals::Core::sendPacket = false;

        fakeLagCounter++;
    }
}
