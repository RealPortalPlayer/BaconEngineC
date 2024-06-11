// Purpose: Fake client, used as a substitute for some functions
// Created on: 6/11/24 @ 1:03 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#ifdef BE_DUMMYCLIENT_INCLUDED
#   error You cannot include the dummy client header more than once
#endif

#define BE_DUMMYCLIENT_INCLUDED

#include <BaconAPI/Internal/Boolean.h>
#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Logger.h>

#include "AntiClientGuard.h"
#include "BaconEngine/BinaryExport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_BINARYEXPORT int I_EntryPoint_Start(int argc, char** argv) {
    BA_LOGGER_DEBUG("Called dummied start\n");
    return 0;
}

BE_BINARYEXPORT int I_EntryPoint_Shutdown(void) {
    BA_LOGGER_DEBUG("Called dummied shutdown\n");
    return 0;
}

BE_BINARYEXPORT BA_Boolean I_EntryPoint_SupportsServers(void) {
    BA_LOGGER_DEBUG("Called dummied supports server\n");
    return BA_BOOLEAN_FALSE;
}

BE_BINARYEXPORT const char* I_EntryPoint_GetName(void) {
    BA_LOGGER_DEBUG("Called dummied get name\n");
    return BE_ENGINE_NAME " " BE_ENGINE_VERSION;
}

BE_BINARYEXPORT void I_EntryPoint_InitializeWrapper(void* engineBinary) {
    (void) engineBinary;

    BA_ASSERT_ALWAYS("I am not a client\n");
}

BE_BINARYEXPORT const char* I_EntryPoint_GetEngineVersion(void) {
    BA_LOGGER_DEBUG("Called dummied get engine version\n");
    return BE_ENGINE_VERSION;
}

BE_BINARYEXPORT const char* I_EntryPoint_GetEngineName(void) {
    BA_LOGGER_DEBUG("Called dummied get engine name\n");
    return BE_ENGINE_NAME;
}

const char* BE_DummyClient_EmptyName(void) {
    BA_LOGGER_DEBUG("Called dummied get name\n");
    return "";
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
