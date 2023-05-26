// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <string.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <SharedEngineCode/Threads.h>
#include <SharedEngineCode/Internal/PlatformSpecific.h>

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <unistd.h>
#   include <signal.h>
#   include <fcntl.h>
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   include <signal.h>
#   include <io.h>
#   define fileno _fileno
#   define write(file, message, size) _write(file, message, (unsigned) size) // HACK: This is a stupid idea, but it's the only way to make MSVC shut up.
#endif

#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Rendering/Window.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/EngineMemoryInformation.h"
#include "Rendering/PrivateWindow.h"
#include "Platform/SpecificPlatformFunctions.h"
#include "Rendering/PrivateLayer.h"
#include "Rendering/PrivateUI.h"
#include "PrivateDeltaTime.h"
#include "Console/PrivateConsole.h"
#include "Rendering/PrivateRenderer.h"
#include "BaconEngine/Console/Console.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
void BE_EntryPoint_SignalDetected(int receivedSignal) {
    switch (receivedSignal) {
        case SIGSEGV:
            if (SEC_Logger_IsLevelEnabled(SEC_LOGGER_LOG_LEVEL_FATAL)) {
#define BE_ENTRYPOINT_SAFE_PUTS(message) write(fileno(stdout), message, strlen(message))
                SEC_Logger_LogHeader(stdout, SEC_LOGGER_LOG_LEVEL_FATAL);
                BE_ENTRYPOINT_SAFE_PUTS("A segmentation fault was detected\n"
                                        "This means something tried to access bad memory (for example, tried to dereference a null pointer)\n"
                                        "If you're a user, then your only option is to report it to the developers\n"
                                        "If you're a developer, then you should start debugging your code\n"
                                        "If you suspect this is a problem with the engine, then please report it to us\n"
                                        "Users should avoid this client until further notice, as segmentation faults could be a sign of a lurking (and potentially dangerous) vulnerability\n"
                                        "This client will now exit\n");
#undef BE_ENTRYPOINT_SAFE_PUTS
            }

            signal(SIGSEGV, SIG_DFL);
            raise(SIGSEGV);
            abort();

        case SIGINT:
            if (!BE_ClientInformation_IsRunning())
                return;

            // TODO: Windows?
#if SEC_OPERATINGSYSTEM_UNIX
            // TODO: Reprint cursor
            siginterrupt(SIGINT, SEC_BOOLEAN_FALSE);
#endif

            printf(" (type 'exit' to quit)\n");
            return;

        default:
            return;
    }
}

void BE_EntryPoint_CommandThreadFunction(void) {
    SEC_Boolean printedCursor = SEC_BOOLEAN_FALSE;

    // FIXME: Find out why this is not working on Serenity.
    // TODO: Find fcntl replacement for Windows.
#if !SEC_OPERATINGSYSTEM_SERENITY && !SEC_OPERATINGSYSTEM_WINDOWS
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
#endif

    while (BE_ClientInformation_IsRunning()) {
        if (BE_Console_GetCommandAmount() == 0)
            continue;

        char input[4024];

        memset(input, 0, 4024);

        if (BE_Renderer_GetCurrentType() == BE_RENDERER_TYPE_TEXT && !printedCursor) {
            SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, "%s ", BE_ClientInformation_IsCheatsEnabled() ? "#" : "$");

            printedCursor = SEC_BOOLEAN_TRUE;
        }

        fgets(input, sizeof(input), stdin); // TODO: Arrow keys to go back in history.

        input[strcspn(input, "\n")] = '\0';

        if (input[0] == '\0')
            continue;

        BE_Console_ExecuteCommand(input);

        printedCursor = SEC_BOOLEAN_FALSE;
    }
}

BE_BINARYEXPORT const char* BE_EntryPoint_GetVersion(void) {
    return BE_ENGINE_VERSION;
}

