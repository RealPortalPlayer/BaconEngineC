// Purpose: Command arguments.
// Created on: 7/30/22 @ 2:36 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#define MAX_ARGUMENTS 1024

CPP_GUARD_START()
    typedef struct {
        const char* keys[MAX_ARGUMENTS];
        int values[MAX_ARGUMENTS];
    } CommandArguments;
CPP_GUARD_END()

#define CREATE_EMPTY_COMMAND_ARGUMENTS() (CommandArguments) {.keys = {""}, .values = {0}}
#define CREATE_ARGUMENTS(theKeys, theValues) (CommandArguments) {.keys = {theKeys}, .values = {theValues}}
#define MULTIPLE(...) __VA_ARGS__