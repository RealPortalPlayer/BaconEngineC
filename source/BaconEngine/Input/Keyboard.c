// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconEngine/Input/Keyboard.h"
#include "../InterfaceFunctions.h"
#include "BaconAPI/Debugging/StaticAssert.h"

#ifndef BE_CLIENT_BINARY
#   include "../Platform/SpecificPlatformFunctions.h"
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static char keyCodesCharArray[] = {
    1,
    
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0,
    
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
    0, '0', '.'
};

BA_STATIC_ASSERT_LOOKUP_TABLE_CHECK(keyCodesCharArray, BE_KEYBOARD_KEY_CODE_SIZE);
#endif

char BE_Keyboard_ConvertKeyCodeToCharacter(BE_Keyboard_KeyCodes keyCode) {
#ifndef BE_CLIENT_BINARY
    return (char) (BE_Keyboard_IsEngineKeyCodeValid(keyCode) ? keyCodesCharArray[keyCode] : 0);
#else
    BE_INTERFACEFUNCTION(char, BE_Keyboard_KeyCodes);
    return function(keyCode);
#endif
}

BA_Boolean BE_Keyboard_IsKeyDown(BE_Keyboard_KeyCodes keyCode) {
#ifndef BE_CLIENT_BINARY
    return BE_Keyboard_IsEngineKeyCodeValid(keyCode) && BE_SpecificPlatformFunctions_Get().inputFunctions.IsKeyDown(keyCode);
#else
    BE_INTERFACEFUNCTION(BA_Boolean, BE_Keyboard_KeyCodes);
    return function(keyCode);
#endif
}

BA_Boolean BE_Keyboard_IsKeyPressed(BE_Keyboard_KeyCodes keyCode) {
#ifndef BE_CLIENT_BINARY
    return BE_Keyboard_IsEngineKeyCodeValid(keyCode) && BE_SpecificPlatformFunctions_Get().inputFunctions.IsKeyPressed(keyCode);
#else
    BE_INTERFACEFUNCTION(BA_Boolean, BE_Keyboard_KeyCodes);
    return function(keyCode);
#endif
}

BA_Boolean BE_Keyboard_IsEngineKeyCodeValid(BE_Keyboard_KeyCodes keyCode) {
#ifndef BE_CLIENT_BINARY
    return keyCode < BE_KEYBOARD_KEY_CODE_SIZE && keyCodesCharArray[keyCode] != 1;
#else
    BE_INTERFACEFUNCTION(BA_Boolean, BE_Keyboard_KeyCodes);
    return function(keyCode);
#endif
}

BA_Boolean BE_Keyboard_IsKeyCodeFromKeyPad(BE_Keyboard_KeyCodes keyCode) {
#ifndef BE_CLIENT_BINARY
    return (keyCode >= BE_KEYBOARD_KEY_CODE_KEYPAD_NUM_LOCK && keyCode <= BE_KEYBOARD_KEY_CODE_KEYPAD_MINUS) ||
           (keyCode >= BE_KEYBOARD_KEY_CODE_KEYPAD_SEVEN && keyCode <= BE_KEYBOARD_KEY_CODE_KEYPAD_PLUS) ||
           (keyCode >= BE_KEYBOARD_KEY_CODE_KEYPAD_FOUR && keyCode <= BE_KEYBOARD_KEY_CODE_KEYPAD_SIX) ||
           (keyCode >= BE_KEYBOARD_KEY_CODE_KEYPAD_ONE && keyCode <= BE_KEYBOARD_KEY_CODE_KEYPAD_RETURN) ||
           keyCode == BE_KEYBOARD_KEY_CODE_KEYPAD_ZERO || keyCode == BE_KEYBOARD_KEY_CODE_KEYPAD_PERIOD;
#else
    BE_INTERFACEFUNCTION(BA_Boolean, BE_Keyboard_KeyCodes);
    return function(keyCode);
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
