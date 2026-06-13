//
// Created by sh1rm4n on 5/16/26.
//

#include "modules.hpp"

#include <dlfcn.h>

void *Modules::GetModule(const std::string &moduleName) noexcept {
    if (moduleName.empty())
        return nullptr;

    const auto moduleAddr = dlopen(moduleName.c_str(), RTLD_NOW | RTLD_NOLOAD);
    if (!moduleAddr)
        return nullptr;

    return moduleAddr;
}

void *Modules::GetExport(void *module, const std::string &exportName) noexcept {
    if (exportName.empty())
        return nullptr;

    const auto exportAddr = dlsym(module, exportName.c_str());
    if (!exportAddr)
        return nullptr;

    return exportAddr;
}

void Modules::Prepare() noexcept {
    tier0 = GetModule("libtier0_client.so");
    inputSystem = GetModule("inputsystem_client.so");
    engine = GetModule("engine_client.so");
    client = GetModule("client_client.so");
    launcher = GetModule("launcher_client.so");
    materialSystem = GetModule("materialsystem_client.so");
    vgui2 = GetModule("vgui2_client.so");
    sdl = GetModule("libSDL2-2.0.so.0");
    studioRender = GetModule("studiorender_client.so");
    luaShared = GetModule("lua_shared_client.so");
}
