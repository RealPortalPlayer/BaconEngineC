// Purpose: The entry point for the game engine.
// Created on: 3/29/22 @ 12:33 AM

#pragma once

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/ANSICodes.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SharedEngineCode/Internal/CppHeader.h>

#include "Debugging/Assert.h"
#include "ClientInformation.h"
#include "Rendering/Window.h"
#include "Rendering/Renderer.h"
#include "Debugging/StrictMode.h"
#include "Console/Console.h"

CPP_GUARD_START()
    int ClientStart(int argc, char** argv);
    int ClientShutdown(void);
    __attribute__((visibility("default"))) int ClientSupportsServer(void);
    __attribute__((visibility("default"))) const char* GetClientName(void);

    void HelpCommand(Command this, CommandContext context) {
        const char* commandName = GetArgumentString(context.arguments, "command", "");

        if (commandName[0] == '\0') {
            LOG_INFO("Commands:");

            for (int i = 0; i < GetCommandAmount(); i++) {
                Command command = GetCommands()[i];

                printf("%s%s - %s", i != 0 ? "\n" : "", command.name, command.description);

                for (int j = 0; j < MAX_ARGUMENTS; j++) {
                    if (command.arguments.keys[j] == NULL)
                        break;

                    printf("%s%s%s%s", j == 0 ? " - " : " ",
                           command.arguments.values[j] ? "<" : "[",
                           command.arguments.keys[j],
                           command.arguments.values[j] ? ">" : "]");
                }
            }

            printf("\n");
            return;
        }

        int found = 0;

        for (int i = 0; i < GetCommandAmount(); ++i) {
            Command command = GetCommands()[i];

            if (strcmp(command.name, commandName) != 0)
                continue;

            found = 1;

            LOG_INFO("Command:");
            printf("%s - %s", command.name, command.description);

            for (int j = 0; j < MAX_ARGUMENTS; j++) {
                if (command.arguments.keys[j] == NULL)
                    break;

                printf("%s%s%s%s", j == 0 ? " - " : " ",
                       command.arguments.values[j] ? "<" : "[",
                       command.arguments.keys[j],
                       command.arguments.values[j] ? ">" : "]");
            }

            break;
        }

        if (!found) {
            LOG_ERROR("%s was not found", commandName);
            return;
        }

        printf("\n");
    }

    void CheatsCommand(Command this, CommandContext context) {
        if (context.client != NULL) {
            // TODO: Tell user.
            return;
        }

        if (!ContainsArgument(context.arguments, "enable")) {
            LOG_INFO("Cheats is %s", cheats ? "enabled" : "disabled");
            return;
        }

        cheats = GetArgumentBoolean(context.arguments, "enable", 0);

        LOG_INFO("Cheats is now %s", cheats ? "enabled" : "disabled");
    }

    void StopCommand() {
        running = 0;
    }

    __attribute__((visibility("default"))) int StartBaconEngine(struct LauncherConfiguration configuration, int argc, char** argv) {
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

        alreadyStarted = 1;

        LOG_INFO("Starting BaconEngine");

        if (IsServerModeEnabled() && !ClientSupportsServer()) {
            LOG_FATAL("This client does not support servers");
            return 1;
        }

        LOG_DEBUG("Registering commands");
        RegisterCommand("help", "Shows every command information.", CREATE_ARGUMENTS("command", 0), &HelpCommand);
        RegisterCommand("cheats", "Enable or disable cheat commands.", CREATE_ARGUMENTS("enable", 0), &CheatsCommand);
        RegisterCommand("stop", "Stops the engine.", CREATE_EMPTY_COMMAND_ARGUMENTS(), &StopCommand);

        if (!IsServerModeEnabled()) {
            int width = 1080;
            int height = 720;

            {
                const char* preParsedWidth = GetArgumentValue("--width");
                const char* preParsedHeight = GetArgumentValue("--height");

                if (preParsedWidth != NULL)
                    width = atoi(preParsedWidth);

                if (preParsedHeight != NULL)
                    height = atoi(preParsedHeight);
            }

            InitializeWindow(configuration.clientName, (Vector2U) {width, height});
        }

        ASSERT(ClientStart(argc, argv) == 0, "Client start returned non-zero");
        ClearScreen();
        SetWindowVisibility(1);

        while (running) {
            if (!IsServerModeEnabled() && IsWindowStillOpened()) {
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

                if (!running)
                    break;

                SDL_RenderPresent(GetInternalSDLRenderer());

                continue;
            }

            printf(ANSI_BRIGHT_FOREGROUND_GREEN);

            if (cheats)
                printf("[C] ");

            printf("> " ANSI_RESET);

            char input[2046] = {'\0'};
            {
                char preInput[2046];

                fgets(preInput, sizeof(preInput), stdin);

                preInput[strcspn(preInput, "\n")] = '\0';

                int writer = 0;

                for (int i = 0; i < 2046; i++) {
                    if (preInput[i] == '\0')
                        break;

                    if (preInput[i] == ' ' && writer == 0)
                        continue;

                    input[writer++] = preInput[i];
                }

                if (input[0] == '\0')
                    continue;

                for (int i = writer - 1; i >= 0; i--) {
                    if (input[i] != ' ')
                        break;

                    input[i] = '\0';
                }
            }

            if (input[0] == '\0')
                continue;

            const char* commandName = strtok(input, " ");
            const char* arguments[MAX_ARGUMENTS] = {}; // TODO: Parse quotes.

            for (int i = 0; i < MAX_ARGUMENTS; i++) {
                const char* argument = strtok(NULL, " ");

                if (argument == NULL)
                    break;

                arguments[i] = argument;
            }

            ExecuteCommand(commandName, arguments);
        }

        LOG_TRACE("Client loop ended, shutting down");
        DestroyWindow();

        return 0;
    }
CPP_GUARD_END()