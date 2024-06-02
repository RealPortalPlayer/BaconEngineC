// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/Internal/OperatingSystem.h>

#if BA_OPERATINGSYSTEM_WINDOWS
#   include <ws2tcpip.h>
#endif

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Logger.h>
#include <string.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <BaconAPI/Thread.h>
#include <BaconAPI/Internal/PlatformSpecific.h>
#include <SharedEngineCode/Paths.h>
#include <SharedEngineCode/Launcher.h>
#include <BaconAPI/Debugging/Assert.h>
#include <SharedEngineCode/Debugging/StrictMode.h>
#include <errno.h>
#include <BaconAPI/Number.h>

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <unistd.h>
#   include <signal.h>
#   include <fcntl.h>
#   include <netinet/in.h>
#   include <arpa/inet.h>
#elif BA_OPERATINGSYSTEM_WINDOWS
#   include <signal.h>
#   include <io.h>
#   include <Windows.h>
#   define fileno _fileno
#   define write(file, message, size) _write(file, message, (unsigned) size) // HACK: This is a stupid idea, but it's the only way to make MSVC shut up.
#endif

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
#include "Storage/PrivateDefaultPackage.h"
#include "BaconEngine/Client/Information.h"
#include "BaconEngine/Server/Server.h"
#include "Server/PrivateServer.h"
#include "Server/PrivatePacket.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Boolean printedCursor = BA_BOOLEAN_FALSE;
BA_Boolean commandThreadRunning = BA_BOOLEAN_FALSE;

void BE_EntryPoint_SignalDetected(int receivedSignal) {
    switch (receivedSignal) {
        case SIGSEGV:
            if (BA_Logger_IsLevelEnabled(BA_LOGGER_LOG_LEVEL_FATAL)) {
#define BE_ENTRYPOINT_SAFE_PUTS(message) write(fileno(stdout), message, strlen(message))
                BA_Logger_LogHeader(stdout, BA_LOGGER_LOG_LEVEL_FATAL);
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
            ((void (*)(void)) NULL)();
            abort();

        case SIGINT:
            if (!BE_ClientInformation_IsRunning() || !commandThreadRunning) {
                if (!commandThreadRunning)
                    BA_LOGGER_WARN("Command thread is not running, using default behavior\n");

                signal(SIGINT, SIG_DFL);
                raise(SIGINT);
                return;
            }

#ifndef BE_NO_ENGINE_COMMANDS
            if (BA_Logger_IsLevelEnabled(BA_LOGGER_LOG_LEVEL_INFO))
                printf(" (type 'exit' to quit)");
#endif
            
            printf("\n");
            printedCursor = BA_BOOLEAN_FALSE;
            return;

        default:
            return;
    }
}

BA_THREAD_RETURN_VALUE BE_EntryPoint_CommandThreadFunction(void* argument) {
    commandThreadRunning = BA_BOOLEAN_TRUE;

    // FIXME: Find out why this is not working on Serenity.
    // TODO: Find fcntl replacement for Windows.
#if !BA_OPERATINGSYSTEM_SERENITY && !BA_OPERATINGSYSTEM_WINDOWS
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
#endif

    BA_LOGGER_DEBUG("Command thread started\n");

    while (BE_ClientInformation_IsRunning()) {
        if (BE_Console_GetCommandAmount() == 0)
            continue;

        char input[4024];

        memset(input, 0, 4024);

        if (BE_Renderer_GetCurrentType() == BE_RENDERER_TYPE_TEXT && !printedCursor) {
            BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, "%s ", BE_ClientInformation_IsCheatsEnabled() ? "#" : "$");

            printedCursor = BA_BOOLEAN_TRUE;
        }

        fgets(input, sizeof(input), stdin); // TODO: Arrow keys to go back in history.

        if (input[0] == '\0')
            continue;
        
        input[strcspn(input, "\n")] = '\0';

        if (input[0] != '\0')
            BE_Console_ExecuteCommand(input, BE_CLIENT_UNCONNECTED);

        printedCursor = BA_BOOLEAN_FALSE;
    }

    commandThreadRunning = BA_BOOLEAN_FALSE;

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    return NULL;
#else
    return 0;
#endif
}

#ifndef BE_DISABLE_NETWORK
BA_THREAD_RETURN_VALUE BE_EntryPoint_ServerThreadFunction(void* argument) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    fcntl(BE_PrivateServer_GetSocketDescriptor(), F_SETFL, O_NONBLOCK);
#elif BA_OPERATINGSYSTEM_WINDOWS
    {
        ULONG enable = 1;
        
        ioctlsocket(BE_PrivateServer_GetSocketDescriptor(), FIONBIO, &enable);
    }
