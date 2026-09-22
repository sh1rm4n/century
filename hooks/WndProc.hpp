//
// Created by sh1rm4n on 7/23/26.
//

#ifndef CENTURY_WNDPROC_HPP
#define CENTURY_WNDPROC_HPP

#pragma once

#include <imgui.h>

#include "../core/globals.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

inline WNDPROC ogWndProc = nullptr;

LRESULT __stdcall dtWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static bool lastState = false;

    if (wParam == VK_HOME) {
        if (uMsg == WM_KEYDOWN && lastState == false) {
            lastState = true;
            globals::Menu::active = !globals::Menu::active;
        }
        else if (uMsg == WM_KEYUP) {
            lastState = false;
        }
    }

    if (globals::Menu::active)
        ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);

    return CallWindowProcA(ogWndProc, hwnd, uMsg, wParam, lParam);
}

#endif //CENTURY_WNDPROC_HPP
