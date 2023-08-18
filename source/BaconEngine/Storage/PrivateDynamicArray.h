// Purpose: Private implementation of DynamicArray
// Created on: 4/24/2023 @ 9:02 AM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>
#include <SharedEngineCode/Internal/Boolean.h>
#include <SharedEngineCode/Storage/DynamicArray.h>

#include "../AntiClientGuard.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_Boolean BE_PrivateDynamicArray_Create(SEC_DynamicArray* array, size_t size);
SEC_Boolean BE_PrivateDynamicArray_CheckResize(SEC_DynamicArray* array);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
