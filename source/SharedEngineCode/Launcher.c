#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>

#include "SharedEngineCode/Launcher.h"

#ifdef __cplusplus
extern "C" {
#endif

Configuration* GetConfiguration(const char* path) {
    Configuration* configuration = malloc(sizeof(Configuration));

    if (configuration == NULL)
        return NULL;

    chdir(path);

    configuration->clientBinary = dlopen("binary.dylib", RTLD_NOW);

    if (configuration->clientBinary == NULL) {
        configuration->code = ERROR_CODE_BINARY;
        configuration->errorMessage = dlerror();
        return configuration;
    }

    const char* (*name)() = dlsym(configuration->clientBinary, "GetName");

    if (name == NULL) {
        configuration->code = ERROR_CODE_NAME_NULL;
        configuration->errorMessage = dlerror();
        return configuration;
    }

    configuration->clientName = name();
    configuration->Start = dlsym(configuration->clientBinary, "Start");

    if (configuration->Start == NULL) {
        configuration->code = ERROR_CODE_ENTRY_NULL;
        configuration->errorMessage = dlerror();
        return configuration;
    }

    configuration->code = ERROR_CODE_NULL;

    return configuration;
}

#ifdef __cplusplus
};
#endif