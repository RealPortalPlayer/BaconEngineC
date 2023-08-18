// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/BuiltInArguments.h>

#include "BaconEngine/ClientInformation.h"
#include "InterfaceFunctions.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static SEC_Boolean beClientInformationRunning = SEC_BOOLEAN_TRUE;
static SEC_Boolean beClientInformationCheats = SEC_BOOLEAN_FALSE;
#endif

SEC_Boolean BE_ClientInformation_IsRunning(void) {
#ifndef BE_CLIENT_BINARY
    return beClientInformationRunning;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, void);
    return function();
#endif
}

SEC_Boolean BE_ClientInformation_IsCheatsEnabled(void) {
#ifndef BE_CLIENT_BINARY
    return beClientInformationCheats;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, void);
    return function();
#endif
}

SEC_Boolean BE_ClientInformation_IsServerModeEnabled(void) {
#ifndef BE_CLIENT_BINARY
    static int enabled = -1;

    if (enabled == -1)
        enabled = SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_SERVER, SEC_BUILTINARGUMENTS_SERVER_SHORT, 0);

    return enabled;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, void);
    return function();
#endif
}

void BE_ClientInformation_StopRunning(void) {
#ifndef BE_CLIENT_BINARY
    if (!beClientInformationRunning) {
        SEC_LOGGER_WARN("Client is already stopping\n");
        return;
    }

    beClientInformationRunning = SEC_BOOLEAN_FALSE;
#else
    BE_INTERFACEFUNCTION(void, void)();
#endif
}

void BE_ClientInformation_SetCheats(SEC_Boolean enable) {
#ifndef BE_CLIENT_BINARY
    beClientInformationCheats = enable;
#else
    BE_INTERFACEFUNCTION(void, SEC_Boolean)(enable);
#endif
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
