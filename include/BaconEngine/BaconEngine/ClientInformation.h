// Purpose: Stores information about the current client.
// Created on: 3/30/22 @ 11:59 PM

#pragma once

#include <SharedEngineCode/ArgumentHandler.h>

#ifdef __cplusplus
extern "C" {
#endif

volatile int running = 1;
volatile int cheats = 0;

#define IS_SERVER_MODE_ENABLED() (GetArgumentIndex("--server") != -1 || GetArgumentIndex("-s") != -1)
#define IS_STRICT_MODE_ENABLED() (GetArgumentIndex("--strict") != -1)

#ifdef __cplusplus
};
#endif