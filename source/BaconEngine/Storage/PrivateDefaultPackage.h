// Purpose: Functions to open/close the default package
// Created on: 8/8/23 @ 2:34 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>
#include <SharedEngineCode/Internal/Boolean.h>

#include "../AntiClientGuard.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_Boolean BE_PrivateDefaultPackage_Open(void);
SEC_Boolean BE_PrivateDefaultPackage_IsOpen(void);
void BE_PrivateDefaultPackage_Close(void);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
