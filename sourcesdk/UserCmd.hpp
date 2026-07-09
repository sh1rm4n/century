//
// Created by sh1rm4n on 5/17/26.
//

#ifndef CENTURY_USERCMD_HPP
#define CENTURY_USERCMD_HPP

#include "math.hpp"

#define IN_ATTACK  (1 << 0)
#define IN_JUMP   (1 << 1)
#define IN_DUCK   (1 << 2)
#define IN_FORWARD  (1 << 3)
#define IN_BACK   (1 << 4)
#define IN_USE   (1 << 5)
#define IN_CANCEL  (1 << 6)
#define IN_LEFT   (1 << 7)
#define IN_RIGHT  (1 << 8)
#define IN_MOVELEFT  (1 << 9)
#define IN_MOVERIGHT (1 << 10)
#define IN_ATTACK2  (1 << 11)
#define IN_RUN   (1 << 12)
#define IN_RELOAD  (1 << 13)
#define IN_ALT1   (1 << 14)
#define IN_ALT2   (1 << 15)
#define IN_SCORE  (1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED  (1 << 17) // Player is holding the speed key
#define IN_WALK   (1 << 18) // Player holding walk key
#define IN_ZOOM   (1 << 19) // Zoom key for HUD zoom
#define IN_WEAPON1  (1 << 20) // weapon defines these bits
#define IN_WEAPON2  (1 << 21) // weapon defines these bits
#define IN_BULLRUSH  (1 << 22)
#define IN_GRENADE1  (1 << 23) // grenade 1
#define IN_GRENADE2  (1 << 24) // grenade 2
#define IN_LOOKSPIN  (1 << 25)

class UserCmd {
public:
    int commandNumber;
    int tickCount;
    Angle viewAngles;
    float forwardMove;
    float sideMove;
    float upMove;
    int buttons;
    int weaponSelect;
    int weaponSubtype;
    int randomSeed;
    short mouseX;
    short mouseY;
    bool hasBeenPredicted;
    unsigned char impulse;
    bool isTyping;
    char pad_0001[6];
    char scrollWheelSpeed;
    bool contextMenu;
    Vector contextNormal;
    char pad_0002[240];

    constexpr std::uint32_t GetSeed() noexcept {
        return *reinterpret_cast<std::uint32_t*>(std::uint64_t(this) + 0x2C);
    }
};

#endif //CENTURY_USERCMD_HPP
