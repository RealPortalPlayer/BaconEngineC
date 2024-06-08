// Purpose: Connected client management thread
// Created on: 6/7/24 @ 4:30 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>

#include "../AntiClientGuard.h"
#include "BaseThread.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_BASETHREAD_CREATE_HEADER(BE_ClientThread);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
