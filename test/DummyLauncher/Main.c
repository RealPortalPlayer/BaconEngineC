// Copyright (c) 2022, PortalPlayer
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/Launcher.h>

int main(int argc, char** argv) {
    SEC_Launcher_Configuration configuration;

    SEC_Launcher_CreateConfiguration(&configuration, ".");

    return configuration.code == SEC_LAUNCHER_ERROR_CODE_NULL ? configuration.Start(argc, argv) : (int) configuration.code;
}
