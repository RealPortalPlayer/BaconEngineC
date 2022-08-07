// Purpose: A basic client object.
// Created on: 8/6/22 @ 7:22 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

CPP_GUARD_START()
    typedef struct {
        const char* name;
        int enabled;
        // TODO: Components
    } ClientObject;
CPP_GUARD_END()
