#pragma once

#include <Windows.h>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

void drawMenu();

void visuals();
void aimbot();
void misc();
void hotkey(int* k, const ImVec2& size_arg = ImVec2(0, 0));

