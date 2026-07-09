//
// Created by sh1rm4n on 5/16/26.
//

#ifndef CENTURY_MODULES_HPP
#define CENTURY_MODULES_HPP

#pragma once

class Modules {
public:
    void *GetModule(const std::string &moduleName) noexcept;
    void *GetExport(void *module, const std::string &exportName) noexcept;
    void Prepare() noexcept;

    void *tier0 = nullptr;
    void *inputSystem = nullptr;
    void *engine = nullptr;
    void *client = nullptr;
    void *launcher = nullptr;
    void *materialSystem = nullptr;
    void *vgui2 = nullptr;
    void *sdl = nullptr;
    void *studioRender = nullptr;
    void *luaShared = nullptr;
    void *vstdlib = nullptr;
};

inline auto modules = std::make_shared<Modules>();

#endif //CENTURY_MODULES_HPP
