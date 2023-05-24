// Purpose: Allows making launchers easier.
// Created on: 3/28/22 @ 9:54 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <limits.h>

#include "Internal/OperatingSystem.h"
#include "Internal/CPlusPlusSupport.h"
#include "Internal/Boolean.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
typedef enum {
    SEC_LAUNCHER_ERROR_CODE_NULL, // No error.
    SEC_LAUNCHER_ERROR_CODE_BINARY, // Errored while opening binary file.
    SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL, // Errored attempting to load the entry point.
    SEC_LAUNCHER_ERROR_CODE_CHDIR // Errored while changing directories.
} SEC_Launcher_ErrorCodes;

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
            int (*Start)(void*, void*, int, char**);
        } data;
    } unionVariables;
} SEC_Launcher_Configuration;

const char* SEC_Launcher_GetDefaultHelpList(void);

/**
  * @return The initialized configuration file. NULL if malloc fails.
  */
void SEC_Launcher_CreateConfiguration(SEC_Launcher_Configuration* configuration, const char* path);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()

#define SEC_LAUNCHER_START_ENGINE_POINTER(configuration, argc, argv) (configuration)->unionVariables.data.Start((configuration)->unionVariables.data.engineBinary, (configuration)->unionVariables.data.clientBinary, argc, argv)
#define SEC_LAUNCHER_START_ENGINE(configuration, argc, argv) SEC_LAUNCHER_START_ENGINE_POINTER(&configuration, argc, argv)

