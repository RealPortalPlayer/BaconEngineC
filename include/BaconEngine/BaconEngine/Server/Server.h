// Purpose: The engines server implementation.
// Created on: 10/22/22 @ 5:03 AM

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>
#include <SharedEngineCode/Internal/Boolean.h>

#include "BaconEngine/DllExport.h"

SEC_CPP_SUPPORT_GUARD_START()
BE_DLLEXPORT SEC_Boolean BE_Server_IsRunning(void);
BE_DLLEXPORT unsigned BE_Server_GetPort(void);

BE_DLLEXPORT void BE_Server_Start(unsigned port);
BE_DLLEXPORT void BE_Server_Stop(void);
SEC_CPP_SUPPORT_GUARD_END()
