// Purpose: Private implementation of DynamicDictionary
// Created on: 4/24/23 @ 9:17 AM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>
#include <BaconAPI/Storage/DynamicDictionary.h>

#include "../AntiClientGuard.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Boolean BE_PrivateDynamicDictionary_Create(BA_DynamicDictionary* dictionary, size_t size);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
