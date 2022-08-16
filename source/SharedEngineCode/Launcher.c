#include "SharedEngineCode/Launcher.h"

#include "SharedEngineCode/Internal/CppHeader.h"

#if SEC_OS_POSIX_COMPLIANT
#   include <dlfcn.h>
#   include <unistd.h>
#   define CHDIR(dir) chdir(dir)
#   define GET_BINARY(name, options) dlopen(name, options)
#   define GET_ADDRESS(binary, name) dlsym(binary, name)
#   define SET_ERROR() configuration->errorMessage = dlerror()
#   define BINARY_EXTENSION ".dylib"
#elif SEC_OS_WINDOWS
#   include <Windows.h>
#   include <direct.h>
#   define CHDIR(dir) _chdir(dir)
#   define GET_BINARY(name, options) LoadLibrary(name)
#   define GET_ADDRESS(binary, name) GetProcAddress(binary, name)
#   define SET_ERROR() configuration->errorMessage = NULL
#   define BINARY_EXTENSION ".dll"
#endif

CPP_GUARD_START()
    void SEC_CreateLauncherConfiguration(SEC_LauncherConfiguration* configuration, const char* path) {
        CHDIR(path);

        configuration->clientBinary = GET_BINARY("binary" BINARY_EXTENSION, RTLD_NOW);

        if (configuration->clientBinary == NULL) {
            configuration->code = SEC_LAUNCHER_ERROR_CODE_BINARY;

            SET_ERROR();
            return;
        }

        const char* (*name)(void) = (const char* (*)(void)) GET_ADDRESS(configuration->clientBinary, "BE_GetClientName");

        if (name == NULL) {
            configuration->code = SEC_LAUNCHER_ERROR_CODE_NAME_NULL;

            SET_ERROR();
            return;
        }

        configuration->clientName = name();
        configuration->Start = (int (*)(int, char**)) GET_ADDRESS(configuration->clientBinary, "BE_StartBaconEngine");

        if (configuration->Start == NULL) {
            configuration->code = SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL;
            SET_ERROR();
            return;
        }

        configuration->code = SEC_LAUNCHER_ERROR_CODE_NULL;
    }
CPP_GUARD_END()
