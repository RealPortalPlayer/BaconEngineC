// Purpose: Private implementation of DynamicDictionary
// Created on: 4/24/2023 @ 9:17 AM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>
#include <SharedEngineCode/Internal/Boolean.h>
#include "../AntiClientGuard.h"
#include "BaconEngine/Storage/DynamicDictionary.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_Boolean BE_PrivateDynamicDictionary_Create(BE_DynamicDictionary* dictionary, size_t size);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
