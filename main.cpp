#include "core/memory.hpp"
#include "core/modules.hpp"
#include "core/interfaces.hpp"
#include "core/netvars.hpp"
#include "core/utils.hpp"
#include "core/globals.hpp"
#include "core/luaexecution.hpp"
#include "core/events.hpp"

#include <polyhook2/Detour/x64Detour.hpp>

#if defined(__linux__)

#include "hooks/SwapWindow.hpp"
#include "hooks/PumpWindowsMessageLoop.hpp"
#include "hooks/SetKeyCodeState.hpp"

#define PUMPWINDOWMESSAGELOOP_INDEX 19
#define SETKEYCODESTATE_INDEX 84
#define CREATEMOVE_INDEX 22
#define DRAWMODEL_INDEX 33
#define FRAMESTAGENOTIFY_INDEX 35
#define OVERRIDEVIEW_INDEX 17

#else

#include "hooks/WndProc.hpp"
#include "hooks/EndScene.hpp"

#define ENDSCENE_INDEX 42
#define CREATEMOVE_INDEX 21
#define DRAWMODEL_INDEX 33
#define FRAMESTAGENOTIFY_INDEX 35
#define OVERRIDEVIEW_INDEX 16

#endif

#include "hooks/CreateMove.hpp"
#include "hooks/DrawModel.hpp"
#include "hooks/FrameStageNotify.hpp"
#include "hooks/OverrideView.hpp"
#include "hooks/TakeScreenshot.hpp"
#include "hooks/TakeJpeg.hpp"
#include "hooks/RenderCapture.hpp"
#include "hooks/CLMove.hpp"
#include "hooks/ShouldDrawLocalPlayer.hpp"

std::thread thread;

void dummyCallback(CConVar* conVar, const char* pOldValString, float flOldValue) {
    utils::Warn("pOldValString: {}; flOldValue: {}", pOldValString, flOldValue);
}

static void mainThread() {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    modules->Prepare();
    utils::ColorPrint(Color(189, 0, 255),"[century] Modules initialized!");

    interfaces->Prepare();
    utils::ColorPrint(Color(166, 255, 0),"[century] Interfaces initialized!");

    netVars->Prepare();
    utils::ColorPrint(Color(166, 255, 0),"[century] Netvars initialized!");

    luaExecution->Prepare();
    utils::ColorPrint(Color(50, 84, 168),"[century] Lua execution initialized!");

    events->Prepare();

    std::vector<std::unique_ptr<PLH::x64Detour>> hooks;

#if defined(__linux__)
    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::ResolvePLT(reinterpret_cast<std::uint64_t>(modules->GetExport(modules->sdl, "SDL_GL_SwapWindow"))),
        reinterpret_cast<std::uint64_t>(dtSwapWindow), &ogSwapWindow));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::GetVMTFunc<std::uint64_t>(interfaces->sdlMgr, PUMPWINDOWMESSAGELOOP_INDEX),
        reinterpret_cast<std::uint64_t>(dtPumpWindowsMessageLoop), &ogPumpWindowsMessageLoop));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::GetVMTFunc<std::uint64_t>(interfaces->inputInternal, SETKEYCODESTATE_INDEX),
        reinterpret_cast<std::uint64_t>(dtSetKeyCodeState), &ogSetKeyCodeState));
#else
    const auto d3d = Direct3DCreate9(D3D_SDK_VERSION);
    IDirect3DDevice9* d3dDevice = nullptr;
    
    D3DPRESENT_PARAMETERS pp = {};
    pp.Windowed = TRUE;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.hDeviceWindow = GetForegroundWindow();

    d3d->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        pp.hDeviceWindow,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &pp,
        &d3dDevice);

    if (d3dDevice) {
        ogWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(FindWindowA(0, "Garry's Mod (64-bit)"), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(dtWndProc)));

        hooks.push_back(std::make_unique<PLH::x64Detour>(
            memory::GetVMTFunc<std::uint64_t>(d3dDevice, ENDSCENE_INDEX),
            reinterpret_cast<std::uint64_t>(dtEndScene), &ogEndScene));
    }
#endif

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::GetVMTFunc<std::uint64_t>(interfaces->clientMode, CREATEMOVE_INDEX),
        reinterpret_cast<std::uint64_t>(dtCreateMove), &ogCreateMove));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::GetVMTFunc<std::uint64_t>(interfaces->studioRender, DRAWMODEL_INDEX),
        reinterpret_cast<std::uint64_t>(dtDrawModel), &ogDrawModel));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::GetVMTFunc<std::uint64_t>(interfaces->client, FRAMESTAGENOTIFY_INDEX),
        reinterpret_cast<std::uint64_t>(dtFrameStageNotify), &ogFrameStageNotify));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::GetVMTFunc<std::uint64_t>(interfaces->clientMode, OVERRIDEVIEW_INDEX),
        reinterpret_cast<std::uint64_t>(dtOverrideView), &ogOverrideView));

