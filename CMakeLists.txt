cmake_minimum_required(VERSION 4.3)
project(century)

# 1. Optimize Recompiles with ccache
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_library(century SHARED
        # =================================================================
        # DEPENDENCIES & THIRD-PARTY
        # =================================================================
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

        sourcesdk/typedefs.hpp
        sourcesdk/math.hpp
        sourcesdk/UserCmd.hpp
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

        # =================================================================
        # CORE APPLICATION CODE
        # =================================================================
        main.cpp
        core/globals.hpp
        core/utils.hpp
        core/memory.hpp
        core/modules.hpp
        core/modules.cpp
        core/interfaces.hpp
        core/interfaces.cpp
        core/menu.hpp
        core/pages/visuals/chams.hpp
        core/pages/visuals/entityesp.hpp
        core/pages/visuals/playeresp.hpp
        core/pages/visuals/other.hpp
        core/pages/aiming/aimbot.hpp
        core/pages/misc/network.hpp

        # =================================================================
        # HOOKS & HACKS
        # =================================================================
        hooks/CreateMove.hpp
        hooks/PumpWindowsMessageLoop.hpp
        hooks/SwapWindow.hpp
        hooks/SetKeyCodeState.hpp
        hooks/DrawModel.hpp
        hooks/FrameStageNotify.hpp
        hooks/OverrideView.hpp

        hacks/BunnyHop.hpp
        hacks/BunnyHop.cpp
        sourcesdk/interfaces/CModelRender.hpp
        sourcesdk/interfaces/CNetChan.hpp
        hooks/TakeScreenshot.hpp
        hooks/TakeJpeg.hpp
        hooks/RenderCapture.hpp
        3rd/imgui/imgui_notify/ImGuiNotify.hpp
        3rd/fonts/IconsFontAwesome6.h
        hacks/Aimbot.cpp
        hacks/Aimbot.hpp
        sourcesdk/interfaces/CEngineTrace.hpp
        core/luaexecution.hpp
        core/luaexecution.cpp
        sourcesdk/interfaces/CLuaShared.hpp
        core/pages/lua/executor.hpp
        3rd/imgui/coloredtextbox/dtl.h
        3rd/imgui/coloredtextbox/TextDiff.cpp
        3rd/imgui/coloredtextbox/TextDiff.h
        3rd/imgui/coloredtextbox/TextEditor.cpp
        3rd/imgui/coloredtextbox/TextEditor.h
)

#set(CMAKE_CXX_FLAGS_RELEASE "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -flto")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s")

target_compile_options(century PRIVATE -fno-gnu-unique -fno-rtti)
target_link_options(century PRIVATE $<$<CONFIG:RELEASE>:-s>)

# 2. Modern Linker & Compilation properties
set_target_properties(century PROPERTIES
        LINKER_TYPE MOLD
        UNITY_BUILD ON
        UNITY_BUILD_BATCH_SIZE 8
)

# 3. Reduce Header Overhead via PCH
target_precompile_headers(century PRIVATE
        <vector>
        <string>
        <format>
        <thread>
        <memory>
        <algorithm>
        <unordered_map>
        <fstream>
        <cstdint>
        <mutex>
        <dlfcn.h>
)

find_package(polyhook_2 CONFIG REQUIRED)
target_link_libraries(century PRIVATE PolyHook_2::PolyHook_2)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(century PRIVATE imgui::imgui)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(century
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

find_package(gl3w CONFIG REQUIRED)
target_link_libraries(century PRIVATE unofficial::gl3w::gl3w)