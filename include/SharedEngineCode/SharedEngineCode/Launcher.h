// Purpose: Allows making launchers easier.
// Created on: 3/28/22 @ 9:54 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <limits.h>

#include "Internal/OperatingSystem.h"
#include "Internal/CPlusPlusSupport.h"
#include "Internal/Boolean.h"
#include "Paths.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
typedef enum {
    SEC_LAUNCHER_ERROR_CODE_NULL, // No error.
    SEC_LAUNCHER_ERROR_CODE_BINARY, // Errored while opening binary file.
    SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL // Errored attempting to load the entry point.
} SEC_Launcher_ErrorCodes;

typedef int (*SEC_Launcher_ClientStart)(const char*, const char*, const char*, void*, void*, int, char**);

typedef struct SEC_Launcher_Configuration {
    SEC_Launcher_ErrorCodes code;
    union {
        struct {
            const char* errorMessage; // The message from something like dlopen.
            SEC_Boolean isEngine; // If the error code is related to the engine.
        } errorReason;
        struct {
            const char* clientName;
            void* engineBinary;
            void* clientBinary;
            SEC_Launcher_ClientStart Start;
        } data;
    } unionVariables;
} SEC_Launcher_Configuration;

const char* SEC_Launcher_GetDefaultHelpList(void);
void SEC_Launcher_CreateConfiguration(SEC_Launcher_Configuration* configuration, const char* path);

/**
 * Initializes engine binary, and gets entry-point for engine
 * @note SEC_Launcher_CreateConfiguration does this for you
 * @warning Relies that the engine path is set
 */
void SEC_Launcher_InitializeEngine(SEC_Launcher_Configuration* configuration);

/**
 * Initializes client binary
 * @note SEC_Launcher_CreateConfiguration does this for you
 * @warning Relies that the client path is set
 */
void SEC_Launcher_InitializeClient(SEC_Launcher_Configuration* configuration);

void SEC_Launcher_SetEnginePath(void);
void SEC_Launcher_SetLauncherPath(void);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()

#define SEC_LAUNCHER_START_ENGINE_POINTER(configuration, argc, argv) (configuration)->unionVariables.data.Start(SEC_Paths_GetLauncherDirectory(), SEC_Paths_GetEngineDirectory(), SEC_Paths_GetClientDirectory(), (configuration)->unionVariables.data.engineBinary, (configuration)->unionVariables.data.clientBinary, argc, argv)
#define SEC_LAUNCHER_START_ENGINE(configuration, argc, argv) SEC_LAUNCHER_START_ENGINE_POINTER(&configuration, argc, argv)
