// Purpose: OpenGL Input
// Created on: 9/23/22 @ 12:18 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>

#include "../../AntiClientGuard.h"
#include "BaconEngine/Input/Keyboard.h"
#include "BaconEngine/Input/Mouse.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Boolean BE_OpenGLInput_IsKeyDown(BE_Keyboard_KeyCodes keyCode);
BA_Boolean BE_OpenGLInput_IsKeyPressed(BE_Keyboard_KeyCodes keyCode);
BA_Boolean BE_OpenGLInput_IsMouseDown(BE_Mouse_ButtonType button);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
