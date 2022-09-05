// Purpose: The entry point for the game engine.
// Created on: 3/29/22 @ 12:33 AM

#ifdef BACON_ENGINE_INCLUDED_ENTRY_POINT
#   error You cannot include the entry point more than once.
#endif

#define BACON_ENGINE_INCLUDED_ENTRY_POINT

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/Internal/OperatingSystem.h>
#include <string.h>
#include <SharedEngineCode/Debugging/Assert.h>
#include <SharedEngineCode/Debugging/StrictMode.h>
#include <unistd.h>
#include <SharedEngineCode/ANSI.h>

#include "ClientInformation.h"
#include "Rendering/Window.h"
#include "Rendering/Renderer.h"
#include "Console/Console.h"
#include "Rendering/Layer.h"
#include "Rendering/UI.h"
#include "SDLSupport.h"
#include "Input/Keyboard.h"
#include "Event.h"

#ifndef BACON_ENGINE_LAUNCHER
#   if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#       define BE_ENTRYPOINT_EXPOSE_FUNC __attribute__((visibility("default")))
#   elif SEC_OPERATINGSYSTEM_WINDOWS
#       define EXPOSE_FUNC __declspec(dllexport)
#   endif
#else
#   define EXPOSE_FUNC
#endif

SEC_CPP_GUARD_START()
#ifdef BACON_ENGINE_LAUNCHER
    int CallLauncherMain(int argc, char** argv);

    int main(int argc, char** argv) {
        return CallLauncherMain(argc, argv);
    }
