// Purpose: Allows making launchers easier.
// Created on: 3/28/22 @ 9:54 PM

// Copyright (c) 2022, PortalPlayer
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <limits.h>

#include "SharedEngineCode/Internal/OperatingSystem.h"
#include "Internal/CppSupport.h"

SEC_CPP_SUPPORT_GUARD_START()
typedef enum {
    SEC_LAUNCHER_ERROR_CODE_NULL, // No error.
    SEC_LAUNCHER_ERROR_CODE_BINARY, // Errored while opening binary file.
    SEC_LAUNCHER_ERROR_CODE_NAME_NULL, // Errored attempting to load the name method.
    SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL, // Errored attempting to load the entry point.
    SEC_LAUNCHER_ERROR_CODE_CHDIR // Errored while changing directories.
} SEC_Launcher_ErrorCodes;

typedef struct SEC_Launcher_Configuration {
    SEC_Launcher_ErrorCodes code;
    union {
        const char* errorMessage; // The message from something like dlopen.
        struct {
            const char* clientName;
            void* clientBinary;
            int (*Start)(int, char**);
        };
    };
} SEC_Launcher_Configuration;

/**
  * @return The initialized configuration file. NULL if malloc fails.
  */
void SEC_Launcher_CreateConfiguration(SEC_Launcher_Configuration* configuration, const char* path);
const char* SEC_Launcher_GetDefaultHelpList(void);
SEC_CPP_SUPPORT_GUARD_END()
