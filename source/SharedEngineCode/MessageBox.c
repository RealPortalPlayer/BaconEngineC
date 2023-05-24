// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "SharedEngineCode/MessageBox.h"
#include "SharedEngineCode/Internal/OperatingSystem.h"
#include "SharedEngineCode/Internal/CPlusPlusSupport.h"

#if SEC_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#endif

// TODO: This code is very ugly, and platform specific.

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_MessageBox_Results SEC_MessageBox_Display(const char* message, const char* caption, SEC_MessageBox_Icons icon, SEC_MessageBox_Buttons buttons) {
#if SEC_OPERATINGSYSTEM_WINDOWS
    UINT flags = MB_OK;

    switch (icon) {
        case SEC_MESSAGEBOX_ICON_ERROR:                flags |= MB_ICONERROR; break;
        case SEC_MESSAGEBOX_ICON_QUESTION:             flags |= MB_ICONQUESTION; break;
        case SEC_MESSAGEBOX_ICON_WARNING:              flags |= MB_ICONWARNING; break;
        default: case SEC_MESSAGEBOX_ICON_INFORMATION: flags |= MB_ICONINFORMATION; break;
    }

    switch (buttons) {
        default: case SEC_MESSAGEBOX_BUTTON_OK:               flags |= MB_OK; break;
        case SEC_MESSAGEBOX_BUTTON_OK_CANCEL:                 flags |= MB_OKCANCEL; break;
        case SEC_MESSAGEBOX_BUTTON_ABORT_RETRY_IGNORE:        flags |= MB_ABORTRETRYIGNORE; break;
        case SEC_MESSAGEBOX_BUTTON_YES_NO_CANCEL:             flags |= MB_YESNOCANCEL; break;
        case SEC_MESSAGEBOX_BUTTON_YES_NO:                    flags |= MB_YESNO; break;
        case SEC_MESSAGEBOX_BUTTON_RETRY_CANCEL:              flags |= MB_RETRYCANCEL; break;
        case SEC_MESSAGEBOX_BUTTON_CANCEL_TRY_AGAIN_CONTINUE: flags |= MB_CANCELTRYCONTINUE; break;
    }

    return MessageBox(NULL, message, caption, flags);
#else // TODO: Everything else.
    return 0;
#endif
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
