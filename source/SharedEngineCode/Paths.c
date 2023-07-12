// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <stdlib.h>

#include "SharedEngineCode/Paths.h"
#include "SharedEngineCode/Internal/PlatformSpecific.h"
#include "SharedEngineCode/Internal/Boolean.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
static const char* secPathsLauncherDirectory;
static const char* secPathsClientDirectory;
static const char* secPathsEngineDirectory;

static void SEC_Paths_Append(char** result, const char* string1, const char* string2) {
    *result = malloc(sizeof(char) * (strlen(string1) + strlen(string2) + 1));
    
    strcpy(*result, string1);
    strcat(*result, string2);
}

static SEC_Boolean SEC_Paths_IsAbsolute(const char* path) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    return path[0] == '/';
#elif SEC_OPERATINGSYSTEM_WINDOWS
    return path[0] == '/' || path[0] == '\\' || (strlen(path) >= 3 && path[1] == ':' && (path[2] == '/' || path[2] == '\\'));
#endif
}

// TODO: This is so complicated, try to simplify it
#define SEC_PATHS_GETDIRECTORY(path1, path2) \
static char* createdPath = NULL;             \
if (createdPath == NULL) {                   \
    if (SEC_Paths_IsAbsolute(path2)) {       \
        createdPath = malloc(sizeof(char) * (strlen(path2) + 1)); \
        strcpy(createdPath, path2);          \
    } else {                                 \
        char lastCharacter = path1[strlen(path1) - 1]; \
        if (lastCharacter != '/' && lastCharacter != '\\') { \
            SEC_Paths_Append(&createdPath, path1, "/"); \
            SEC_Paths_Append(&createdPath, createdPath, path2); \
        } else                               \
            SEC_Paths_Append(&createdPath, path1, path2); \
    }                                        \
    char lastCharacter = path2[strlen(path2) - 1]; \
    if (lastCharacter != '/' && lastCharacter != '\\') \
        SEC_Paths_Append(&createdPath, createdPath, "/"); \
}                                            \
return createdPath


const char* SEC_Paths_GetLauncherDirectory(void) {
    SEC_PATHS_GETDIRECTORY("/", secPathsLauncherDirectory);
}

const char* SEC_Paths_GetEngineDirectory(void) {
    SEC_PATHS_GETDIRECTORY(secPathsLauncherDirectory, secPathsEngineDirectory);
}

const char* SEC_Paths_GetClientDirectory(void) {
    SEC_PATHS_GETDIRECTORY(secPathsLauncherDirectory, secPathsClientDirectory);
}

#undef SEC_PATHS_GETDIRECTORY

const char* SEC_Paths_GetEngineBinaryPath(void) {
    static char* binaryPath = NULL;
    
    if (binaryPath == NULL)
        SEC_Paths_GetItemInsideEngineDirectory(&binaryPath, "BaconEngine" SEC_PLATFORMSPECIFIC_BINARY_EXTENSION);
    
    return binaryPath;
}

const char* SEC_Paths_GetClientBinaryPath(void) {
    static char* binaryPath = NULL;

    if (binaryPath == NULL) // FIXME: Assuming the binary name can cause problems for standalone builds
        SEC_Paths_GetItemInsideClientDirectory(&binaryPath, "Binary" SEC_PLATFORMSPECIFIC_BINARY_EXTENSION);

    return binaryPath;
}

void SEC_Paths_GetItemInsideEngineDirectory(char** result, const char* file) {
    SEC_Paths_Append(result, SEC_Paths_GetEngineDirectory(), file);
}

void SEC_Paths_GetItemInsideClientDirectory(char** result, const char* file) {
    SEC_Paths_Append(result, SEC_Paths_GetClientDirectory(), file);
}

void SEC_Paths_SetLauncherPath(const char* path) {
    secPathsLauncherDirectory = path;
}

void SEC_Paths_SetClientPath(const char* path) {
    secPathsClientDirectory = path;
}

void SEC_Paths_SetEnginePath(const char* path) {
    secPathsEngineDirectory = path;
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
