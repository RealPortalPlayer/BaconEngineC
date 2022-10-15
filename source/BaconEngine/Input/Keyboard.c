#include "BaconEngine/Input/Keyboard.h"

SEC_CPP_SUPPORT_GUARD_START()
char keyCodesCharArray[BE_KEYBOARD_KEY_CODE_SIZE] = {
    1,

    '`', '1', '2',
    '3', '4', '5',
    '6', '7', '8',
    '9', '0', '-',
    '=', 0, 0,
    0, 0, 0,
    '/', '*', '-',

    '\t', 'Q', 'W',
    'E', 'R', 'T',
    'Y', 'U', 'I',
    'O', 'P', '[',
    ']', '\\', 0,
    0, 0, '7',
    '8', '9', '+',

    0, 'A', 'S',
    'D', 'F', 'G',
    'H', 'J', 'K',
    'L', ';', '\'',
    '\n', '4', '5',
    '6',

    0, 'Z', 'X',
    'C', 'V', 'B',
    'N', 'M', ',',
    '.', '/', 0,
    0, '1', '2',
    '3', 0,

    0, 0, 0,
    ' ', 0, 0,
    0, 0, 0,
    '0', '.'
};

int keysDown[BE_KEYBOARD_KEY_CODE_SIZE] = {0};
int keysPressed[BE_KEYBOARD_KEY_CODE_SIZE] = {0};

void BE_Keyboard_SetKeyDown(BE_Keyboard_KeyCodes keyCode, int down) {
    if (!BE_Keyboard_IsEngineKeyCodeValid(keyCode))
        return;

    keysDown[keyCode] = down;
}

void BE_Keyboard_SetKeyPressed(BE_Keyboard_KeyCodes keyCode, int pressed) {
    if (!BE_Keyboard_IsEngineKeyCodeValid(keyCode))
        return;

    keysPressed[keyCode] = pressed;
}

char BE_Keyboard_ConvertKeyCodeToChar(BE_Keyboard_KeyCodes keyCode) {
    return (char) (BE_Keyboard_IsEngineKeyCodeValid(keyCode) ? keyCodesCharArray[keyCode] : 0);
}

int BE_Keyboard_IsKeyDown(BE_Keyboard_KeyCodes keyCode) {
    return BE_Keyboard_IsEngineKeyCodeValid(keyCode) && keysDown[keyCode];
}

int BE_Keyboard_IsKeyPressed(BE_Keyboard_KeyCodes keyCode) {
    return BE_Keyboard_IsEngineKeyCodeValid(keyCode) && keysPressed[keyCode];
}

int BE_Keyboard_IsKeyUp(BE_Keyboard_KeyCodes keyCode) {
    return BE_Keyboard_IsEngineKeyCodeValid(keyCode) && !keysDown[keyCode] && !keysPressed[keyCode];
}

int BE_Keyboard_IsEngineKeyCodeValid(BE_Keyboard_KeyCodes keyCode) {
    return keyCode < BE_KEYBOARD_KEY_CODE_SIZE && ((int) keyCodesCharArray[keyCode]) != 1;
}

int BE_Keyboard_IsKeyCodeFromKeyPad(BE_Keyboard_KeyCodes keyCode) {
    return (keyCode >= BE_KEYBOARD_KEY_CODE_KP_NUM_LOCK && keyCode <= BE_KEYBOARD_KEY_CODE_KP_MINUS) ||
           (keyCode >= BE_KEYBOARD_KEY_CODE_KP_SEVEN && keyCode <= BE_KEYBOARD_KEY_CODE_KP_PLUS) ||
           (keyCode >= BE_KEYBOARD_KEY_CODE_KP_FOUR && keyCode <= BE_KEYBOARD_KEY_CODE_KP_SIX) ||
           (keyCode >= BE_KEYBOARD_KEY_CODE_KP_ONE && keyCode <= BE_KEYBOARD_KEY_CODE_KP_RETURN) ||
           keyCode == BE_KEYBOARD_KEY_CODE_KP_ZERO || keyCode == BE_KEYBOARD_KEY_CODE_KP_PERIOD;
}
SEC_CPP_SUPPORT_GUARD_END()
