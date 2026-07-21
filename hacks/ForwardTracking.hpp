//
// Created by sh1rm4n on 7/13/26.
//

#ifndef CENTURY_FORWARDTRACKING_HPP
#define CENTURY_FORWARDTRACKING_HPP

class CUserCmd;
class C_BaseEntity;
class Vector;

class ForwardTracking {
public:
    void Run(CUserCmd* cmd, C_BaseEntity* player) noexcept;
};

inline auto forwardTracking = std::make_unique<ForwardTracking>();

#endif //CENTURY_FORWARDTRACKING_HPP
