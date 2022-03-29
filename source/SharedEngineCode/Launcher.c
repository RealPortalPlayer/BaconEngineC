#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>

#include "SharedEngineCode/Launcher.h"

struct Configuration* GetConfiguration(const char* path) {
    struct Configuration* configuration = malloc(sizeof(struct Configuration));

    if (configuration == NULL)
        return NULL;

    chdir(path);

    configuration->clientBinary = dlopen("binary.dylib", RTLD_NOW);

    if (configuration->clientBinary == NULL) {
        configuration->success = 0;
        configuration->errorMessage = dlerror();
        return configuration;
    }

    const char* (*name)() = dlsym(configuration->clientBinary, "GetName");

    if (name == NULL) {
        configuration->success = 0;
        configuration->errorMessage = dlerror();
        return configuration;
    }

    configuration->clientName = name();
    configuration->Start = dlsym(configuration->clientBinary, "Start");

    if (configuration->Start == NULL) {
        configuration->success = 0;
        configuration->errorMessage = dlerror();
        return configuration;
    }

    configuration->success = 1;

    return configuration;
}