// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <SharedEngineCode/OSUser.h>
#include <SharedEngineCode/Internal/PlatformSpecific.h>

#if SEC_OPERATINGSYSTEM_WINDOWS
#   include <stdio.h>
#   include <Windows.h>
#endif

SEC_CPP_SUPPORT_GUARD_START()
int main(int argc, char** argv) {
    SEC_ArgumentHandler_Initialize(argc, argv);
    SEC_LOGGER_TRACE("Built on: %s\nBuilt for: %s\n", __TIMESTAMP__, SEC_OPERATINGSYSTEM_NAME);

    if (SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_VERSION, SEC_BUILTINARGUMENTS_VERSION_SHORT, 0)) {
        SEC_LOGGER_INFO("Engine version: 0.1\n");
        return 0;
    }

    if (SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_HELP, SEC_BUILTINARGUMENTS_HELP_SHORT, 0)) {
        SEC_LOGGER_INFO("Arguments:\n%s\n", SEC_Launcher_GetDefaultHelpList());
        return 0;
    }

    if (SEC_OSUser_IsAdmin())
        SEC_LOGGER_WARN("You're running as root! If a client says you require to be root, then it's probably a virus\n");

    SEC_LOGGER_DEBUG("Getting engine\n");

    void* engineBinary = NULL;

    {
        SEC_ArgumentHandler_ShortResults results;

        if (SEC_ArgumentHandler_GetInfoWithShort(SEC_BUILTINARGUMENTS_ENGINE, SEC_BUILTINARGUMENTS_ENGINE_SHORT, SEC_FALSE, &results) != 0)
            engineBinary = SEC_PLATFORMSPECIFIC_GET_BINARY(*results.value, RTLD_NOW);
        else
            engineBinary = SEC_PLATFORMSPECIFIC_GET_BINARY("./BaconEngine" SEC_PLATFORMSPECIFIC_BINARY_EXTENSION, RTLD_NOW);
    }

    if (engineBinary == NULL) {
        const char* errorMessage = NULL;

        SEC_PLATFORMSPECIFIC_GET_ERROR(errorMessage);
        SEC_LOGGER_FATAL("Failed to get engine binary: %s\n", errorMessage);
        return 1;
    }

    SEC_LOGGER_DEBUG("Getting engine entry-point\n");

    int (*start)(void*, void*, int, char**) = (int (*)(void*, void*, int, char**)) SEC_PLATFORMSPECIFIC_GET_ADDRESS(engineBinary, "BE_EntryPoint_StartBaconEngine");

    if (start == NULL) {
        const char* errorMessage = NULL;

        SEC_PLATFORMSPECIFIC_GET_ERROR(errorMessage);
        SEC_LOGGER_FATAL("Failed to get engine entry-point: %s\n", errorMessage);
        return 1;
    }

    void* clientBinary = SEC_PLATFORMSPECIFIC_GET_BINARY(NULL, RTLD_NOW);
    const char* (*getName)(void) = (const char* (*)(void)) SEC_PLATFORMSPECIFIC_GET_ADDRESS(clientBinary, "I_EntryPoint_GetName");

    {
        const char* name = getName != NULL ? getName() : "";

        if (name[0] != '\0')
            SEC_LOGGER_INFO("Ready, starting: %s\n", name);
        else
            SEC_LOGGER_INFO("Ready, starting");
    }

    SEC_LOGGER_TRACE("Entering engine code\n");

    int returnValue = start(engineBinary, clientBinary, argc, argv);

    SEC_LOGGER_TRACE("Returned back to launcher\n");
    SEC_LOGGER_TRACE("Freeing engine binary\n");

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    dlclose(engineBinary);
#elif SEC_OPERATINGSYSTEM_WINDOWS
    FreeLibrary(engineBinary);
#endif

    SEC_LOGGER_INFO("Goodbye\n");
    return returnValue;
}
SEC_CPP_SUPPORT_GUARD_END()
