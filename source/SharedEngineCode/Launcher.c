#include "SharedEngineCode/Launcher.h"

#include "SharedEngineCode/Internal/CppHeader.h"

#if OS_POSIX_COMPLIANT
#   include <dlfcn.h>
#   include <unistd.h>
#   define CHDIR(dir) chdir(dir)
#   define GET_BINARY(name, options) dlopen(name, options)
#   define GET_ADDRESS(binary, name) dlsym(binary, name)
#   define SET_ERROR() configuration->errorMessage = dlerror()
#elif OS_WINDOWS
#   include <Windows.h>
#   include <direct.h>
#   define CHDIR(dir) _chdir(dir)
#   define GET_BINARY(name, options) LoadLibrary(name)
#   define GET_ADDRESS(binary, name) GetProcAddress(binary, name)
#   define SET_ERROR() configuration->errorMessage = NULL
#endif

CPP_GUARD_START()
    void CreateLauncherConfiguration(LauncherConfiguration* configuration, const char* path) {
        CHDIR(path);

        configuration->clientBinary = GET_BINARY("binary.dll", RTLD_NOW);

        if (configuration->clientBinary == NULL) {
            configuration->code = LAUNCHER_ERROR_CODE_BINARY;

            SET_ERROR();
            return;
        }

        const char* (*name)(void) = (const char* (*)(void)) GET_ADDRESS(configuration->clientBinary, "GetClientName");

        if (name == NULL) {
            configuration->code = LAUNCHER_ERROR_CODE_NAME_NULL;

            SET_ERROR();
            return;
        }

        configuration->clientName = name();
        configuration->Start = (int (*)(int, char**)) GET_ADDRESS(configuration->clientBinary, "StartBaconEngine");

        if (configuration->Start == NULL) {
            configuration->code = LAUNCHER_ERROR_CODE_ENTRY_NULL;
            SET_ERROR();
            return;
        }

        configuration->code = LAUNCHER_ERROR_CODE_NULL;
    }
CPP_GUARD_END()
