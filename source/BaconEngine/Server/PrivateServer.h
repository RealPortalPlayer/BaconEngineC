// Purpose: Private server socket getter.
// Created on: 10/22/22 @ 5:50 AM

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>

#include "../AntiClientGuard.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
int BE_PrivateServer_GetSocketDescriptor(void);
void BE_PrivateServer_AddConnection(int clientDescriptor);
BA_CPLUSPLUS_SUPPORT_GUARD_END()