// Purpose: Allows you to show a messagebox.
// Created on: 10/18/22 @ 4:21 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef enum {
    SEC_MESSAGEBOX_BUTTON_OK,
    SEC_MESSAGEBOX_BUTTON_OK_CANCEL,
    SEC_MESSAGEBOX_BUTTON_ABORT_RETRY_IGNORE,
    SEC_MESSAGEBOX_BUTTON_YES_NO_CANCEL,
    SEC_MESSAGEBOX_BUTTON_YES_NO,
    SEC_MESSAGEBOX_BUTTON_RETRY_CANCEL,
    SEC_MESSAGEBOX_BUTTON_CANCEL_TRY_AGAIN_CONTINUE
} SEC_MessageBox_Buttons;

typedef enum {
    SEC_MESSAGEBOX_ICON_ERROR,
    SEC_MESSAGEBOX_ICON_QUESTION,
    SEC_MESSAGEBOX_ICON_WARNING,
    SEC_MESSAGEBOX_ICON_INFORMATION
} SEC_MessageBox_Icons;

typedef enum {
    SEC_MESSAGEBOX_RESULT_OK = 1,
    SEC_MESSAGEBOX_RESULT_CANCEL,
    SEC_MESSAGEBOX_RESULT_ABORT,
    SEC_MESSAGEBOX_RESULT_RETRY,
    SEC_MESSAGEBOX_RESULT_IGNORE,
    SEC_MESSAGEBOX_RESULT_YES,
    SEC_MESSAGEBOX_RESULT_NO,
    SEC_MESSAGEBOX_RESULT_TRY_AGAIN = 10,
    SEC_MESSAGEBOX_RESULT_CONTINUE
} SEC_MessageBox_Results;

SEC_MessageBox_Results SEC_MessageBox_Display(SEC_MessageBox_Icons icon, SEC_MessageBox_Buttons buttons, const char* caption, const char* message, ...);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
