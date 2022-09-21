// Purpose: Window functions only to be called by the engine's entry point.
// Created on: 9/13/22 @ 9:08 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Math/Vector.h"

SEC_CPP_GUARD_START()
    void BE_PrivateWindow_Initialize(const char* windowTitle, BE_Vector_2U windowSize);
    void BE_PrivateWindow_Destroy(void);
SEC_CPP_GUARD_END()
