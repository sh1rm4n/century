//
// Created by sh1rm4n on 5/18/26.
//

#ifndef CENTURY_GLOBALS_HPP
#define CENTURY_GLOBALS_HPP

#include <deque>
#include <unordered_set>
#include <vector>

#include "../sourcesdk/math.hpp"

class C_BaseEntity;

class Matrix3x4;

struct DrawEntity {
    std::string name;
    bool luaBased;
};

enum PlayerESPRenderingType {
    Square,
    Box
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

struct RealmEntry {
    std::string_view name;
    int              value;
};

namespace globals {
    namespace core {
        inline C_BaseEntity *localPlayer = nullptr;

        inline Matrix3x4 *viewMatrix = {};

        inline float screenWidth = 0.f;
        inline float screenHeight = 0.f;

        inline bool shouldUnload = false;

        inline int luaRealm = 0;
    }

    namespace menu {
        inline bool active = true;
    }

    namespace hacks {
        namespace chams {
            inline bool enabled = false;

            inline std::uint32_t flags = 0;

            inline std::string material = "models/shiny";

            inline float visibleColors[3] = {1.f, 1.f, 1.f};
            inline float hiddenColors[3] = {0.5f, 0.5f, 0.5f};
        }

        namespace entityesp {
            inline bool enabled = false;

            inline std::vector<DrawEntity> entsToDraw;
            inline std::unordered_set<std::string> allowedEntsToDraw;
        }

        namespace playeresp {
            inline bool enabled = false;
            inline bool bones = false;

            inline PlayerESPRenderingType renderingType = Box;
        }

        namespace visuals_other {
            inline float fov = 100.f;
            inline float viewmodelFov = 60.f;
        }

        namespace aimbot {
            inline bool enabled = false;
        }
    }
}

#endif //CENTURY_GLOBALS_HPP
