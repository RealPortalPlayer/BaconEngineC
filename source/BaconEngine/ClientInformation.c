#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Internal/CppSupport.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <stddef.h>

#include "BaconEngine/ClientInformation.h"

SEC_CPP_SUPPORT_GUARD_START()
int running = 1;
int cheats = 0;

int BE_ClientInformation_IsRunning(void) {
    return running;
}

int BE_ClientInformation_IsCheatsEnabled(void) {
    return cheats;
}

int BE_ClientInformation_IsServerModeEnabled(void) {
    static int enabled = -1;

    if (enabled == -1)
        enabled = SEC_ArgumentHandler_GetIndexWithShort(SEC_BUILTINARGUMENTS_SERVER, SEC_BUILTINARGUMENTS_SERVER_SHORT, 0, NULL, NULL);

    return enabled;
}

int BE_ClientInformation_IsStrictModeEnabled(void) {
    static int enabled = -1;

    if (enabled == -1)
        enabled = !SEC_ArgumentHandler_GetIndexWithShort(SEC_BUILTINARGUMENTS_NO_STRICT, SEC_BUILTINARGUMENTS_NO_STRICT_SHORT, 0, NULL, NULL);

    return enabled;
}

void BE_ClientInformation_StopRunning(void) {
    if (!running) {
        SEC_LOGGER_WARN("Client is already stopping");
        return;
    }

    running = 0;
}

void BE_ClientInformation_SetCheats(int enable) {
    cheats = enable;
}
SEC_CPP_SUPPORT_GUARD_END()
