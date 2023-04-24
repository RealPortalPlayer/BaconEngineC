// Purpose: Private implementation of DynamicArray
// Created on: 4/24/2023 @ 9:02 AM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>
#include <SharedEngineCode/Internal/Boolean.h>

#include "../AntiClientGuard.h"
#include "BaconEngine/Storage/DynamicArray.h"

SEC_CPP_SUPPORT_GUARD_START()
SEC_Boolean BE_PrivateDynamicArray_Create(BE_DynamicArray* array, size_t size);
SEC_CPP_SUPPORT_GUARD_END()
