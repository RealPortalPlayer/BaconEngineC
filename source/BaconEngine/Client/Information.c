// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Logger.h>
#include <SharedEngineCode/BuiltInArguments.h>

#include "BaconEngine/Client/Information.h"
#include "../InterfaceFunctions.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static BA_Boolean beClientInformationRunning = BA_BOOLEAN_TRUE;
static BA_Boolean beClientInformationCheats = BA_BOOLEAN_FALSE;
#endif

BA_Boolean BE_ClientInformation_IsRunning(void) {
#ifndef BE_CLIENT_BINARY
    return beClientInformationRunning;
#else
    BE_INTERFACEFUNCTION(BA_Boolean, void);
    return function();
#endif
}

BA_Boolean BE_ClientInformation_IsCheatsEnabled(void) {
#ifndef BE_CLIENT_BINARY
    return beClientInformationCheats;
#else
    BE_INTERFACEFUNCTION(BA_Boolean, void);
    return function();
#endif
}

BA_Boolean BE_ClientInformation_IsServerModeEnabled(void) {
#ifndef BE_CLIENT_BINARY
    static int enabled = -1;

    if (enabled == -1)
        enabled = BA_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_SERVER, SEC_BUILTINARGUMENTS_SERVER_SHORT, 0);

    return enabled;
#else
    BE_INTERFACEFUNCTION(BA_Boolean, void);
    return function();
#endif
}

void BE_ClientInformation_StopRunning(void) {
#ifndef BE_CLIENT_BINARY
    if (!beClientInformationRunning) {
        BA_LOGGER_WARN("Client is already stopping\n");
        return;
    }

    beClientInformationRunning = BA_BOOLEAN_FALSE;
#else
    BE_INTERFACEFUNCTION(void, void)();
#endif
}

void BE_ClientInformation_SetCheats(BA_Boolean enable) {
#ifndef BE_CLIENT_BINARY
    beClientInformationCheats = enable;
#else
    BE_INTERFACEFUNCTION(void, BA_Boolean)(enable);
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
