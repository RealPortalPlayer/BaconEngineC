// Purpose: OpenGL Input
// Created on: 9/23/22 @ 12:18 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "BaconEngine/Input/Keyboard.h"
#include "BaconEngine/Input/Mouse.h"
#include "../../AntiClientGuard.h"

SEC_CPP_SUPPORT_GUARD_START()
SEC_Boolean BE_OpenGLInput_IsKeyDown(BE_Keyboard_KeyCodes keyCode);
SEC_Boolean BE_OpenGLInput_IsKeyPressed(BE_Keyboard_KeyCodes keyCode);
SEC_Boolean BE_OpenGLInput_IsMouseDown(BE_Mouse_ButtonType button);
SEC_CPP_SUPPORT_GUARD_END()
