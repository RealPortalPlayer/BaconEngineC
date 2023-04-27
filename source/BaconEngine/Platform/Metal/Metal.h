// Purpose: Metal entry point.
// Created on: 1/1/23 @ 1:36 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "../../AntiClientGuard.h"

SEC_CPP_SUPPORT_GUARD_START()
void BE_Metal_Initialize(void);
void BE_Metal_Destroy(void);
SEC_CPP_SUPPORT_GUARD_END()
