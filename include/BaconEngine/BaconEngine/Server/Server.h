// Purpose: The engines server implementation.
// Created on: 10/22/22 @ 5:03 AM

// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>

#include "BaconEngine/BinaryExport.h"
#include "Client.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_BINARYEXPORT BA_Boolean BE_Server_IsRunning(void);
BE_BINARYEXPORT unsigned BE_Server_GetPort(void);

BE_BINARYEXPORT void BE_Server_Start(unsigned port);
BE_BINARYEXPORT void BE_Server_Stop(void);
BE_BINARYEXPORT BE_Client_Connected* BE_Server_GetClient(unsigned clientId);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
