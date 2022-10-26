// Purpose: Stores information about the current client.
// Created on: 3/30/22 @ 11:59 PM

// Copyright (c) 2022, PortalPlayer
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

SEC_CPP_SUPPORT_GUARD_START()
int BE_ClientInformation_IsRunning(void);
int BE_ClientInformation_IsCheatsEnabled(void);
int BE_ClientInformation_IsServerModeEnabled(void);
int BE_ClientInformation_IsStrictModeEnabled(void);

void BE_ClientInformation_StopRunning(void);
void BE_ClientInformation_SetCheats(int enable);
SEC_CPP_SUPPORT_GUARD_END()
