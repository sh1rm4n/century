//
// Created by sh1rm4n on 5/16/26.
//

#include "modules.hpp"

void *Modules::GetModule(const std::string &moduleName) noexcept {
    if (moduleName.empty())
        return nullptr;

#if defined(__linux__)
    const auto moduleAddr = dlopen(moduleName.c_str(), RTLD_NOW | RTLD_NOLOAD);
#else
    const auto moduleAddr = GetModuleHandleA(moduleName.c_str());
#endif

    if (!moduleAddr)
        return nullptr;

    return moduleAddr;
}

void *Modules::GetExport(void *module, const std::string &exportName) noexcept {
    if (exportName.empty())
        return nullptr;

#if defined(__linux__)
    const auto exportAddr = dlsym(module, exportName.c_str());
#else
    const auto exportAddr = static_cast<void*>(
        GetProcAddress(static_cast<HMODULE>(module), exportName.c_str()));
#endif

    if (!exportAddr)
        return nullptr;

    return exportAddr;
}

void Modules::Prepare() noexcept {
#if defined(__linux__)
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
    vstdlib = GetModule("libvstdlib_client.so");
#else
    tier0 = GetModule("tier0.dll");
    inputSystem = GetModule("inputsystem.dll");
    engine = GetModule("engine.dll");
    client = GetModule("client.dll");
    launcher = GetModule("launcher.dll");
    materialSystem = GetModule("materialsystem.dll");
    vgui2 = GetModule("vgui2.dll");
    //sdl = GetModule("libSDL2-2.0.so.0");
    studioRender = GetModule("studiorender.dll");
    luaShared = GetModule("lua_shared.dll");
    vstdlib = GetModule("vstdlib.dll");
#endif
}
