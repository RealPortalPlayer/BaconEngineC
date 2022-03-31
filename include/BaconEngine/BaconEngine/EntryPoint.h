// Purpose: The entry point for the game engine.
// Created on: 3/29/22 @ 12:33 AM

#pragma once

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/Logger.h>

#include "Debugging/Assert.h"
#include "ClientInformation.h"
#include "Rendering/Window.h"

#ifdef __cplusplus
extern "C" {
#endif

int ClientStart(int argc, char **argv);
int ClientShutdown();
int SupportsServer();
__attribute__((visibility("default"))) const char* GetName();

__attribute__((visibility("default"))) int Start(struct Configuration* configuration, int argc, char** argv) {
    argumentCount = argc;
    argumentVector = argv;

    if (GetArgumentIndex("--enable-debug-logs") != -1 || GetArgumentIndex("-edl") != -1)
        currentLogLevel = LOG_LEVEL_DEBUG;
    else if (GetArgumentIndex("--enable-trace-logs") != -1 || GetArgumentIndex("-etl") != -1)
        currentLogLevel = LOG_LEVEL_TRACE;

    LOG_INFO("Starting BaconEngine.");

    InitializeWindow(configuration->clientName, (Vector2UI) {1080, 720});

    ASSERT(ClientStart(argc, argv) == 0, "Client start returned non-zero.");

    while (running) {
        {
            SDL_Event event;

            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        ClientShutdown();
                        running = 0;
                        break;
                }
            }

            (void) cheats;
        }
    }

    DestroyWindow();

    return 0;
}

#ifdef __cplusplus
};
#endif