#endif
    
    while (BE_ClientInformation_IsRunning()) {
        struct sockaddr_in clientInterface;
        socklen_t clientSize = sizeof(clientInterface);
        BE_PrivatePacket_Sent packet = {0};
        ssize_t packetLength = recvfrom(BE_PrivateServer_GetSocketDescriptor(), &packet, sizeof(BE_PrivatePacket_Sent), 0, (struct sockaddr*) &clientInterface, &clientSize);

        if (packetLength == -1) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
            if (errno == EWOULDBLOCK)
                continue;
#elif BA_OPERATINGSYSTEM_WINDOWS
            if (WSAGetLastError() == WSAEWOULDBLOCK)
                continue;
#endif
            
            BA_LOGGER_ERROR("Errored while getting packet from client: %s\n", strerror(errno));
            continue;
        }

        if (packetLength != sizeof(BE_PrivatePacket_Sent)) {
            BA_LOGGER_ERROR("Invalid packet: size mismatch (%zu != %zu)\n", packetLength, sizeof(BE_PrivatePacket_Sent));
            continue; // TODO: Disconnect
        }
        
        BE_PrivatePacket_Parse(BE_PrivateServer_GetPrivateClientFromSocket(&clientInterface), &clientInterface, packet);
    }

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    return NULL;
#else
    return 0;
#endif
}
#endif

BE_BINARYEXPORT const char* BE_EntryPoint_GetVersion(void) {
    return BE_ENGINE_VERSION;
}

BE_BINARYEXPORT int BE_EntryPoint_StartBaconEngine(const SEC_Launcher_EngineDetails* engineDetails) {
    static BA_Boolean alreadyStarted = BA_BOOLEAN_FALSE;

    BA_ArgumentHandler_Initialize(engineDetails->argc, engineDetails->argv);
    SEC_Paths_SetLauncherPath(engineDetails->launcherPath);
    SEC_Paths_SetEnginePath(engineDetails->enginePath);
    SEC_Paths_SetClientPath(engineDetails->clientPath);
    SEC_STRICTMODE_CHECK(!alreadyStarted, 1, "Reinitializing the engine is not supported\n");
    BA_LOGGER_TRACE("Entered engine code\n");

    alreadyStarted = BA_BOOLEAN_TRUE;

#ifndef BE_ASAN_ENABLED
    BA_LOGGER_DEBUG("Registering important signals\n");
    signal(SIGSEGV, BE_EntryPoint_SignalDetected);
#endif

    BA_LOGGER_DEBUG("Initializing client interface\n");
    
    engineDetails->clientInitialize(engineDetails->launcherPath, engineDetails->enginePath, engineDetails->clientPath, engineDetails->engineBinary, engineDetails->argc, engineDetails->argv);
    
    if (engineDetails->clientStart == NULL)
        BA_LOGGER_DEBUG("Client has no start function\n");

    if (engineDetails->clientShutdown == NULL)
        BA_LOGGER_DEBUG("Client has no shutdown function\n");

    if (engineDetails->clientSupportsServer == NULL)
        BA_LOGGER_DEBUG("Client doesn't specify if it supports servers, assuming it doesn't\n");

    if (engineDetails->clientGetName == NULL)
        BA_LOGGER_DEBUG("Client doesn't specify a name, defaulting to nothing\n");

    {
        const char* clientEngineName = engineDetails->clientGetName != NULL ? engineDetails->clientGetEngineName() : BE_ENGINE_NAME;
        const char* clientEngineVersion = engineDetails->clientGetEngineVersion != NULL ? engineDetails->clientGetEngineVersion() : BE_ENGINE_VERSION;
        
        BA_LOGGER_INFO("Starting %s %s", BE_ENGINE_NAME, BE_ENGINE_VERSION);

        if (strcmp(clientEngineName, BE_ENGINE_NAME) != 0 || strcmp(clientEngineVersion, BE_ENGINE_VERSION) != 0)
            BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, " (client was compiled with %s %s)", clientEngineName, clientEngineVersion);

        BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, "\n");
    }

    if (BE_ClientInformation_IsServerModeEnabled() && (engineDetails->clientSupportsServer == NULL || !engineDetails->clientSupportsServer())) {
        BA_LOGGER_FATAL("Client does not support servers\n");
        return 1;
    }

    if (BE_ClientInformation_IsServerModeEnabled() && BA_Thread_IsSingleThreaded()) {
        BA_LOGGER_FATAL("Single-threaded server is not supported\n");
        return 1;
    }

    BE_PrivateDefaultPackage_Open();
    BE_PrivateRenderer_Initialize();

    if (BE_ClientInformation_IsServerModeEnabled()) {
        BA_ArgumentHandler_ShortResults portResults;
        int port = 5000;

        if (BA_ArgumentHandler_GetInformationWithShort(SEC_BUILTINARGUMENTS_PORT, SEC_BUILTINARGUMENTS_PORT_SHORT, BA_BOOLEAN_FALSE, &portResults) != 0)
            port = BA_Number_StringToInteger(*portResults.value, NULL, NULL, "Invalid port was supplied, defaulting to 5000\n", port);

        BE_Server_Start(port);
    }

    BE_PrivateLayer_InitializeLayers();

    if (!BE_ClientInformation_IsServerModeEnabled() && BE_Renderer_GetCurrentType() != BE_RENDERER_TYPE_TEXT) {
        int width = 1080;
        int height = 720;

        {
            const char* preParsedWidth = BA_ArgumentHandler_GetValue(SEC_BUILTINARGUMENTS_WIDTH, 0);
            const char* preParsedHeight = BA_ArgumentHandler_GetValue(SEC_BUILTINARGUMENTS_HEIGHT, 0);

            if (preParsedWidth != NULL)
                width = BA_Number_StringToInteger(preParsedWidth, NULL, NULL, "Invalid width was supplied, ignoring...\n", width);

            if (preParsedHeight != NULL)
                height = BA_Number_StringToInteger(preParsedHeight, NULL, NULL, "Invalid height was supplied, ignoring...\n", height);
        }

        BE_PrivateWindow_Initialize(engineDetails->clientGetName != NULL ? engineDetails->clientGetName() : "", BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, (unsigned) width, (unsigned) height));
    }

    BE_PrivateUI_Initialize();
    BE_PrivateConsole_Initialize();

