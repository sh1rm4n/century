//
// Created by sh1rm4n on 6/16/26.
//

#ifndef CENTURY_ENGINEPREDICTION_HPP
#define CENTURY_ENGINEPREDICTION_HPP

#include "../sourcesdk/UserCmd.hpp"

class EnginePrediction {
private:
    double oldCurTime = 0.f;
    float oldFrameTime = 0.f;
public:
    void Begin(UserCmd* cmd) noexcept;
    void End() noexcept;
};

inline auto enginePrediction = std::make_shared<EnginePrediction>();

#endif //CENTURY_ENGINEPREDICTION_HPP
