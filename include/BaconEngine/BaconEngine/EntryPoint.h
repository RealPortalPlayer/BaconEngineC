// Purpose: The entry point for the game engine.
// Created on: 3/29/22 @ 12:33 AM

#pragma once

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/Logger.h>
#include <SDL.h>
#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/OperatingSystem.h>

#include "Debugging/Assert.h"
#include "ClientInformation.h"
#include "Rendering/Window.h"
#include "Rendering/Renderer.h"
#include "Debugging/StrictMode.h"
#include "BaconEngine/Console/Console.h"
#include "BaconEngine/Rendering/Layer.h"

#if OS_POSIX_COMPLIANT
#   define EXPOSE_FUNC __attribute__((visibility("default")))
#elif OS_WINDOWS
#   define EXPOSE_FUNC __declspec(dllexport)
#endif

CPP_GUARD_START()
    int ClientStart(int argc, char** argv);
    int ClientShutdown(void);
    EXPOSE_FUNC int ClientSupportsServer(void);
    EXPOSE_FUNC const char* GetClientName(void);

    __attribute__((unused)) EXPOSE_FUNC int StartBaconEngine(int argc, char** argv) {
        static int alreadyStarted = 0;

        addedArgumentsCount = argc;
        argumentVector = argv;

        if (GetArgumentIndex("--enable-debug-logs") != -1 || GetArgumentIndex("-edl") != -1)
            currentLogLevel = LOG_LEVEL_DEBUG;

        if (GetArgumentIndex("--enable-trace-logs") != -1 || GetArgumentIndex("-etl") != -1)
            currentLogLevel = LOG_LEVEL_TRACE;

        // NOTE: Avoid putting log statements, or anything that uses the logging library before this line.
        //       Doing so could potentially prevent debug and trace logs from showing due to it now being fully initialized yet.

        STRICT_CHECK(!alreadyStarted, 1, "Reinitializing the engine is not supported");
        LOG_TRACE("Entered client code");

        alreadyStarted = 1;

        LOG_INFO("Starting BaconEngine");

        if (IsServerModeEnabled() && !ClientSupportsServer()) {
            LOG_FATAL("This client does not support servers");
            return 1;
        }

        InitializeLayers();
        InitializeRenderer();
        InitializeConsole();

        if (!IsServerModeEnabled() && GetCurrentRenderer() != RENDERER_TYPE_TEXT) {
            int width = 1080;
            int height = 720;

            {
                const char* preParsedWidth = GetArgumentValue("--width");
                const char* preParsedHeight = GetArgumentValue("--height");

                if (preParsedWidth != NULL) {
                    char* error;
                    int parsedWith = (int) strtol(preParsedWidth, &error, 0);

                    if (error != NULL) {
                        LOG_ERROR("Invalid width was supplied, ignoring...");

                        parsedWith = 1080;
                    }

                    width = parsedWith;
                }

                if (preParsedHeight != NULL) {
                    char* error;
                    int parsedHeight = (int) strtol(preParsedHeight, &error, 0);

                    if (error != NULL) {
                        LOG_ERROR("Invalid height was supplied, ignoring...");

                        parsedHeight = 720;
                    }

                    height = parsedHeight;
                }
            }

            InitializeWindow(GetClientName(), (Vector2U) {width, height});
        }

        ASSERT(ClientStart(argc, argv) == 0, "Client start returned non-zero");
        ClearScreen();
        SetWindowVisibility(1);

        while (IsClientRunning()) {
            if (!IsWindowStillOpened() && !IsServerModeEnabled() && GetCurrentRenderer() != RENDERER_TYPE_TEXT)
                break;

            if (IsServerModeEnabled() || GetCurrentRenderer() == RENDERER_TYPE_TEXT)
                continue;

            ClearScreen();
            {
                SDL_Event event;

                while (SDL_PollEvent(&event)) {
                    if (LayerOnEvent(event))
                        continue;

                    switch (event.type) {
                        case SDL_QUIT:
                            ClientShutdown();
                            StopClientRunning();
                            break;
                    }
                }
            }

            if (!IsClientRunning())
                break;

            LayerOnUpdate(LAYER_UPDATE_TYPE_BEFORE_RENDERING);
            SDL_RenderPresent(GetInternalSDLRenderer());
            LayerOnUpdate(LAYER_UPDATE_TYPE_AFTER_RENDERING);
        }

        LOG_TRACE("Client loop ended, shutting down");
        DestroyLayers();
        DestroyWindow();

        return 0;
    }
CPP_GUARD_END()
