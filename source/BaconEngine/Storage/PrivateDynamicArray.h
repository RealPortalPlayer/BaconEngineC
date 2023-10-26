// Purpose: Private implementation of DynamicArray
// Created on: 4/24/2023 @ 9:02 AM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>
#include <BaconAPI/Storage/DynamicArray.h>

#include "../AntiClientGuard.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Boolean BE_PrivateDynamicArray_Create(BA_DynamicArray* array, size_t size);
BA_Boolean BE_PrivateDynamicArray_CheckResize(BA_DynamicArray* array);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