#ifndef BE_DISABLE_NETWORK
    BE_PrivatePacket_Initialize();
#endif
    
    BA_ASSERT(engineDetails->clientStart == NULL || engineDetails->clientStart(engineDetails->argc, engineDetails->argv) == 0, "Client start returned non-zero\n");
    BA_LOGGER_DEBUG("Registering signals\n");
    signal(SIGINT, &BE_EntryPoint_SignalDetected);

    {
        const char* preParsedExitCode = BA_ArgumentHandler_GetValue(SEC_BUILTINARGUMENTS_EXIT, 0);

        if (preParsedExitCode != NULL)
            return BA_Number_StringToInteger(preParsedExitCode, NULL, NULL, "Invalid exit code, defaulting to 0\n", 0);
    }

    BA_Thread commandThread;

    if (!BA_Thread_IsSingleThreaded()) {
        BA_LOGGER_INFO("Starting threads\n");

        BA_Thread_Create(&commandThread, &BE_EntryPoint_CommandThreadFunction, NULL);
    }

#ifndef BE_DISABLE_NETWORK
    BA_Thread serverThread;

    if (BE_ClientInformation_IsServerModeEnabled())
        BA_Thread_Create(&serverThread, &BE_EntryPoint_ServerThreadFunction, NULL);
#endif
    
    double deltaStart = 0;

    while (BE_ClientInformation_IsRunning()) {
        if (!BE_Window_IsStillOpened())
            break;

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

    if (BE_ClientInformation_IsRunning())
        BE_ClientInformation_StopRunning();

    BA_LOGGER_TRACE("Client loop ended, shutting down\n");
    BA_ASSERT(engineDetails->clientShutdown == NULL || engineDetails->clientShutdown() == 0, "Client shutdown returned non-zero\n");

    if (!BA_Thread_IsSingleThreaded()) {
        BA_LOGGER_INFO("Waiting for thread shutdown (press CTRL+C if frozen)\n");
        BA_Thread_Join(commandThread, NULL);
        BA_LOGGER_DEBUG("Command thread ended\n");

#ifndef BE_DISABLE_NETWORK
        if (BE_ClientInformation_IsServerModeEnabled()) {
            BA_Thread_Join(serverThread, NULL);
            BA_LOGGER_DEBUG("Server thread ended\n");
        }
#endif
    }
    
    BE_PrivateLayer_DestroyLayers();

    if (BE_Server_IsRunning())
        BE_Server_Stop();

    BE_PrivateUI_Destroy();
    BE_PrivateConsole_Destroy();
    BE_PrivateWindow_Destroy();
    BE_SpecificPlatformFunctions_Get().Destroy();

#ifndef BE_DISABLE_NETWORK
    BE_PrivatePacket_Destroy();
#endif

    if (BE_PrivateDefaultPackage_IsOpen())
        BE_PrivateDefaultPackage_Close();

    if (BE_EngineMemoryInformation_GetAllocatedBytes() > 0) {
        char* message = BE_EngineMemoryInformation_GetAllocationInformation("");
        
        BA_LOGGER_WARN("Memory leak detected:\n"
                       "Leaked: %zu allocations, %zu bytes\n%s\n", BE_EngineMemoryInformation_GetAllocatedAmount(), BE_EngineMemoryInformation_GetAllocatedBytes(), message);
        free(message);
    }

    return 0;
}

BE_BINARYEXPORT void I_EntryPoint_InitializeWrapper(void* engineBinary) {
    (void) engineBinary;

    BA_ASSERT_ALWAYS("I am not a client\n");
}

BE_BINARYEXPORT const char* I_EntryPoint_GetName(void) {
    return BE_ENGINE_NAME " " BE_ENGINE_VERSION;
}

BE_BINARYEXPORT const char* BE_EntryPoint_GetName(void) {
    return BE_ENGINE_NAME;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