BE_BINARYEXPORT int BE_EntryPoint_StartBaconEngine(void* engineBinary, void* clientBinary, int argc, char** argv) {
    static SEC_Boolean alreadyStarted = SEC_BOOLEAN_FALSE;

    SEC_ArgumentHandler_Initialize(argc, argv);
    BE_STRICTMODE_CHECK(!alreadyStarted, 1, "Reinitializing the engine is not supported\n");
    SEC_LOGGER_TRACE("Entered engine code\n");

    alreadyStarted = SEC_BOOLEAN_TRUE;

#ifndef BE_ASAN_ENABLED
    SEC_LOGGER_DEBUG("Registering important signals\n");
    signal(SIGSEGV, BE_EntryPoint_SignalDetected);
#endif

    SEC_LOGGER_DEBUG("Getting client interface initializer\n");

    {
        typedef void (*ClientInitialize)(void*, int, char**);

        ClientInitialize initialize;

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
        *(void**) &initialize = SEC_PLATFORMSPECIFIC_GET_ADDRESS(clientBinary, "I_EntryPoint_InitializeWrapper");
#elif SEC_OPERATINGSYSTEM_WINDOWS
        initialize = (ClientInitialize) SEC_PLATFORMSPECIFIC_GET_ADDRESS(clientBinary, "I_EntryPoint_InitializeWrapper");
#endif

        if (initialize == NULL) {
            SEC_LOGGER_FATAL("Unable to find interface initializer\n");

            const char* errorMessage = NULL;

            SEC_PLATFORMSPECIFIC_GET_ERROR(errorMessage);
            SEC_LOGGER_FATAL("%s\n", errorMessage);
            abort();
        }

        initialize(engineBinary, argc, argv);
    }

    SEC_LOGGER_DEBUG("Getting client functions\n");

    typedef int (*ClientStart)(int, char**);
    typedef int (*ClientShutdown)(void);
    typedef SEC_Boolean (*ClientSupportsServer)(void);
    typedef const char* (*ClientGetName)(void);
    typedef const char* (*ClientGetEngineVersion)(void);

    ClientStart start;
    ClientShutdown shutdown;
    ClientSupportsServer supportsServer;
    ClientGetName getName;
    ClientGetEngineVersion getEngineVersion;

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    *(void**) &start = SEC_PLATFORMSPECIFIC_GET_ADDRESS(clientBinary, "I_EntryPoint_Start");
    *(void**) &shutdown = SEC_PLATFORMSPECIFIC_GET_ADDRESS(clientBinary, "I_EntryPoint_Shutdown");
    *(void**) &supportsServer = SEC_PLATFORMSPECIFIC_GET_ADDRESS(clientBinary, "I_EntryPoint_GetName");
    *(void**) &getName = SEC_PLATFORMSPECIFIC_GET_ADDRESS(clientBinary, "I_EntryPoint_GetName");
    *(void**) &getEngineVersion = SEC_PLATFORMSPECIFIC_GET_ADDRESS(clientBinary, "I_EntryPoint_GetEngineVersion");
#elif SEC_OPERATINGSYSTEM_WINDOWS
    start = (ClientStart) SEC_PLATFORMSPECIFIC_GET_ADDRESS(clientBinary, "I_EntryPoint_Start");
    shutdown = (ClientShutdown) SEC_PLATFORMSPECIFIC_GET_ADDRESS(clientBinary, "I_EntryPoint_Shutdown");
    supportsServer = (ClientSupportsServer) SEC_PLATFORMSPECIFIC_GET_ADDRESS(clientBinary, "I_EntryPoint_GetName");
    getName = (ClientGetName) SEC_PLATFORMSPECIFIC_GET_ADDRESS(clientBinary, "I_EntryPoint_GetName");
    getEngineVersion = (ClientGetEngineVersion) SEC_PLATFORMSPECIFIC_GET_ADDRESS(clientBinary, "I_EntryPoint_GetEngineVersion");
#endif

    if (start == NULL)
        SEC_LOGGER_DEBUG("Client has no start function\n");

    if (shutdown == NULL)
        SEC_LOGGER_DEBUG("Client has no shutdown function\n");

    if (supportsServer == NULL)
        SEC_LOGGER_DEBUG("Client doesn't specify if it supports servers, assuming it doesn't\n");

    if (getName == NULL)
        SEC_LOGGER_DEBUG("Client doesn't specify a name, defaulting to nothing\n");

    {
        const char* clientEngineVersion = getEngineVersion != NULL ? getEngineVersion() : BE_ENGINE_VERSION;

        if (strcmp(clientEngineVersion, BE_ENGINE_VERSION) == 0)
            SEC_LOGGER_INFO("Starting BaconEngine %s\n", BE_ENGINE_VERSION);
        else
            SEC_LOGGER_INFO("Starting BaconEngine %s (client was compiled with %s)\n", BE_ENGINE_VERSION, clientEngineVersion);
    }

    if (BE_ClientInformation_IsServerModeEnabled() && (supportsServer == NULL || !supportsServer())) {
        SEC_LOGGER_FATAL("Client does not support servers\n");
        return 1;
    }

    SEC_LOGGER_DEBUG("Registering signals\n");
    signal(SIGINT, BE_EntryPoint_SignalDetected);
    BE_PrivateRenderer_Initialize();
    BE_PrivateLayer_InitializeLayers();

    if (!BE_ClientInformation_IsServerModeEnabled() && BE_Renderer_GetCurrentType() != BE_RENDERER_TYPE_TEXT) {
        int width = 1080;
        int height = 720;

        {
            const char* preParsedWidth = SEC_ArgumentHandler_GetValue(SEC_BUILTINARGUMENTS_WIDTH, 0);
            const char* preParsedHeight = SEC_ArgumentHandler_GetValue(SEC_BUILTINARGUMENTS_HEIGHT, 0);

            if (preParsedWidth != NULL) {
                char* error;
                int parsedWith = (int) strtol(preParsedWidth, &error, 0);

                if (error != NULL && strlen(error) != 0) {
                    SEC_LOGGER_ERROR("Invalid width was supplied, ignoring...\n");

                    parsedWith = 1080;
                }

                width = parsedWith;
            }

            if (preParsedHeight != NULL) {
                char* error;
                int parsedHeight = (int) strtol(preParsedHeight, &error, 0);

                if (error != NULL && strlen(error) != 0) {
                    SEC_LOGGER_ERROR("Invalid height was supplied, ignoring...\n");

                    parsedHeight = 720;
                }

                height = parsedHeight;
            }
        }

        BE_PrivateWindow_Initialize(getName != NULL ? getName() : "", SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, (unsigned) width, (unsigned) height));
    }

    BE_PrivateUI_Initialize();
    BE_PrivateConsole_Initialize();
    BE_ASSERT(start == NULL || start(argc, argv) == 0, "Client start returned non-zero\n");

    {
        const char* preParsedExitCode = SEC_ArgumentHandler_GetValue(SEC_BUILTINARGUMENTS_EXIT, 0);

        if (preParsedExitCode != NULL) {
            char* error;
            int parsedExitCode = (int) strtol(preParsedExitCode, &error, 0);

            if (error != NULL && strlen(error) != 0) {
                SEC_LOGGER_ERROR("Invalid exit code, defaulting to 0\n");

                parsedExitCode = 0;
            }

            return parsedExitCode;
        }
    }

    SEC_LOGGER_INFO("Starting threads\n");

    SEC_Thread commandThread;

    SEC_Thread_Create(&commandThread, &BE_EntryPoint_CommandThreadFunction);

    double deltaStart = 0;

    // TODO: Command running inside of terminal.

    while (BE_ClientInformation_IsRunning()) {
        if (!BE_Window_IsStillOpened()) {
            if (BE_ClientInformation_IsRunning())
                BE_ClientInformation_StopRunning();

            break;
        }

        BE_Renderer_ClearScreen();

        double deltaNow = BE_SpecificPlatformFunctions_Get().GetTimer();
        double deltaTime = deltaNow - deltaStart;

        deltaStart = deltaNow;

        BE_PrivateDeltaTime_Setter(deltaTime);
        BE_PrivateLayer_OnUpdate(BE_LAYER_UPDATE_TYPE_BEFORE_RENDERING);
        BE_SpecificPlatformFunctions_Get().rendererFunctions.Render();
        BE_PrivateLayer_OnUpdate(BE_LAYER_UPDATE_TYPE_AFTER_RENDERING);
        BE_SpecificPlatformFunctions_Get().windowFunctions.UpdateEvents();
    }

    SEC_LOGGER_TRACE("Client loop ended, shutting down\n");
    BE_ASSERT(shutdown == NULL || shutdown() == 0, "Client shutdown returned non-zero\n");
    SEC_LOGGER_INFO("Waiting for thread shutdown (press CTRL+C if frozen)\n");
    SEC_Thread_Join(commandThread);
    SEC_LOGGER_DEBUG("Command thread ended\n");
    BE_PrivateLayer_DestroyLayers();
    BE_PrivateUI_Destroy();
    BE_PrivateConsole_Destroy();
    BE_PrivateWindow_Destroy();
    BE_SpecificPlatformFunctions_Get().Destroy();

    if (BE_EngineMemory_GetAllocatedBytes() > 0) {
        BE_EngineMemory_MemoryInformation memoryInformation = BE_EngineMemory_GetMemoryInformation();

        SEC_LOGGER_WARN("Memory leak detected:\n"
                        "Leaked: %zu bytes\n"
                        "Command: %zu allocated, %zu bytes\n"
                        "UI: %zu allocated, %zu bytes\n"
                        "DynamicArray: %zu allocated, %zu bytes\n"
                        "Layer: %zu allocated, %zu bytes\n",
                        BE_EngineMemory_GetAllocatedBytes(),
                        memoryInformation.command.allocatedAmount, memoryInformation.command.allocatedBytes,
                        memoryInformation.ui.allocatedAmount, memoryInformation.ui.allocatedBytes,
                        memoryInformation.dynamicArray.allocatedAmount, memoryInformation.dynamicArray.allocatedBytes,
                        memoryInformation.layer.allocatedAmount, memoryInformation.layer.allocatedBytes);
    }

    return 0;
}

BE_BINARYEXPORT void I_EntryPoint_InitializeWrapper(void* engineBinary) {
    (void) engineBinary;

    BE_ASSERT_ALWAYS("I am not a client\n");
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
