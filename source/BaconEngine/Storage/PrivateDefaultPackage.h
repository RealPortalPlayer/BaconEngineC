// Purpose: Functions to open/close the default package
// Created on: 8/8/2023 @ 2:34 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>

#include "../AntiClientGuard.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Boolean BE_PrivateDefaultPackage_Open(void);
BA_Boolean BE_PrivateDefaultPackage_IsOpen(void);
void BE_PrivateDefaultPackage_Close(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
