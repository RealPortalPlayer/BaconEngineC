// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/BuiltInArguments.h>

#include "BaconEngine/ClientInformation.h"

SEC_CPP_SUPPORT_GUARD_START()
SEC_Boolean beClientInformationRunning = SEC_TRUE;
SEC_Boolean beClientInformationCheats = SEC_FALSE;

SEC_Boolean BE_ClientInformation_IsRunning(void) {
    return beClientInformationRunning;
}

SEC_Boolean BE_ClientInformation_IsCheatsEnabled(void) {
    return beClientInformationCheats;
}

SEC_Boolean BE_ClientInformation_IsServerModeEnabled(void) {
    static int enabled = -1;

    if (enabled == -1)
        enabled = SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_SERVER, SEC_BUILTINARGUMENTS_SERVER_SHORT, 0);

    return enabled;
}

SEC_Boolean BE_ClientInformation_IsStrictModeEnabled(void) {
    static int enabled = -1;

    if (enabled == -1)
        enabled = !SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_NO_STRICT, SEC_BUILTINARGUMENTS_NO_STRICT_SHORT, 0);

    return enabled;
}

void BE_ClientInformation_StopRunning(void) {
    if (!beClientInformationRunning) {
        SEC_LOGGER_WARN("Client is already stopping\n");
        return;
    }

    beClientInformationRunning = SEC_FALSE;
}

void BE_ClientInformation_SetCheats(SEC_Boolean enable) {
    beClientInformationCheats = enable;
}
SEC_CPP_SUPPORT_GUARD_END()