#if defined(__linux__)
    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::FindPattern("engine_client.so", "55 48 89 E5 41 55 49 89 FD 41 54 53 48 83 EC ? 48 85 FF C6 05"),
        reinterpret_cast<std::uint64_t>(dtTakeScreenshot), &ogTakeScreenshot));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::FindPattern("engine_client.so", "55 48 89 E5 41 55 41 54 49 89 FC 53 89 F3 48 83 EC ? 48 8B 05"),
        reinterpret_cast<std::uint64_t>(dtTakeJpeg), &ogTakeJpeg));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::FindPattern("client_client.so", "55 48 89 FE 48 89 E5 53 48 83 EC ? 48 8B 5F ? ? ? ? 48 89 DF FF 90 ? ? ? ? 48 83 C4 ? 48 89 DF 5B 5D E9 ? ? ? ? 90 90 90 90 90 90 55 48 89 E5 41 55"),
        reinterpret_cast<std::uint64_t>(dtRenderCapture), &ogRenderCapture));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::FindPattern("engine_client.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 48 F3 0F 11 45 9C 83 3D"),
        reinterpret_cast<std::uint64_t>(dtCLMove), &ogCLMove));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::FindPattern("client_client.so", "48 8B 05 ? ? ? ? 48 8B 50 ? 48 3B 50 ? 74 ? 48 3B 50 ? 48 8B 48 ? 74 ? 0F B6 42"),
        reinterpret_cast<std::uint64_t>(dtShouldDrawLocalPlayer), &ogShouldDrawLocalPlayer));
#else
    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::FindPattern("engine.dll", "48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8B EC 48 83 EC ? 48 8B 05"),
        reinterpret_cast<std::uint64_t>(dtTakeScreenshot), &ogTakeScreenshot));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::FindPattern("engine.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B F1"),
        reinterpret_cast<std::uint64_t>(dtTakeJpeg), &ogTakeJpeg));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::FindPattern("client.dll", "40 53 48 83 EC ? 48 8B 59 ? 48 8B D1 48 8B CB ? ? ? FF 90 ? ? ? ? 48 8B CB 48 83 C4 ? 5B E9 ? ? ? ? ? ? ? ? ? ? ? ? ? ? 48 89 5C 24 ? 57 48 83 EC ? 48 8B D1 48 8B 49 ? ? ? ? FF 90 ? ? ? ? 48 8B 0D ? ? ? ? ? ? ? FF 90"),
        reinterpret_cast<std::uint64_t>(dtRenderCapture), &ogRenderCapture));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::FindPattern("engine.dll", "40 55 53 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 0F 29 B4 24"),
        reinterpret_cast<std::uint64_t>(dtCLMove), &ogCLMove));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::FindPattern("client.dll", "48 83 EC ? 48 8B 0D ? ? ? ? 48 85 C9 74 ? 48 8D 51"),
        reinterpret_cast<std::uint64_t>(dtShouldDrawLocalPlayer), &ogShouldDrawLocalPlayer));
#endif

    if (!hooks.empty()) {
        for (const auto& hk : hooks) {
            hk->hook();
        }
    }

    const auto name = interfaces->cvar->FindCommandBase("name");
    name->RemoveFlags(FCVAR_ARCHIVE | FCVAR_PRINTABLEONLY | FCVAR_SERVER_CAN_EXECUTE);
    name->ChangeCallback(dummyCallback);

    const auto sv_maxupdaterate = interfaces->cvar->FindCommandBase("sv_maxupdaterate");
    sv_maxupdaterate->RemoveFlags(FCVAR_REPLICATED);

    const auto sv_maxcmdrate = interfaces->cvar->FindCommandBase("sv_maxcmdrate");
    sv_maxcmdrate->RemoveFlags(FCVAR_REPLICATED);

    interfaces->engineClient->ClientCmd_Unrestricted("play hl1/fvox/morphine_shot.wav");

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    utils::Warn("[century] Initialized in {}ms!", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    while (!interfaces->inputSystem->IsButtonDown(KEY_END) && !globals::Core::shouldUnload) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        globals::Core::localPlayer = utils::GetLocalPlayer();
    }

    interfaces->engineClient->ClientCmd_Unrestricted("play hl1/fvox/deactivated.wav");

    events->Unload();

    globals::Core::sendPacket = true;

    if (!hooks.empty()) {
        for (const auto& hk : hooks) {
            hk->unHook();
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    hooks.clear();

    utils::Print("[century] unloaded!");
}

#if defined(__linux__)

void __attribute__((constructor)) attach() {
    thread = std::thread(mainThread);
    thread.detach();
}

void __attribute__((destructor)) detach() {
    thread.join();
}

#else

int __stdcall DllMain(void*, std::uint32_t reason, void*) {
    if (reason == 1) {
        thread = std::thread(mainThread);
        thread.detach();
    }
    return 1;
}

#endif