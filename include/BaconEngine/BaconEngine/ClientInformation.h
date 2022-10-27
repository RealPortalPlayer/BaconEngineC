// Purpose: Stores information about the current client.
// Created on: 3/30/22 @ 11:59 PM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>
#include <SharedEngineCode/Internal/Boolean.h>

SEC_CPP_SUPPORT_GUARD_START()
SEC_Boolean BE_ClientInformation_IsRunning(void);
SEC_Boolean BE_ClientInformation_IsCheatsEnabled(void);
SEC_Boolean BE_ClientInformation_IsServerModeEnabled(void);
SEC_Boolean BE_ClientInformation_IsStrictModeEnabled(void);

void BE_ClientInformation_StopRunning(void);
void BE_ClientInformation_SetCheats(SEC_Boolean enable);
SEC_CPP_SUPPORT_GUARD_END()
