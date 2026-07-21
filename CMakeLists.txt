cmake_minimum_required(VERSION 4.3)

project(century LANGUAGES CXX)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(century SHARED
        3rd/imgui/imgui_impl_sdl2.cpp
        3rd/imgui/imgui_impl_sdl2.h
        3rd/imgui/imgui_impl_opengl3.cpp
        3rd/imgui/imgui_impl_opengl3.h
        3rd/imgui/imgui_impl_opengl3_loader.h
        3rd/imgui/imanim/im_anim.cpp
        3rd/imgui/imanim/im_anim.h
        3rd/imgui/imgui_toggle/imgui_offset_rect.h
        3rd/imgui/imgui_toggle/imgui_toggle.cpp
        3rd/imgui/imgui_toggle/imgui_toggle.h
        3rd/imgui/imgui_toggle/imgui_toggle_math.h
        3rd/imgui/imgui_toggle/imgui_toggle_palette.cpp
        3rd/imgui/imgui_toggle/imgui_toggle_palette.h
        3rd/imgui/imgui_toggle/imgui_toggle_presets.cpp
        3rd/imgui/imgui_toggle/imgui_toggle_presets.h
        3rd/imgui/imgui_toggle/imgui_toggle_renderer.cpp
        3rd/imgui/imgui_toggle/imgui_toggle_renderer.h
        3rd/imgui/imgui_notify/ImGuiNotify.hpp
        3rd/imgui/coloredtextbox/dtl.h
        3rd/imgui/coloredtextbox/TextDiff.cpp
        3rd/imgui/coloredtextbox/TextDiff.h
        3rd/imgui/coloredtextbox/TextEditor.cpp
        3rd/imgui/coloredtextbox/TextEditor.h
        3rd/fonts/IconsFontAwesome6.h

        sourcesdk/typedefs.hpp
        sourcesdk/math.hpp
        sourcesdk/CUserCmd.hpp
        sourcesdk/entities/IClientUnknown.hpp
        sourcesdk/entities/IClientRenderable.hpp
        sourcesdk/entities/C_BaseEntity.hpp
        sourcesdk/entities/C_GMOD_Player.hpp
        sourcesdk/interfaces/InputSystem.hpp
        sourcesdk/interfaces/HLClient.hpp
        sourcesdk/interfaces/CClientEntityList.hpp
        sourcesdk/interfaces/CEngineClient.hpp
        sourcesdk/interfaces/CSDLMgr.hpp
        sourcesdk/interfaces/ClientModeGModNormal.hpp
        sourcesdk/interfaces/CInputInternal.hpp
        sourcesdk/interfaces/IStudioRender.hpp
        sourcesdk/interfaces/IVModelInfo.hpp
        sourcesdk/interfaces/CModelLoader.hpp
        sourcesdk/interfaces/IMaterialSystem.hpp
        sourcesdk/interfaces/CClientLeafSystem.hpp
        sourcesdk/interfaces/ISpatialQuery.hpp
        sourcesdk/interfaces/CGlobalVars.hpp
        sourcesdk/interfaces/CCollisionProperty.hpp
        sourcesdk/interfaces/IVRenderView.hpp
        sourcesdk/interfaces/CModelRender.hpp
        sourcesdk/interfaces/CNetChan.hpp
        sourcesdk/interfaces/CEngineTrace.hpp
        sourcesdk/interfaces/CLuaShared.hpp

        core/globals.hpp
        core/utils.hpp
        core/memory.hpp
        core/modules.hpp
        core/modules.cpp
        core/interfaces.hpp
        core/interfaces.cpp
        core/netvars.cpp
        core/netvars.hpp
        core/menu.hpp
        core/luaexecution.hpp
        core/luaexecution.cpp

        core/pages/visuals/chams.hpp
        core/pages/visuals/entityesp.hpp
        core/pages/visuals/playeresp.hpp
        core/pages/visuals/other.hpp
        core/pages/accuracy.hpp
        core/pages/miscellaneous.hpp
        core/pages/lua.hpp

        hooks/CreateMove.hpp
        hooks/PumpWindowsMessageLoop.hpp
        hooks/SwapWindow.hpp
        hooks/SetKeyCodeState.hpp
        hooks/DrawModel.hpp
        hooks/FrameStageNotify.hpp
        hooks/OverrideView.hpp
        hooks/TakeScreenshot.hpp
        hooks/TakeJpeg.hpp
        hooks/RenderCapture.hpp

        hacks/BunnyHop.cpp
        hacks/BunnyHop.hpp
        hacks/Aimbot.cpp
        hacks/Aimbot.hpp

        main.cpp
        sourcesdk/interfaces/CGMODPrediction.hpp
        sourcesdk/interfaces/CGameMovement.hpp
        sourcesdk/interfaces/CMoveHelperClient.hpp
        3rd/checksum_md5.h
        sourcesdk/interfaces/CCvar.hpp
        sourcesdk/interfaces/CConVar.hpp
        hooks/CLMove.hpp
        3rd/fonts/robotoFont.hpp
        sourcesdk/entities/C_BaseCombatWeapon.hpp
        sourcesdk/interfaces/CUniformRandomStream.hpp
        sourcesdk/C_MultiPlayerAnimState.hpp
        3rd/fonts/firaCodeFont.hpp
        sourcesdk/entities/C_BaseViewModel.hpp
        hooks/ShouldDrawLocalPlayer.hpp
        core/pages/visuals.hpp
        core/pages/friends.hpp
        hooks/CalcRenderableWorldSpaceAABB.hpp
        hacks/AntiAim.cpp
        hacks/AntiAim.hpp
        sourcesdk/CLC_Move.hpp
        sourcesdk/tier1/bitbuf.h
        sourcesdk/tier0/basetypes.h
        sourcesdk/tier0/platform.h
        sourcesdk/tier1/bitbuf.cpp
        sourcesdk/coordsize.h
        sourcesdk/worldsize.h
        sourcesdk/tier0/annotations.h
        sourcesdk/tier1/bitvec.h
        hacks/Bruteforce.cpp
        hacks/Bruteforce.hpp
        sourcesdk/interfaces/CGameEventsManager.hpp
        core/events.hpp
        core/events.cpp
        hacks/FakeLag.cpp
        hacks/FakeLag.hpp
        hacks/ForwardTracking.cpp
        hacks/ForwardTracking.hpp
        hacks/BackTracking.cpp
        hacks/BackTracking.hpp
)

set_target_properties(century PROPERTIES
        LINKER_TYPE MOLD
        UNITY_BUILD ON
        UNITY_BUILD_BATCH_SIZE 8
)

target_compile_options(century PRIVATE
        -fno-gnu-unique
        -fno-rtti
        -O3
        -g3
        -march=native
        -mtune=native
)

target_precompile_headers(century PRIVATE
        <algorithm>
        <cstdint>
        <dlfcn.h>
        <unistd.h>
        <sys/mman.h>
        <format>
        <random>
        <fstream>
        <memory>
        <mutex>
        <string>
        <thread>
        <unordered_map>
        <unordered_set>
        <vector>
)

find_package(polyhook_2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

target_link_libraries(century PRIVATE
        PolyHook_2::PolyHook_2
        imgui::imgui
        fmt::fmt
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)