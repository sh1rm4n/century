//
// Created by sh1rm4n on 5/16/26.
//

#include "interfaces.hpp"

#include "modules.hpp"
#include "utils.hpp"
#include "memory.hpp"

template<typename T>
T *Interfaces::GetInterface(void *module, const std::string &interfaceName) noexcept {
    if (!module || interfaceName.empty()) {
        utils::Error("[century] Interface finding error: no module/interface.");
        return nullptr;
    }

    using _CreateInterface = void*(*)(const char *, int *);
    const auto CreateInterface = reinterpret_cast<_CreateInterface>(modules->GetExport(module, "CreateInterface"));
    if (!CreateInterface) {
        utils::Error("[century] Interface finding error: failed to get CreateInterface.");
        return nullptr;
    }

    const auto interface = CreateInterface(interfaceName.c_str(), 0);
    if (!interface) {
        utils::Error("[century] Interface finding error: interface isn't found.");
        return nullptr;
    }

    utils::Print("[century] Interface found: {}; address: {}", interfaceName, interface);

    return static_cast<T *>(interface);
}

void Interfaces::Prepare() noexcept {
    // fun fact: gcc compiler likes to do PLT and GOT global pointers with vtables

    inputSystem = GetInterface<InputSystem>(modules->inputSystem, "InputSystemVersion001");
    client = GetInterface<HLClient>(modules->client, "VClient017");
    clientMode = *reinterpret_cast<ClientModeGModNormal **>(memory::GetAbsAddress(
        memory::FindPattern("client_client.so",
                            "48 89 05 ? ? ? ? E8 ? ? ? ? 5D 48 8D 35 ? ? ? ? ? ? ? 48 89 C7 ? ? ? FF E2")));
    engineClient = GetInterface<CEngineClient>(modules->engine, "VEngineClient015");
    entityList = GetInterface<CClientEntityList>(modules->client, "VClientEntityList003");
    sdlMgr = *reinterpret_cast<CSDLMgr **>(memory::GetAbsAddress(
        memory::FindPattern("launcher_client.so", "48 8B 3D ? ? ? ? 48 85 FF")));
    inputInternal = GetInterface<CInputInternal>(modules->vgui2, "VGUI_InputInternal001");
    studioRender = GetInterface<IStudioRender>(modules->studioRender, "VStudioRender025");
    modelInfo = GetInterface<IVModelInfo>(modules->engine, "VModelInfoClient006");
    modelLoader = *reinterpret_cast<CModelLoader **>(memory::GetAbsAddress(
        memory::FindPattern("engine_client.so", "48 8B 05 ? ? ? ? 55 80 FA")));
    materialSystem = GetInterface<IMaterialSystem>(modules->materialSystem, "VMaterialSystem080");
    leafSystem = GetInterface<CClientLeafSystem>(modules->client, "ClientLeafSystem002");
    globalVars = *reinterpret_cast<CGlobalVars **>(memory::GetAbsAddress(memory::FindPattern(
        "client_client.so",
        "48 8B 05 ? ? ? ? F3 0F 10 48 ? F3 0F 11 4D ? E8 ? ? ? ? F3 0F 10 45 ? 48 89 C7 0F 5A C0")));
    renderView = GetInterface<IVRenderView>(modules->engine, "VEngineRenderView014");
    modelRender = GetInterface<CModelRender>(modules->engine, "VEngineModel016");
    engineTrace = GetInterface<CEngineTrace>(modules->engine, "EngineTraceClient003");
    luaShared = GetInterface<CLuaShared>(modules->luaShared, "LUASHARED003");
    prediction = GetInterface<CGMODPrediction>(modules->client, "VClientPrediction001");
    gameMovement = GetInterface<CGameMovement>(modules->client, "GameMovement001");
    cvar = GetInterface<CCvar>(modules->vstdlib, "VEngineCvar007");
    randomStream = GetInterface<CUniformRandomStream>(modules->engine, "VEngineRandom001");
    moveHelper = *reinterpret_cast<CMoveHelperClient **>(memory::ResolveGOT(memory::GetAbsAddress(memory::FindPattern(
        "client_client.so",
        "48 8B 05 ? ? ? ? 4C 89 85 ? ? ? ? 4C 8D B5"))));
    gameEventsManager = GetInterface<CGameEventsManager>(modules->engine, "GAMEEVENTSMANAGER002");
}