// Purpose: The entry point for the game engine.
// Created on: 3/29/22 @ 12:33 AM

#pragma once

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/ANSICodes.h>

#include "Debugging/Assert.h"
#include "ClientInformation.h"
#include "Rendering/Window.h"

#ifdef __cplusplus
extern "C" {
#endif

int ClientStart(int argc, char **argv);
int ClientShutdown(void);
__attribute__((visibility("default"))) int SupportsServer(void);
__attribute__((visibility("default"))) const char* GetName(void);

__attribute__((visibility("default"))) int Start(struct Configuration* configuration, int argc, char** argv) {
    argumentCount = argc;
    argumentVector = argv;

    if (GetArgumentIndex("--enable-debug-logs") != -1 || GetArgumentIndex("-edl") != -1)
        currentLogLevel = LOG_LEVEL_DEBUG;
    else if (GetArgumentIndex("--enable-trace-logs") != -1 || GetArgumentIndex("-etl") != -1)
        currentLogLevel = LOG_LEVEL_TRACE;

    LOG_INFO("Starting BaconEngine.");

    if (IS_SERVER_MODE_ENABLED( ) && !SupportsServer()) {
        LOG_FATAL("This client does not support servers.");
        return 1;
    }

    if (!IS_SERVER_MODE_ENABLED())
        InitializeWindow(configuration->clientName, (Vector2UI) {1080, 720});

    ASSERT(ClientStart(argc, argv) == 0, "Client start returned non-zero.");

    while (running) {
        if (!IS_SERVER_MODE_ENABLED()) {
            {
                SDL_Event event;

                while (SDL_PollEvent(&event)) {
                    switch (event.type) {
                        case SDL_QUIT:
                            ClientShutdown();
                            break;
                    }
                }
            }

            continue;
        }

        printf("\r" ANSI_BRIGHT_FOREGROUND_GREEN);

        if (cheats)
            printf("[C] ");

        printf("> " ANSI_RESET);

        char input[2046];

        fgets(input, sizeof(input), stdin);
    }

    DestroyWindow();

    return 0;
}

#ifdef __cplusplus
};
#endif