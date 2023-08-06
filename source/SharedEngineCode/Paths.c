// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <stdlib.h>

#include "SharedEngineCode/Paths.h"
#include "SharedEngineCode/Internal/PlatformSpecific.h"
#include "SharedEngineCode/Internal/Boolean.h"
#include "SharedEngineCode/String.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
static const char* secPathsLauncherDirectory;
static const char* secPathsClientDirectory;
static const char* secPathsEngineDirectory;

static SEC_Boolean SEC_Paths_IsAbsolute(const char* path) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    return path[0] == '/';
#elif SEC_OPERATINGSYSTEM_WINDOWS
    return path[0] == '/' || path[0] == '\\' || (strlen(path) >= 3 && path[1] == ':' && (path[2] == '/' || path[2] == '\\'));
#endif
}

#define SEC_PATHS_GETDIRECTORY(path) \
static char* createdPath = NULL;             \
if (createdPath == NULL) {                   \
    createdPath = SEC_String_Copy(secPathsLauncherDirectory); \
    SEC_String_Append(&createdPath, "/");    \
    SEC_String_Append(&createdPath, path);   \
    SEC_String_Append(&createdPath, "/");    \
}                                            \
return createdPath;

const char* SEC_Paths_GetLauncherDirectory(void) {
    return secPathsLauncherDirectory;
}

const char* SEC_Paths_GetEngineDirectory(void) {
    SEC_PATHS_GETDIRECTORY(secPathsEngineDirectory);
}

const char* SEC_Paths_GetClientDirectory(void) {
    SEC_PATHS_GETDIRECTORY(secPathsClientDirectory);
}

#undef SEC_PATHS_GETDIRECTORY

const char* SEC_Paths_GetEngineBinaryPath(void) {
    static char* binaryPath = NULL;
    
    if (binaryPath == NULL) {
        binaryPath = malloc(sizeof(char));
        
        SEC_Paths_GetItemInsideEngineDirectory(&binaryPath, "BaconEngine" SEC_PLATFORMSPECIFIC_BINARY_EXTENSION);
    }
    
    return binaryPath;
}

const char* SEC_Paths_GetClientBinaryPath(void) {
    static char* binaryPath = NULL;

    if (binaryPath == NULL) {
        binaryPath = malloc(sizeof(char));

        SEC_Paths_GetItemInsideClientDirectory(&binaryPath, "Binary" SEC_PLATFORMSPECIFIC_BINARY_EXTENSION);
    }
        
    return binaryPath;
}

void SEC_Paths_GetItemInsideEngineDirectory(char** result, const char* file) {
    SEC_String_Append(result, SEC_Paths_GetEngineDirectory());
    SEC_String_Append(result, file);
}

void SEC_Paths_GetItemInsideClientDirectory(char** result, const char* file) {
    SEC_String_Append(result, SEC_Paths_GetClientDirectory());
    SEC_String_Append(result, file);
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
