// Purpose: Manage important paths for the engine
// Created on: 6/30/2023 @ 10:39 AM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "Internal/CPlusPlusSupport.h"
#include "Internal/Boolean.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
const char* SEC_Paths_GetLauncherDirectory(void);
const char* SEC_Paths_GetEngineDirectory(void);
const char* SEC_Paths_GetClientDirectory(void);
const char* SEC_Paths_GetEngineBinaryPath(void);
const char* SEC_Paths_GetClientBinaryPath(void);

/**
 * @note Call free on result once finished to prevent memory leaks
 * @return False if it failed to allocate memory
 */
SEC_Boolean SEC_Paths_GetItemInsideEngineDirectory(char** result, const char* file);

/**
 * @note Call free on result once finished to prevent memory leaks
 * @return False if it failed to allocate memory
 */
SEC_Boolean SEC_Paths_GetItemInsideClientDirectory(char** result, const char* file);

void SEC_Paths_SetLauncherPath(const char* path);
void SEC_Paths_SetClientPath(const char* path);
void SEC_Paths_SetEnginePath(const char* path);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
