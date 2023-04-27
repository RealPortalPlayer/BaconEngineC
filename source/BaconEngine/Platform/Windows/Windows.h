// Purpose: Windows software entry-point.
// Created on: 10/9/22 @ 3:10 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "../../AntiClientGuard.h"

SEC_CPP_SUPPORT_GUARD_START()
void BE_Windows_Initialize(void);
void BE_Windows_Destroy(void);
SEC_CPP_SUPPORT_GUARD_END()
