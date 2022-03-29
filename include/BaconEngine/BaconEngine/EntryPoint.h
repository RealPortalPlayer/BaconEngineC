// Purpose: The entry point for the game engine.
// Created on: 3/29/22 @ 12:33 AM

#pragma once

#include <SharedEngineCode/Launcher.h>

int ClientStart(int argc, char** argv);
int ClientShutdown();
int SupportsServer();
__attribute__((visibility("default"))) const char* GetName();

__attribute__((visibility("default"))) int Start(struct Configuration* configuration, int argc, char** argv) {
    ClientStart(argc, argv);
    ClientShutdown();
    SupportsServer();
    GetName();
    return 0;
}