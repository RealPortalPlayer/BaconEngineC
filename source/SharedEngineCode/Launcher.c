#include <dlfcn.h>
#include <string.h>
#include <unistd.h>

#include "SharedEngineCode/Launcher.h"
#include "SharedEngineCode/Internal/CppHeader.h"

CPP_GUARD_START()
    void CreateLauncherConfiguration(LauncherConfiguration* configuration, const char* path) {
        chdir(path);

        configuration->clientBinary = dlopen("binary.dylib", RTLD_NOW);

        if (configuration->clientBinary == NULL) {
            configuration->code = LAUNCHER_ERROR_CODE_BINARY;
            configuration->errorMessage = dlerror();
            return;
        }

        const char* (*name)(void) = dlsym(configuration->clientBinary, "GetClientName");

        if (name == NULL) {
            configuration->code = LAUNCHER_ERROR_CODE_NAME_NULL;
            configuration->errorMessage = dlerror();
            return;
        }

        configuration->clientName = name();
        configuration->Start = dlsym(configuration->clientBinary, "StartBaconEngine");

        if (configuration->Start == NULL) {
            configuration->code = LAUNCHER_ERROR_CODE_ENTRY_NULL;
            configuration->errorMessage = dlerror();
            return;
        }

        configuration->code = LAUNCHER_ERROR_CODE_NULL;
    }
CPP_GUARD_END()