#endif

    int BE_EntryPoint_ClientStart(int argc, char** argv);
    int BE_EntryPoint_ClientShutdown(void);
    BE_ENTRYPOINT_EXPOSE_FUNC int BE_EntryPoint_ClientSupportsServer(void);
    BE_ENTRYPOINT_EXPOSE_FUNC const char* BE_EntryPoint_GetClientName(void);
    void BE_EntryPoint_SignalDetected(int signal);

    BE_ENTRYPOINT_EXPOSE_FUNC int BE_EntryPoint_StartBaconEngine(int argc, char** argv) {
        static int alreadyStarted = 0;

        SEC_ArgumentHandler_Initialize(argc, argv);

        if (!SEC_Logger_AlreadySentFistLog())
            printf("\n");

        SEC_STRICTMODE_CHECK(!alreadyStarted, 1, "Reinitializing the engine is not supported");
        SEC_LOGGER_TRACE("Entered client code");

        alreadyStarted = 1;

        SEC_LOGGER_INFO("Starting BaconEngine");

        if (BE_ClientInformation_IsServerModeEnabled() && !BE_EntryPoint_ClientSupportsServer()) {
            SEC_LOGGER_FATAL("This client does not support servers");
            return 1;
        }

        SEC_LOGGER_DEBUG("Registering signals");
        signal(SIGSEGV, BE_EntryPoint_SignalDetected);
        BE_Renderer_Initialize();

        if (!BE_ClientInformation_IsServerModeEnabled() && BE_Renderer_GetCurrentType() != BE_RENDERER_TYPE_TEXT) {
            int width = 1080;
            int height = 720;

            {
                const char* preParsedWidth = SEC_ArgumentHandler_GetValue("--width");
                const char* preParsedHeight = SEC_ArgumentHandler_GetValue("--height");

                if (preParsedWidth != NULL) {
                    char* error;
                    int parsedWith = (int) strtol(preParsedWidth, &error, 0);

                    if (error != NULL && strlen(error) != 0) {
                        SEC_LOGGER_ERROR("Invalid width was supplied, ignoring...");

                        parsedWith = 1080;
                    }

                    width = parsedWith;
                }

                if (preParsedHeight != NULL) {
                    char* error;
                    int parsedHeight = (int) strtol(preParsedHeight, &error, 0);

                    if (error != NULL && strlen(error) != 0) {
                        SEC_LOGGER_ERROR("Invalid height was supplied, ignoring...");

                        parsedHeight = 720;
                    }

                    height = parsedHeight;
                }
            }

#ifndef BACON_ENGINE_DISABLE_SDL_TTF
            SEC_ASSERT(TTF_Init() == 0, "Failed to initialize SDL TTF: %s", SDL_GetError());
#endif

            BE_Layer_InitializeLayers();
            BE_Window_Initialize(BE_EntryPoint_GetClientName(), BE_VECTOR_CREATE(2U, (unsigned) width, (unsigned) height));
        }

        BE_UI_InitializeUIs();
        BE_Console_Initialize();
        SEC_ASSERT(BE_EntryPoint_ClientStart(argc, argv) == 0, "Client start returned non-zero");
        BE_Renderer_ClearScreen();
        BE_Window_SetVisibility(1);

#ifndef BACON_ENGINE_DISABLE_SDL
        double lastTime = SDL_GetTicks();
#endif

        BE_Console_ExecuteCommand("debuginfo");

        while (BE_ClientInformation_IsRunning()) {
            if (!BE_Window_IsStillOpened() && !BE_ClientInformation_IsServerModeEnabled() &&
                BE_Renderer_GetCurrentType() != BE_RENDERER_TYPE_TEXT)
                break;

            if (BE_ClientInformation_IsServerModeEnabled() || BE_Renderer_GetCurrentType() == BE_RENDERER_TYPE_TEXT)
                continue;

#ifndef BACON_ENGINE_DISABLE_SDL
            double deltaCurrentTime = SDL_GetTicks();
            double deltaTime = (deltaCurrentTime - lastTime) / 1000.0f;

            lastTime = deltaCurrentTime;

            BE_Renderer_ClearScreen();
            BE_Layer_OnUpdate(LAYER_UPDATE_TYPE_BEFORE_RENDERING, deltaTime);
            SDL_RenderPresent(BE_Window_GetInternalSDLRenderer());
            BE_Layer_OnUpdate(LAYER_UPDATE_TYPE_AFTER_RENDERING, deltaTime);

            {
                SDL_Event event;

                while (SDL_PollEvent(&event)) {
                    if (BE_Layer_OnEvent(event))
                        continue;

                    switch (event.type) {
                        case SDL_QUIT:
                            BE_EntryPoint_ClientShutdown();
                            BE_ClientInformation_StopRunning();
                            break;

                        case SDL_KEYDOWN:
                            BE_Keyboard_SetKeyDown(BE_Keyboard_SDLToEngineKeyCode(event.key.keysym.scancode), 1);
                            break;
                    }
                }
            }
#endif
        }

        SEC_LOGGER_TRACE("Client loop ended, shutting down");
        BE_Layer_DestroyLayers();
        BE_UI_DestroyWindows();
        BE_Console_Destroy();
        BE_Window_Destroy();

#ifndef BACON_ENGINE_DISABLE_SDL_TTF
        TTF_Quit();
#endif

#ifndef BACON_ENGINE_DISABLE_SDL
        SDL_Quit();
#endif

        if (SEC_EngineMemory_GetAllocated() > 0)
            SEC_LOGGER_WARN("Potential memory leak detected! %u megabytes (%u bytes) of data is still being used by the engine", SEC_EngineMemory_GetAllocated() / 1000000, SEC_EngineMemory_GetAllocated());

        return 0;
    }

    void BE_EntryPoint_SignalDetected(int signal) {
        // NOTE: Do NOT use printf, SEC_LOG_TYPE, or family inside this function.
        //       None of those functions are signal-safe.

        if (signal == SIGSEGV) {
            static int antiDoubleSegfault = 0;

            SEC_Logger_SetLogLevel(SEC_LOGGER_LOG_LEVEL_NULL);

            if (SEC_Logger_GetLogLevel() <= SEC_LOGGER_LOG_LEVEL_FATAL && !antiDoubleSegfault) {
                antiDoubleSegfault = 1;

                write(STDOUT_FILENO, "\n", 1);

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
    }
SEC_CPP_GUARD_END()
