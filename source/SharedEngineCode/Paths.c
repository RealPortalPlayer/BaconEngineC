// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <stdlib.h>
#include <BaconAPI/Internal/PlatformSpecific.h>
#include <BaconAPI/Internal/Boolean.h>
#include <BaconAPI/String.h>

#include "SharedEngineCode/Paths.h"

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   define SEC_PATHS_SEPARATOR "/"
#else
#   define SEC_PATHS_SEPARATOR "\\"
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static const char* secPathsLauncherDirectory;
static const char* secPathsClientDirectory;
static const char* secPathsEngineDirectory;

static BA_Boolean SEC_Paths_IsAbsolute(const char* path) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    return path[0] == '/';
#elif BA_OPERATINGSYSTEM_WINDOWS
    return path[0] == '/' || path[0] == '\\' || (strlen(path) >= 3 && path[1] == ':' && (path[2] == '/' || path[2] == '\\'));
#endif
}

#define SEC_PATHS_GETDIRECTORY(path) \
static char* createdPath = NULL;     \
if (createdPath == NULL) {           \
    if (!SEC_Paths_IsAbsolute(path)) { \
        createdPath = BA_String_Copy(secPathsLauncherDirectory); \
        BA_String_Append(&createdPath, SEC_PATHS_SEPARATOR); \
        BA_String_Append(&createdPath, path); \
        BA_String_Append(&createdPath, SEC_PATHS_SEPARATOR); \
    } else {                         \
        createdPath = BA_String_Copy(path); \
        BA_String_Append(&createdPath, SEC_PATHS_SEPARATOR); \
    }                                \
}                                    \
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
        binaryPath = BA_String_CreateEmpty();
        
        if (binaryPath == NULL)
            return NULL;
        
        SEC_Paths_GetItemInsideEngineDirectory(&binaryPath, "BaconEngine" BA_PLATFORMSPECIFIC_BINARY_EXTENSION);
    }
    
    return binaryPath;
}

const char* SEC_Paths_GetClientBinaryPath(void) {
    static char* binaryPath = NULL;

    if (binaryPath == NULL) {
        binaryPath = BA_String_CreateEmpty();
        
        if (binaryPath == NULL)
            return NULL;
        
        SEC_Paths_GetItemInsideClientDirectory(&binaryPath, "Binary" BA_PLATFORMSPECIFIC_BINARY_EXTENSION);
    }
        
    return binaryPath;
}

BA_Boolean SEC_Paths_GetItemInsideEngineDirectory(char** result, const char* file) {
    return BA_String_Append(result, SEC_Paths_GetEngineDirectory()) != NULL &&
           BA_String_Append(result, file) != NULL;
}

BA_Boolean SEC_Paths_GetItemInsideClientDirectory(char** result, const char* file) {
    return BA_String_Append(result, SEC_Paths_GetClientDirectory()) != NULL &&
           BA_String_Append(result, file) != NULL;
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
BA_CPLUSPLUS_SUPPORT_GUARD_END()
