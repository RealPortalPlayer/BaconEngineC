// Purpose: Private server socket getter.
// Created on: 10/22/22 @ 5:50 AM

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "../AntiClientGuard.h"

SEC_CPP_SUPPORT_GUARD_START()
int BE_PrivateServer_GetSocketDescriptor(void);
void BE_PrivateServer_AddConnection(int clientDescriptor);
SEC_CPP_SUPPORT_GUARD_END()
