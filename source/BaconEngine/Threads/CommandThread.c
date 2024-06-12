// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/Internal/OperatingSystem.h>
#include <stdio.h>
#include <string.h>

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <unistd.h>
#   include <fcntl.h>
#elif BA_OPERATINGSYSTEM_WINDOWS
#   define fileno _fileno
#   include <Windows.h>
#endif

#include "CommandThread.h"
#include "BaconEngine/Client/Information.h"
#include "BaconEngine/Console/Console.h"
#include "BaconEngine/Rendering/Renderer.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static BA_Boolean beCommandThreadStarted = BA_BOOLEAN_FALSE;
static BA_Boolean beCommandThreadCursorDirty = BA_BOOLEAN_TRUE;

BA_Boolean BE_CommandThread_HasThreadStarted(void) {
    return beCommandThreadStarted;
}

void BE_CommandThread_MarkCursorDirty(void) {
    beCommandThreadCursorDirty = BA_BOOLEAN_TRUE;
}

BE_BASETHREAD_CREATE_IMPLEMENTATION(BE_CommandThread, beCommandThread, "command") {
    beCommandThreadStarted = BA_BOOLEAN_TRUE;

    {
        int inputFileNumber = fileno(stdin);

        if (inputFileNumber >= 0) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
            fcntl(inputFileNumber, F_SETFL, O_NONBLOCK);
#elif BA_OPERATINGSYSTEM_WINDOWS
            ULONG enable = 1;
        
            ioctlsocket(inputFileNumber, FIONBIO, &enable);
#endif
        }
    }

    BA_LOGGER_DEBUG("Command thread started\n");

    while (BE_ClientInformation_IsRunning()) {
        if (BE_Console_GetCommandAmount() == 0)
            continue;

        char input[4024];

        memset(input, 0, 4024);

        {
            int written = 0;

            // TODO: Arrow keys to go back in history.
            // TODO: Allow backspacing
            while (BE_ClientInformation_IsRunning()) {
                if (BE_Renderer_GetCurrentType() == BE_RENDERER_TYPE_TEXT && beCommandThreadCursorDirty) {
                    BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, "%c ", BE_ClientInformation_IsCheatsEnabled() ? '#' : '$');

                    beCommandThreadCursorDirty = BA_BOOLEAN_FALSE;
                }
                
                char character = getc(stdin);
                
                if (character == EOF)
                    continue;
                
                if (written >= sizeof(input) - 1 || character == '\n')
                    break;
                
                input[written++] = character;
            }
        }
        
        if (input[0] != '\0')
            BE_Console_ExecuteCommand(input, BE_CLIENT_UNCONNECTED);

        beCommandThreadCursorDirty = BA_BOOLEAN_TRUE;
    }

    beCommandThreadStarted = BA_BOOLEAN_FALSE;

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    return NULL;
#else
    return 0;
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
