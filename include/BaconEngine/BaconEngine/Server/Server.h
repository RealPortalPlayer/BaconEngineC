// Purpose: The engines server implementation.
// Created on: 10/22/22 @ 5:03 AM

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

SEC_CPP_SUPPORT_GUARD_START()
int BE_Server_IsRunning(void);
unsigned BE_Server_GetPort(void);

void BE_Server_Start(unsigned port);
void BE_Server_Stop(void);
SEC_CPP_SUPPORT_GUARD_END()
