cmake_minimum_required(VERSION 4.3)

if(WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "")
endif()

project(century LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
else()
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
endif()

add_library(century SHARED main.cpp
        hacks/NoSpread.cpp
        hacks/NoSpread.hpp
        hooks/FireBullets.hpp
        hacks/NameStealer.cpp
        hacks/NameStealer.hpp)

if(LINUX)
    target_sources(century PRIVATE
            3rd/imgui/imgui_impl_sdl2.cpp
            3rd/imgui/imgui_impl_sdl2.h
            3rd/imgui/imgui_impl_opengl3.cpp
            3rd/imgui/imgui_impl_opengl3.h
            3rd/imgui/imgui_impl_opengl3_loader.h
            hooks/PumpWindowsMessageLoop.hpp
            hooks/SwapWindow.hpp
            hooks/SetKeyCodeState.hpp)
else()
    target_sources(century PRIVATE
            3rd/imgui/imgui_impl_dx9.cpp
            3rd/imgui/imgui_impl_dx9.h
            3rd/imgui/imgui_impl_win32.cpp
            3rd/imgui/imgui_impl_win32.h
            hooks/WndProc.hpp
            hooks/EndScene.hpp)
    target_link_libraries(century PRIVATE d3d9)
endif()

target_sources(century PRIVATE
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

        core/pages/accuracy.hpp
        core/pages/miscellaneous.hpp
        core/pages/lua.hpp

        hooks/CreateMove.hpp
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
        hacks/AntiAim.cpp
        hacks/AntiAim.hpp
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
        hooks/RunStringEx.hpp
        sourcesdk/interfaces/CLuaInterface.hpp
        hacks/ScriptHook.cpp
        hacks/ScriptHook.hpp
)

if(WIN32)
    target_compile_definitions(century PRIVATE
            NOMINMAX
            _WIN32_LEAN_AND_MEAN
            _CRT_SECURE_NO_WARNINGS)
endif()

if(LINUX)
    target_compile_options(century PRIVATE
            -Wreturn-type -Werror=return-type
            -O3
            -g3
            -mavx2
            -flto=thin
    )

    target_link_options(century PRIVATE
            -fuse-ld=lld
            -flto=thin
    )
else()
    target_compile_options(century PRIVATE
            /GR-
            /O2
            /arch:AVX2
            /clang:-flto=thin
    )

    target_link_options(century PRIVATE
            /fuse-ld=lld
            /clang:-flto=thin
    )
endif()

if(LINUX)
    target_precompile_headers(century PRIVATE
            <dlfcn.h>
            <unistd.h>
            <sys/mman.h>
    )
else()
    target_precompile_headers(century PRIVATE
            <Windows.h>
            <d3d9.h>
    )
endif()

target_precompile_headers(century PRIVATE
        <algorithm>
        <array>
        <cstdint>
        <random>
        <fstream>
        <memory>
        <mutex>
        <numbers>
        <string>
        <thread>
        <unordered_map>
        <unordered_set>
        <vector>
)

find_package(polyhook_2 REQUIRED)
find_package(imgui REQUIRED)
find_package(fmt REQUIRED)

if(LINUX)
    find_package(SDL2 REQUIRED)
    target_link_libraries(century PRIVATE SDL2::SDL2)
endif()

target_link_libraries(century PRIVATE
        PolyHook_2::PolyHook_2
        imgui::imgui
        fmt::fmt
)
