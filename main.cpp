#include "core/memory.hpp"
#include "core/modules.hpp"
#include "core/interfaces.hpp"
#include "core/utils.hpp"
#include "core/globals.hpp"

#include <format>
#include <thread>
#include <vector>
#include <memory>

#include <polyhook2/Detour/x64Detour.hpp>

#include "core/luaexecution.hpp"
#include "hooks/CreateMove.hpp"
#include "hooks/SwapWindow.hpp"
#include "hooks/PumpWindowsMessageLoop.hpp"
#include "hooks/SetKeyCodeState.hpp"
#include "hooks/DrawModel.hpp"
#include "hooks/FrameStageNotify.hpp"
#include "hooks/OverrideView.hpp"
#include "hooks/TakeScreenshot.hpp"
#include "hooks/TakeJpeg.hpp"
#include "hooks/RenderCapture.hpp"

std::thread thread;

static void mainThread() {
    modules->Prepare();
    utils::ColorPrint(Color(189, 0, 255),"[century] modules init!");

    interfaces->Prepare();
    utils::ColorPrint(Color(166, 255, 0),"[century] interfaces init!");

    luaExecution->Prepare();
    utils::ColorPrint(Color(50, 84, 168),"[century] lua execution init!");

    std::vector<std::unique_ptr<PLH::x64Detour>> hooks;

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::GetVMTFunc<std::uint64_t>(interfaces->clientMode, 22),
        reinterpret_cast<std::uint64_t>(dtCreateMove), &ogCreateMove));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::ResolvePLT(reinterpret_cast<std::uint64_t>(modules->GetExport(modules->sdl, "SDL_GL_SwapWindow"))),
        reinterpret_cast<std::uint64_t>(dtSwapWindow), &ogSwapWindow));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::GetVMTFunc<std::uint64_t>(interfaces->sdlMgr, 19),
        reinterpret_cast<std::uint64_t>(dtPumpWindowsMessageLoop), &ogPumpWindowsMessageLoop));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::GetVMTFunc<std::uint64_t>(interfaces->inputInternal, 84),
        reinterpret_cast<std::uint64_t>(dtSetKeyCodeState), &ogSetKeyCodeState));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::GetVMTFunc<std::uint64_t>(interfaces->studioRender, 33),
        reinterpret_cast<std::uint64_t>(dtDrawModel), &ogDrawModel));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::GetVMTFunc<std::uint64_t>(interfaces->client, 35),
        reinterpret_cast<std::uint64_t>(dtFrameStageNotify), &ogFrameStageNotify));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::GetVMTFunc<std::uint64_t>(interfaces->clientMode, 17),
        reinterpret_cast<std::uint64_t>(dtOverrideView), &ogOverrideView));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::FindPattern("engine_client.so", "55 48 89 E5 41 55 49 89 FD 41 54 53 48 83 EC ? 48 85 FF C6 05"),
        reinterpret_cast<std::uint64_t>(dtTakeScreenshot), &ogTakeScreenshot));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::FindPattern("engine_client.so", "55 48 89 E5 41 55 41 54 49 89 FC 53 89 F3 48 83 EC ? 48 8B 05"),
        reinterpret_cast<std::uint64_t>(dtTakeJpeg), &ogTakeJpeg));

    hooks.push_back(std::make_unique<PLH::x64Detour>(
        memory::FindPattern("client_client.so", "55 48 89 FE 48 89 E5 53 48 83 EC ? 48 8B 5F ? ? ? ? 48 89 DF FF 90 ? ? ? ? 48 83 C4 ? 48 89 DF 5B 5D E9 ? ? ? ? 90 90 90 90 90 90 55 48 89 E5 41 55"),
        reinterpret_cast<std::uint64_t>(dtRenderCapture), &ogRenderCapture));

    for (const auto& hk : hooks) {
        hk->hook();
    }

    interfaces->engineClient->ClientCmd_Unrestricted("play hl1/fvox/morphine_shot.wav");

    while (!interfaces->inputSystem->IsButtonDown(KEY_END) && !globals::core::shouldUnload) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //utils::Warn("localplayer: {}", (void*)globals::core::localPlayer);
    }

    interfaces->engineClient->ClientCmd_Unrestricted("play hl1/fvox/deactivated.wav");

    for (const auto& hk : hooks) {
        hk->unHook();
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

#elif defined(_WIN32)

int __stdcall DllMain(void*, std::uint32_t reason, void*) {
    if (reason == DLL_PROCESS_ATTACH) {
        thread = std::thread(mainThread);
        thread.detach();
    }
    return 1;
}

#endif