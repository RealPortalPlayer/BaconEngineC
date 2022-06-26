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
#include "Rendering/Renderer.h"
#include "Rendering/LayerStack.h"

#ifdef __cplusplus
extern "C" {
#endif

int ClientStart(int argc, char** argv);
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

    // NOTE: Avoid putting log statements, or anything that uses the logging library before this line.
    //       Doing so could potentially prevent debug and trace logs from showing due to it now being fully initialized yet.

    LOG_INFO("Starting BaconEngine");

    if (IS_SERVER_MODE_ENABLED() && !SupportsServer()) {
        LOG_FATAL("This client does not support servers");
        return 1;
    }

    if (!IS_SERVER_MODE_ENABLED()) {
        InitializeWindow(configuration->clientName, (Vector2U) {1080, 720}); // TODO: --width --height
        InitializeLayers();
    }

    ASSERT(ClientStart(argc, argv) == 0, "Client start returned non-zero");

    ClearScreen();
    SetWindowVisibility(1);
    CreateLayer("Test", OnToggleDummy, OnStartShutdownDummy, OnEventDummy, OnRenderDummy, OnRenderDummy, OnStartShutdownDummy);

    while (running) {
        if (!IS_SERVER_MODE_ENABLED() && IsWindowStillOpened()) {
            ClearScreen();
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
            }

            if (running == 0)
                break;

            SDL_RenderPresent(GetSDLRenderer());

            continue;
        }

        printf(ANSI_BRIGHT_FOREGROUND_GREEN);

        if (cheats)
            printf("[C] ");

        printf("> " ANSI_RESET);

        char input[2046];

        fgets(input, sizeof(input), stdin);

        if (strlen(input) == 1)
            continue;

        LOG_INFO("%s", input);
    }

    DestroyWindow();
    DestroyLayers();

    return 0;
}

#ifdef __cplusplus
};
#endif