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
#include <SharedEngineCode/OperatingSystem.h>
#include <string.h>
#include <SharedEngineCode/Debugging/Assert.h>
#include <SharedEngineCode/Debugging/StrictMode.h>

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
#   if SEC_OS_POSIX_COMPLIANT
#       define EXPOSE_FUNC __attribute__((visibility("default")))
#   elif SEC_OS_WINDOWS
#       define EXPOSE_FUNC __declspec(dllexport)
#   endif
#else
#   define EXPOSE_FUNC
#endif

CPP_GUARD_START()
#ifdef BACON_ENGINE_LAUNCHER
    int CallLauncherMain(int argc, char** argv);

    int main(int argc, char** argv) {
        return CallLauncherMain(argc, argv);
    }
#endif

    int BE_ClientStart(int argc, char** argv);
    int BE_ClientShutdown(void);
    EXPOSE_FUNC int BE_ClientSupportsServer(void);
    EXPOSE_FUNC const char* BE_GetClientName(void);

    EXPOSE_FUNC int BE_StartBaconEngine(int argc, char** argv) {
        static int alreadyStarted = 0;

        SEC_InitializeArgumentHandler(argc, argv);

        if (SEC_GetArgumentIndex("--enable-debug-logs") != -1 || SEC_GetArgumentIndex("-edl") != -1)
            SEC_SetLogLevel(SEC_LOG_LEVEL_DEBUG);

        if (SEC_GetArgumentIndex("--enable-trace-logs") != -1 || SEC_GetArgumentIndex("-etl") != -1)
            SEC_SetLogLevel(SEC_LOG_LEVEL_TRACE);

        // NOTE: Avoid putting log statements, or anything that uses the logging library before this line.
        //       Doing so could potentially prevent debug and trace logs from showing due to it now being fully layerInitialized yet.

        printf("\n");
        SEC_STRICT_CHECK(!alreadyStarted, 1, "Reinitializing the engine is not supported");
        SEC_LOG_TRACE("Entered client code");

        alreadyStarted = 1;

        SEC_LOG_INFO("Starting BaconEngine");

        if (BE_IsServerModeEnabled() && !BE_ClientSupportsServer()) {
            SEC_LOG_FATAL("This client does not support servers");
            return 1;
        }

        BE_InitializeRenderer();

        if (!BE_IsServerModeEnabled() && BE_GetCurrentRenderer() != BE_RENDERER_TYPE_TEXT) {
            int width = 1080;
            int height = 720;

            {
                const char* preParsedWidth = SEC_GetArgumentValue("--width");
                const char* preParsedHeight = SEC_GetArgumentValue("--height");

                if (preParsedWidth != NULL) {
                    char* error;
                    int parsedWith = (int) strtol(preParsedWidth, &error, 0);

                    if (error != NULL && strlen(error) != 0) {
                        SEC_LOG_ERROR("Invalid width was supplied, ignoring...");

                        parsedWith = 1080;
                    }

                    width = parsedWith;
                }

                if (preParsedHeight != NULL) {
                    char* error;
                    int parsedHeight = (int) strtol(preParsedHeight, &error, 0);

                    if (error != NULL && strlen(error) != 0) {
                        SEC_LOG_ERROR("Invalid height was supplied, ignoring...");

                        parsedHeight = 720;
                    }

                    height = parsedHeight;
                }
            }

#ifndef BACON_ENGINE_DISABLE_SDL_TTF
            SEC_ASSERT(TTF_Init() == 0, "Failed to initialize SDL TTF: %s", SDL_GetError());
#endif

            BE_InitializeLayers();
            BE_InitializeWindow(BE_GetClientName(), (BE_Vector2U) {(unsigned) width, (unsigned) height});
        }

        BE_InitializeUISystem();
        BE_InitializeConsole();
        SEC_ASSERT(BE_ClientStart(argc, argv) == 0, "Client start returned non-zero");
        BE_ClearScreen();
        BE_SetWindowVisibility(1);

#ifndef BACON_ENGINE_DISABLE_SDL
        double lastTime = SDL_GetTicks();
#endif

        BE_ExecuteCommand("debuginfo");

        while (BE_IsClientRunning()) {
            if (!BE_IsWindowStillOpened() && !BE_IsServerModeEnabled() && BE_GetCurrentRenderer() != BE_RENDERER_TYPE_TEXT)
                break;

            if (BE_IsServerModeEnabled() || BE_GetCurrentRenderer() == BE_RENDERER_TYPE_TEXT)
                continue;

#ifndef BACON_ENGINE_DISABLE_SDL
            double deltaCurrentTime = SDL_GetTicks();
            double deltaTime = (deltaCurrentTime - lastTime) / 1000.0f;

            lastTime = deltaCurrentTime;

            BE_ClearScreen();
            BE_LayerOnUpdate(LAYER_UPDATE_TYPE_BEFORE_RENDERING, deltaTime);
            SDL_RenderPresent(BE_GetInternalSDLRenderer());
            BE_LayerOnUpdate(LAYER_UPDATE_TYPE_AFTER_RENDERING, deltaTime);

            {
                SDL_Event event;

                while (SDL_PollEvent(&event)) {
                    if (BE_LayerOnEvent(event))
                        continue;

                    switch (event.type) {
                        case SDL_QUIT:
                            BE_ClientShutdown();
                            BE_StopClientRunning();
                            break;

                        case SDL_KEYDOWN:
                            BE_SetKeyDown(BE_SDLToEngineKeyCode(event.key.keysym.scancode), 1);
                            break;
                    }
                }
            }
#endif
        }

        SEC_LOG_TRACE("Client loop ended, shutting down");
        BE_DestroyLayers();
        BE_DestroyUIWindows();
        BE_DestroyConsole();
        BE_DestroyWindow();

#ifndef BACON_ENGINE_DISABLE_SDL_TTF
        TTF_Quit();
#endif

#ifndef BACON_ENGINE_DISABLE_SDL
        SDL_Quit();
#endif

        return 0;
    }
CPP_GUARD_END()
