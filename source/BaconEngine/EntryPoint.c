#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <string.h>
#include <SharedEngineCode/Internal/OperatingSystem.h>
#include <SharedEngineCode/ANSI.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <pthread.h>

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <unistd.h>
#   include <signal.h>
#   include <fcntl.h>
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   include <signal.h>
#   include <io.h>
#   define STDOUT_FILENO _fileno(stdout)
#   define write _write // HACK: This is a stupid idea, but it's the only way to make MSVC shut up.
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

SEC_CPP_SUPPORT_GUARD_START()
int BE_EntryPoint_ClientStart(int argc, char** argv);
int BE_EntryPoint_ClientShutdown(void);
int BE_EntryPoint_ClientSupportsServer(void);
const char* BE_EntryPoint_GetClientName(void);

void BE_EntryPoint_SignalDetected(int signal) {
    switch (signal) {
        case SIGSEGV:
        {
            static int antiDoubleSegfault = 0;

            if (SEC_Logger_GetLogLevel() > SEC_LOGGER_LOG_LEVEL_FATAL && !antiDoubleSegfault) {
                antiDoubleSegfault = 1;

                if (SEC_ANSI_IsEnabled()) {
                    write(STDOUT_FILENO, SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BOLD), 4);
                    write(STDOUT_FILENO, SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_FOREGROUND_RED), 7);
                }

                write(STDOUT_FILENO, "[FTL] ", 6);

                if (SEC_ANSI_IsEnabled())
                    write(STDOUT_FILENO, SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_RESET), 3);

                write(STDOUT_FILENO, "A segmentation fault was detected\n"
                                     "This means something tried to access bad memory (for example, tried to dereference a null pointer)\n"
                                     "If you're a user, then your only option is to report it to the developers\n"
                                     "If you're a developer, then you should start debugging your code\n"
                                     "If you suspect this is a problem with the engine, then please report it to us\n"
                                     "Users should avoid this client until further notice, as segmentation faults could be a sign of a lurking (and potentially dangerous) vulnerability\n"
                                     "Forcing a crash due to the program being in a unstable state\n", 558);
            }

            abort();
        }

        default:
            break;
    }
}

void* BE_EntryPoint_CommandThreadFunction(void* arguments) {
    int printedCursor = 0;

    // FIXME: Find out why this is not working on Serenity.
#if !SEC_OPERATINGSYSTEM_SERENITY
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
#endif

    while (BE_ClientInformation_IsRunning()) {
        if (BE_Console_GetCommandAmount() == 0)
            continue;

        char input[4024];

        memset(input, 0, 4024);

        if (BE_Renderer_GetCurrentType() == BE_RENDERER_TYPE_TEXT && !printedCursor) {
            SEC_Logger_LogImplementation(0, SEC_LOGGER_LOG_LEVEL_INFO, "%s ", BE_ClientInformation_IsCheatsEnabled() ? "#" : "$");

            printedCursor = 1;
        }

        fgets(input, sizeof(input), stdin); // TODO: Arrow keys to go back in history.

        input[strcspn(input, "\n")] = '\0';

        if (input[0] == '\0')
            continue;

        BE_Console_ExecuteCommand(input);

        printedCursor = 0;
    }

    return NULL;
}

int BE_EntryPoint_StartBaconEngine(int argc, char** argv) {
    static int alreadyStarted = 0;

    SEC_ArgumentHandler_Initialize(argc, argv);
    BE_STRICTMODE_CHECK(!alreadyStarted, 1, "Reinitializing the engine is not supported\n");
    SEC_LOGGER_TRACE("Entered client code\n");

    alreadyStarted = 1;

    SEC_LOGGER_INFO("Starting BaconEngine\n");

    if (BE_ClientInformation_IsServerModeEnabled() && !BE_EntryPoint_ClientSupportsServer()) {
        SEC_LOGGER_FATAL("This client does not support servers\n");
        return 1;
    }

    SEC_LOGGER_DEBUG("Registering signals\n");
    signal(SIGSEGV, BE_EntryPoint_SignalDetected);
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

        BE_PrivateWindow_Initialize(BE_EntryPoint_GetClientName(), SEC_CPP_SUPPORT_CREATE_STRUCT(BE_Vector_2U, (unsigned) width, (unsigned) height));
    }

    BE_PrivateUI_Initialize();
    BE_PrivateConsole_Initialize();
    BE_ASSERT(BE_EntryPoint_ClientStart(argc, argv) == 0, "Client start returned non-zero\n");

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

    pthread_t commandThread;

    pthread_create(&commandThread, NULL, &BE_EntryPoint_CommandThreadFunction, NULL);

    double deltaStart = 0;

    // TODO: Command running inside of terminal.

    while (BE_ClientInformation_IsRunning()) {
        if (!BE_Window_IsStillOpened()) {
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
    BE_ASSERT(BE_EntryPoint_ClientShutdown() == 0, "Client shutdown returned non-zero\n");
    SEC_LOGGER_INFO("Waiting for thread shutdown (press CTRL+C if frozen)\n");
    pthread_join(commandThread, NULL);
    SEC_LOGGER_DEBUG("Command thread ended\n");
    BE_PrivateLayer_DestroyLayers();
    BE_PrivateUI_Destroy();
    BE_PrivateConsole_Destroy();
    BE_PrivateWindow_Destroy();
    BE_SpecificPlatformFunctions_Get().Destroy();

    if (BE_EngineMemory_GetAllocatedBytes() > 0) {
        BE_EngineMemory_MemoryInformation memoryInformation = BE_EngineMemory_GetMemoryInformation();

        SEC_LOGGER_WARN("Memory leak detected:\n"
                        "Leaked: %lu bytes\n"
                        "Command: %lu allocated, %lu bytes\n"
                        "UI: %lu allocated, %lu bytes\n"
                        "DynamicArray: %lu allocated, %lu bytes\n"
                        "Layer: %lu allocated, %lu bytes\n",
                        BE_EngineMemory_GetAllocatedBytes(),
                        memoryInformation.command.allocatedAmount, memoryInformation.command.allocatedBytes,
                        memoryInformation.ui.allocatedAmount, memoryInformation.ui.allocatedBytes,
                        memoryInformation.dynamicArray.allocatedAmount, memoryInformation.dynamicArray.allocatedBytes,
                        memoryInformation.layer.allocatedAmount, memoryInformation.layer.allocatedBytes);
    }

    return 0;
}
SEC_CPP_SUPPORT_GUARD_END()
