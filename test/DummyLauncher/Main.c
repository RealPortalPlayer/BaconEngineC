// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/Launcher.h>

int main(int argc, char** argv) {
    SEC_Launcher_Configuration configuration;

    SEC_Launcher_CreateConfiguration(&configuration, ".");
    return configuration.code == SEC_LAUNCHER_ERROR_CODE_NULL ? configuration.unionVariables.data.Start(argc, argv) : (int) configuration.code;
}
