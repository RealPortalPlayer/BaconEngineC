// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <Interface/EntryPoint.h>
#include <BaconEngine/Console/Command.h>
#include <BaconEngine/Debugging/Assert.h>

void CreateProjectCommand(BE_Command_Context context) {
    (void) context;
    BE_ASSERT_NOT_IMPLEMENTED();
}

void OpenProjectCommand(BE_Command_Context context) {
    (void) context;
    BE_ASSERT_NOT_IMPLEMENTED();
}

void CloseProjectCommand(BE_Command_Context context) {
    (void) context;
    BE_ASSERT_NOT_IMPLEMENTED();
}

int I_EntryPoint_Start(int argc, char** argv) {
    BE_Command_Register("create_project", "Creates a new project", BE_COMMAND_FLAG_NULL, &CreateProjectCommand);
    {
        BE_Command_AddArgument("name", SEC_TRUE);
        BE_Command_AddArgument("standalone", SEC_TRUE);
        BE_Command_AddArgument("location", SEC_TRUE);
    }

    BE_Command_Register("open_project", "Opens a project", BE_COMMAND_FLAG_NULL, &OpenProjectCommand);
    {
        BE_Command_AddArgument("location", SEC_TRUE);
    }

    BE_Command_Register("close_project", "Closes the currently opened project", BE_COMMAND_FLAG_NULL, &CloseProjectCommand);

    return 0;
}

const char* I_EntryPoint_GetName(void) {
    return "BaconEditor";
}
