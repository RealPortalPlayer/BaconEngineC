#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Input/Keyboard.h"

CPP_GUARD_START()
    char keyCodesCharArray[BE_KEY_CODE_SIZE] = {
        1, 1, 1, 1, // NULL PLACEHOLDER PLACEHOLDER PLACEHOLDER
        'A', 'B', 'C', 'D', 'E', 'F', 'G',
        'H', 'I', 'J', 'K', 'L', 'M', 'N',
        'O', 'P', 'Q', 'R', 'S', 'T', 'U',
        'V', 'W', 'X', 'Y', 'Z', '1', '2',
        '3', '4', '5', '6', '7', '8', '9',
        '0', '\n',
        0, 0, // ESCAPE BACKSPACE
        '\t', ' ', '-', '=', '[', ']', '\\',
        1, // PLACEHOLDER
        ';', '\'', '`', ',', '.', '/',
        0, 0, 0, 0, 0, 0, 0, // CAPS_LOCK F1 F2 F3 F4 F5 F6
        0, 0, 0, 0, 0, 0, 0, // F7 F8 F9 F10 F11 F12 PRINT_SCREEN
        0, 0, 0, 0, 0, 0, 0, // SCROLL_LOCK PAUSE INSERT HOME PAGE_UP DELETE END
        0, 0, 0, 0, 0, 0, // PAGE_DOWN RIGHT LEFT DOWN UP NUM_LOCK
        '/', '*', '-', '+', '\n', '1', '2',
        '3', '4', '5', '6', '7', '8', '9',
        '0', '.',
        1, 0, 1, // PLACEHOLDER MENU PLACEHOLDER
        '=',
        0, 0, 0, 0, 0, 0, 0, // F13 F14 F15 F16 F17 F18 F19
        0, 0, 0, 0, 0, 1, 1, // F20 F21 F22 F23 F24 PLACEHOLDER PLACEHOLDER
        1, 1, 1, 1, 1, 1, 1, // PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER
        1, 1, 1, 1, 1, 1, 1, // PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER
        1, 1, 1, 1, 1, 1, 1, // PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER
        1, 1, 1, 1, 1, 1, 1, // PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER
        1, 1, 1, 1, 1, 1, 1, // PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER
        1, 1, 1, 1, 1, 1, 1, // PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER
        1, 1, 1, 1, 1, 1, 1, // PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER
        1, 1, 1, 1, 1, 1, 1, // PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER
        1, 1, 1, 1, 1, 1, 1, // PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER
        1, 1, 1, 1, 1, 1, 1, // PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER
        1, 1, 1, 1, 1, 1, 1, // PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER
        1, 1, 1, 1, 1, 1, 1, // PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER
        1, 1, 1, 1, 1, 1, 1, // PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER
        1, 1, 1, 1, 1, 1, 1, // PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER
        1, 1, 1, 1, 1, 1, 1, // PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER PLACEHOLDER
        1, 0, 0, 0, 0, 0, 0, // PLACEHOLDER LEFT_CTRL LEFT_SHIFT LEFT_ALT LEFT_WINDOWS RIGHT_CTRL RIGHT_SHIFT
        0, 0 // RIGHT_ALT RIGHT_WINDOWS
    };

    int keysDown[BE_KEY_CODE_SIZE] = {0};
    int keysPressed[BE_KEY_CODE_SIZE] = {0};

    void BE_SetKeyDown(BE_KeyCodes keyCode, int down) {
        if (!BE_IsEngineKeyCodeValid(keyCode))
            return;

        keysDown[keyCode] = down;
    }

    void BE_SetKeyPressed(BE_KeyCodes keyCode, int pressed) {
        if (!BE_IsEngineKeyCodeValid(keyCode))
            return;

        keysPressed[keyCode] = pressed;
    }

    char BE_ConvertKeyCodeToChar(BE_KeyCodes keyCode) {
        return (char) (BE_IsEngineKeyCodeValid(keyCode) ? keyCodesCharArray[keyCode] : 0);
    }

    int BE_IsKeyDown(BE_KeyCodes keyCode) {
        return BE_IsEngineKeyCodeValid(keyCode) && keysDown[keyCode];
    }

    int BE_IsKeyPressed(BE_KeyCodes keyCode) {
        return BE_IsEngineKeyCodeValid(keyCode) && keysPressed[keyCode];
    }

    int BE_IsKeyUp(BE_KeyCodes keyCode) {
        return BE_IsEngineKeyCodeValid(keyCode) && !keysDown[keyCode] && !keysPressed[keyCode];
    }

    BE_KeyCodes BE_SDLToEngineKeyCode(SDL_Scancode keyCode) {
        (void) keyCode;
#ifndef BACON_ENGINE_DISABLE_SDL
        return keyCode != SDL_SCANCODE_NONUSHASH && keyCode != SDL_SCANCODE_NONUSBACKSLASH && keyCode != SDL_SCANCODE_POWER &&
               BE_IsEngineKeyCodeValid((BE_KeyCodes) keyCode) ? (BE_KeyCodes) keyCode : BE_KEY_CODE_NULL;
#else
        return BE_KEY_CODE_NULL;
#endif
    }

    int BE_IsEngineKeyCodeValid(BE_KeyCodes keyCode) {
        return keyCode < BE_KEY_CODE_SIZE && ((int) keyCodesCharArray[keyCode]) != 1;
    }
CPP_GUARD_END()
