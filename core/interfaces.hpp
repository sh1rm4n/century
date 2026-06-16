//
// Created by sh1rm4n on 5/16/26.
//

#ifndef CENTURY_INTERFACES_HPP
#define CENTURY_INTERFACES_HPP

#include <memory>
#include <string>

#include "../sourcesdk/interfaces/InputSystem.hpp"
#include "../sourcesdk/interfaces/HLClient.hpp"
#include "../sourcesdk/interfaces/ClientModeGModNormal.hpp"
#include "../sourcesdk/interfaces/CEngineClient.hpp"
#include "../sourcesdk/interfaces/CClientEntityList.hpp"
#include "../sourcesdk/interfaces/CSDLMgr.hpp"
#include "../sourcesdk/interfaces/CInputInternal.hpp"
#include "../sourcesdk/interfaces/IStudioRender.hpp"
#include "../sourcesdk/interfaces/IVModelInfo.hpp"
#include "../sourcesdk/interfaces/CModelLoader.hpp"
#include "../sourcesdk/interfaces/IMaterialSystem.hpp"
#include "../sourcesdk/interfaces/CClientLeafSystem.hpp"
#include "../sourcesdk/interfaces/CGlobalVars.hpp"
#include "../sourcesdk/interfaces/IVRenderView.hpp"
#include "../sourcesdk/interfaces/CModelRender.hpp"
#include "../sourcesdk/interfaces/CEngineTrace.hpp"
#include "../sourcesdk/interfaces/CLuaShared.hpp"
#include "../sourcesdk/interfaces/CGMODPrediction.hpp"
#include "../sourcesdk/interfaces/CGameMovement.hpp"
#include "../sourcesdk/interfaces/CMoveHelperClient.hpp"

class Interfaces {
public:
    template<typename T>
    T *GetInterface(void *module, const std::string &interfaceName) noexcept;

    void Prepare() noexcept;

    InputSystem *inputSystem = nullptr;
    HLClient *client = nullptr;
    ClientModeGModNormal *clientMode = nullptr;
    CEngineClient *engineClient = nullptr;
    CClientEntityList *entityList = nullptr;
    CSDLMgr *sdlMgr = nullptr;
    CInputInternal *inputInternal = nullptr;
    IStudioRender *studioRender = nullptr;
    IVModelInfo *modelInfo = nullptr;
    CModelLoader *modelLoader = nullptr;
    IMaterialSystem *materialSystem = nullptr;
    CClientLeafSystem *leafSystem = nullptr;
    CGlobalVars *globalVars = nullptr;
    IVRenderView *renderView = nullptr;
    CModelRender *modelRender = nullptr;
    CEngineTrace *engineTrace = nullptr;
    CLuaShared *luaShared = nullptr;
    CGMODPrediction *prediction = nullptr;
    CGameMovement *gameMovement = nullptr;
    CMoveHelperClient *moveHelper = nullptr;
};

inline auto interfaces = std::make_shared<Interfaces>();

#endif //CENTURY_INTERFACES_HPP
