// Purpose: The engines server implementation.
// Created on: 10/22/22 @ 5:03 AM

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>

#include "BaconEngine/BinaryExport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_BINARYEXPORT BA_Boolean BE_Server_IsRunning(void);
BE_BINARYEXPORT unsigned BE_Server_GetPort(void);

BE_BINARYEXPORT void BE_Server_Start(unsigned port);
BE_BINARYEXPORT void BE_Server_Stop(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
