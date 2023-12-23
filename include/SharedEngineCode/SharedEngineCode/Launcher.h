// Purpose: Allows making launchers easier.
// Created on: 3/28/22 @ 9:54 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <limits.h>
#include <BaconAPI/Internal/OperatingSystem.h>
#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>

#include "Paths.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef enum {
    SEC_LAUNCHER_ERROR_CODE_NULL, // No error.
    SEC_LAUNCHER_ERROR_CODE_BINARY, // Errored while opening binary file.
    SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL // Errored attempting to load the entry point.
} SEC_Launcher_ErrorCodes;

typedef void (*SEC_Launcher_ClientInitialize)(const char*, const char*, const char*, void*, int, char**);
typedef int (*SEC_Launcher_ClientStart)(int, char**);
typedef int (*SEC_Launcher_ClientShutdown)(void);
typedef BA_Boolean (*SEC_Launcher_ClientSupportsServer)(void);
typedef const char* (*SEC_Launcher_ClientGetName)(void);
typedef const char* (*SEC_Launcher_ClientGetEngineVersion)(void);

// Do NOT edit this structure. Not even to move the variables around.
// Do NOT edit the typedef's these variables use.
// Doing so will break compatibility with older versions of the engine.
// If you absolutely need to add something, add it to the end of the structure.
typedef struct {
    int argc;
    char** argv;
    const char* launcherPath;
    const char* enginePath;
    const char* clientPath;
    void* engineBinary;
    SEC_Launcher_ClientInitialize clientInitialize;
    SEC_Launcher_ClientStart clientStart;
    SEC_Launcher_ClientShutdown clientShutdown;
    SEC_Launcher_ClientSupportsServer clientSupportsServer;
    SEC_Launcher_ClientGetName clientGetName;
    SEC_Launcher_ClientGetEngineVersion clientGetEngineVersion;
} SEC_Launcher_EngineDetails;

typedef struct {
    BA_Boolean success;
    union {
        int returnCode; // Set if success
        const char* errorMessage; // Set if it didn't succeed
    } unionVariables;
} SEC_Launcher_StartEngineResults;

typedef int (*SEC_Launcher_EngineStart)(const SEC_Launcher_EngineDetails*);

typedef struct {
    SEC_Launcher_ErrorCodes code;
    union {
        struct {
            const char* errorMessage; // The message from something like dlopen.
            BA_Boolean isEngine; // If the error code is related to the engine.
        } errorReason;
        struct {
            const char* clientName;
            void* engineBinary;
            void* clientBinary;
            SEC_Launcher_EngineStart Start;
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

SEC_Launcher_StartEngineResults SEC_Launcher_StartEngine(const SEC_Launcher_Configuration* configuration);

void SEC_Launcher_SetEnginePath(void);
void SEC_Launcher_SetLauncherPath(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
