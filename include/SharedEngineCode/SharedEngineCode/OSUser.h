// Purpose: Binary specific information.
// Created on: 9/11/22 @ 1:26 PM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "Internal/CppSupport.h"

SEC_CPP_SUPPORT_GUARD_START()
int SEC_OSUser_IsAdmin(void);
const char* SEC_OSUser_GetUsername(void);
SEC_CPP_SUPPORT_GUARD_END()
