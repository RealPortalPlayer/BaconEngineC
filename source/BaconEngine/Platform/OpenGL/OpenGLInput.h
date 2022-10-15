// Purpose: OpenGL Input
// Created on: 9/23/22 @ 12:18 PM

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "BaconEngine/Input/Keyboard.h"
#include "BaconEngine/Input/Mouse.h"

SEC_CPP_SUPPORT_GUARD_START()
int BE_OpenGLInput_IsKeyDown(BE_Keyboard_KeyCodes keyCode);
int BE_OpenGLInput_IsKeyUp(BE_Keyboard_KeyCodes keyCode);
int BE_OpenGLInput_IsKeyPressed(BE_Keyboard_KeyCodes keyCode);
int BE_OpenGLInput_IsMouseDown(BE_Mouse_ButtonType button);
SEC_CPP_SUPPORT_GUARD_END()