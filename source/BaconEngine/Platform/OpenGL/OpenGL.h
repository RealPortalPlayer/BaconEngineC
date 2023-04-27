// Purpose: OpenGL entry point.
// Created on: 9/14/22 @ 11:53 AM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "../../AntiClientGuard.h"

SEC_CPP_SUPPORT_GUARD_START()
void BE_OpenGL_Initialize(void);
void BE_OpenGL_Destroy(void);
SEC_CPP_SUPPORT_GUARD_END()
