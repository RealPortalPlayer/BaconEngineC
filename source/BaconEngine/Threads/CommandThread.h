// Purpose: Main command thread
// Created on: 6/7/24 @ 3:57 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>

#include "../AntiClientGuard.h"
#include "BaseThread.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Boolean BE_CommandThread_HasThreadStarted(void);

void BE_CommandThread_MarkCursorDirty(void);
BE_BASETHREAD_CREATE_HEADER(BE_CommandThread);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
