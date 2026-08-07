//
// Created by sh1rm4n on 5/18/26.
//

#ifndef CENTURY_GLOBALS_HPP
#define CENTURY_GLOBALS_HPP

#pragma once

#include <imgui.h>

class C_BaseEntity;
class CUserCmd;
class Matrix3x4;

struct DrawEntity {
    std::string name;
    bool luaBased;
};

enum PlayerESPRenderingType {
    Square,
    Box
};

enum FakeLagMode {
    Static,
    Adaptive
};

#define CHAMS_SELFILLUM (1 << 0)
#define CHAMS_ADDITIVE (1 << 1)
#define CHAMS_NOFOG (1 << 2)
#define CHAMS_HALFLAMBERT (1 << 3)
#define CHAMS_MULTIPASS (1 << 4)
#define CHAMS_ENVMAP (1 << 5)
#define CHAMS_FLAT (1 << 6)
#define CHAMS_WIREFRAME (1 << 7)
#define CHAMS_NOCULL (1 << 8)

namespace globals {
    namespace Core {
        inline C_BaseEntity *localPlayer = nullptr;
        inline CUserCmd* currentCmd = nullptr;

        inline std::unordered_map<std::string, std::string> friends = {};

        inline bool shouldUnload = false;
        inline bool enginePrediction = false;
        inline bool sendPacket = true;

        inline int luaRealm = 0;
    }

    namespace Menu {
        inline ImFont* secondaryFont = nullptr;

        inline bool active = true;
    }

    namespace Hacks {
        namespace Chams {
            inline bool enabled = false;

            inline std::uint32_t flags = 0;

            inline std::string material = "models/shiny";

            inline float visibleColors[3] = {1.f, 1.f, 1.f};
            inline float hiddenColors[3] = {0.5f, 0.5f, 0.5f};
        }

        namespace EntityESP {
            inline bool enabled = false;

            inline std::vector<DrawEntity> entsToDraw;
            inline std::unordered_set<std::string> allowedEntsToDraw;
        }

        namespace PlayerESP {
            inline bool enabled = false;
            inline bool bones = false;

            inline PlayerESPRenderingType renderingType = Square;
        }

        namespace visuals_other {
            inline float fov = 100.f;
            inline float viewmodelFov = 60.f;
        }

        namespace Aimbot {
            inline bool enabled = false;
            inline bool bruteforce = false;

            inline bool teamCheck = false;

            inline bool silentAim = false;
            inline bool perfectSilent = false;

            inline float fov = 15.f;
            inline float smoothness = 1.f;
        }

        namespace ForwardTracking {
            inline bool enabled = false;

            inline float ghettoPredictionAmount = 2.5f;
        }

        namespace BackTracking {
            inline bool enabled = false;

            inline int ticks = 1;
        }

        namespace FakeLag {
            inline bool         enabled = false;

            inline int ticks            = 1;
            inline int mode             = Static;
        }

        namespace AntiAim {
            inline bool breakingLby = false;
        }

        namespace Killfeed {
            inline bool         enabled = false;

            inline std::vector<std::string> words = {};
        }
    }
}

#endif //CENTURY_GLOBALS_HPP
