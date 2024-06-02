// Purpose: Stores information about the current client.
// Created on: 3/30/22 @ 11:59 PM

// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>

#include "../BinaryExport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_BINARYEXPORT BA_Boolean BE_ClientInformation_IsRunning(void);
BE_BINARYEXPORT BA_Boolean BE_ClientInformation_IsCheatsEnabled(void);
BE_BINARYEXPORT BA_Boolean BE_ClientInformation_IsServerModeEnabled(void);

BE_BINARYEXPORT void BE_ClientInformation_StopRunning(void);
BE_BINARYEXPORT void BE_ClientInformation_SetCheats(BA_Boolean enable);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
