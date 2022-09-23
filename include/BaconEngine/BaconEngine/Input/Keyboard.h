// Purpose: Every supported keyboard key-code, and their char representation.
// Created on: 8/15/22 @ 1:59 PM

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

SEC_CPP_SUPPORT_GUARD_START()
typedef enum {
    BE_KEYBOARD_KEY_CODE_NULL,

    BE_KEYBOARD_KEY_CODE_ESCAPE,      BE_KEYBOARD_KEY_CODE_F1,          BE_KEYBOARD_KEY_CODE_F2,
    BE_KEYBOARD_KEY_CODE_F3,          BE_KEYBOARD_KEY_CODE_F4,          BE_KEYBOARD_KEY_CODE_F5,
    BE_KEYBOARD_KEY_CODE_F6,          BE_KEYBOARD_KEY_CODE_F7,          BE_KEYBOARD_KEY_CODE_F8,
    BE_KEYBOARD_KEY_CODE_F9,          BE_KEYBOARD_KEY_CODE_F10,         BE_KEYBOARD_KEY_CODE_F11,
    BE_KEYBOARD_KEY_CODE_F12,         BE_KEYBOARD_KEY_CODE_F13,         BE_KEYBOARD_KEY_CODE_F14,
    BE_KEYBOARD_KEY_CODE_F15,         BE_KEYBOARD_KEY_CODE_F16,         BE_KEYBOARD_KEY_CODE_F17,
    BE_KEYBOARD_KEY_CODE_F18,         BE_KEYBOARD_KEY_CODE_F19,         BE_KEYBOARD_KEY_CODE_F20,
    BE_KEYBOARD_KEY_CODE_F21,         BE_KEYBOARD_KEY_CODE_F22,         BE_KEYBOARD_KEY_CODE_F23,
    BE_KEYBOARD_KEY_CODE_F24,         BE_KEYBOARD_KEY_CODE_F25,         BE_KEYBOARD_KEY_CODE_PRINT_SCREEN,
    BE_KEYBOARD_KEY_CODE_SCROLL_LOCK, BE_KEYBOARD_KEY_CODE_PAUSE_BREAK,

    BE_KEYBOARD_KEY_CODE_BACKTICK,  BE_KEYBOARD_KEY_CODE_ONE,         BE_KEYBOARD_KEY_CODE_TWO,
    BE_KEYBOARD_KEY_CODE_THREE,     BE_KEYBOARD_KEY_CODE_FOUR,        BE_KEYBOARD_KEY_CODE_FIVE,
    BE_KEYBOARD_KEY_CODE_SIX,       BE_KEYBOARD_KEY_CODE_SEVEN,       BE_KEYBOARD_KEY_CODE_EIGHT,
    BE_KEYBOARD_KEY_CODE_NINE,      BE_KEYBOARD_KEY_CODE_ZERO,        BE_KEYBOARD_KEY_CODE_HYPHEN,
    BE_KEYBOARD_KEY_CODE_EQUAL,     BE_KEYBOARD_KEY_CODE_BACKSPACE,   BE_KEYBOARD_KEY_CODE_INSERT,
    BE_KEYBOARD_KEY_CODE_HOME,      BE_KEYBOARD_KEY_CODE_PAGE_UP,     BE_KEYBOARD_KEY_CODE_KP_NUM_LOCK,
    BE_KEYBOARD_KEY_CODE_KP_DIVIDE, BE_KEYBOARD_KEY_CODE_KP_MULTIPLY, BE_KEYBOARD_KEY_CODE_KP_MINUS,

    BE_KEYBOARD_KEY_CODE_TAB,      BE_KEYBOARD_KEY_CODE_Q,         BE_KEYBOARD_KEY_CODE_W,
    BE_KEYBOARD_KEY_CODE_E,        BE_KEYBOARD_KEY_CODE_R,         BE_KEYBOARD_KEY_CODE_T,
    BE_KEYBOARD_KEY_CODE_Y,        BE_KEYBOARD_KEY_CODE_U,         BE_KEYBOARD_KEY_CODE_I,
    BE_KEYBOARD_KEY_CODE_O,        BE_KEYBOARD_KEY_CODE_P,         BE_KEYBOARD_KEY_CODE_LBRACKET,
    BE_KEYBOARD_KEY_CODE_RBRACKET, BE_KEYBOARD_KEY_CODE_BACKSLASH, BE_KEYBOARD_KEY_CODE_DELETE /** Not the same as the Apple delete key */,
    BE_KEYBOARD_KEY_CODE_END,      BE_KEYBOARD_KEY_CODE_PAGE_DOWN, BE_KEYBOARD_KEY_CODE_KP_SEVEN,
    BE_KEYBOARD_KEY_CODE_KP_EIGHT, BE_KEYBOARD_KEY_CODE_KP_NINE,   BE_KEYBOARD_KEY_CODE_KP_PLUS,

    BE_KEYBOARD_KEY_CODE_CAPS_LOCK, BE_KEYBOARD_KEY_CODE_A,          BE_KEYBOARD_KEY_CODE_S,
    BE_KEYBOARD_KEY_CODE_D,         BE_KEYBOARD_KEY_CODE_F,          BE_KEYBOARD_KEY_CODE_G,
    BE_KEYBOARD_KEY_CODE_H,         BE_KEYBOARD_KEY_CODE_J,          BE_KEYBOARD_KEY_CODE_K,
    BE_KEYBOARD_KEY_CODE_L,         BE_KEYBOARD_KEY_CODE_SEMI_COLON, BE_KEYBOARD_KEY_CODE_APOSTROPHE,
    BE_KEYBOARD_KEY_CODE_RETURN,    BE_KEYBOARD_KEY_CODE_KP_FOUR,    BE_KEYBOARD_KEY_CODE_KP_FIVE,
    BE_KEYBOARD_KEY_CODE_KP_SIX,

    BE_KEYBOARD_KEY_CODE_LSHIFT,   BE_KEYBOARD_KEY_CODE_Z,         BE_KEYBOARD_KEY_CODE_X,
    BE_KEYBOARD_KEY_CODE_C,        BE_KEYBOARD_KEY_CODE_V,         BE_KEYBOARD_KEY_CODE_B,
    BE_KEYBOARD_KEY_CODE_N,        BE_KEYBOARD_KEY_CODE_M,         BE_KEYBOARD_KEY_CODE_COMMA,
    BE_KEYBOARD_KEY_CODE_PERIOD,   BE_KEYBOARD_KEY_CODE_SLASH,     BE_KEYBOARD_KEY_CODE_RSHIFT,
    BE_KEYBOARD_KEY_CODE_UP_ARROW, BE_KEYBOARD_KEY_CODE_KP_ONE,    BE_KEYBOARD_KEY_CODE_KP_TWO,
    BE_KEYBOARD_KEY_CODE_KP_THREE, BE_KEYBOARD_KEY_CODE_KP_RETURN,

    BE_KEYBOARD_KEY_CODE_LCTRL,       BE_KEYBOARD_KEY_CODE_LWINDOWS,   BE_KEYBOARD_KEY_CODE_LALT,
    BE_KEYBOARD_KEY_CODE_SPACE,       BE_KEYBOARD_KEY_CODE_RALT,       BE_KEYBOARD_KEY_CODE_RWINDOWS,
    BE_KEYBOARD_KEY_CODE_RCTRL,       BE_KEYBOARD_KEY_CODE_LEFT_ARROW, BE_KEYBOARD_KEY_CODE_DOWN_ARROW,
    BE_KEYBOARD_KEY_CODE_RIGHT_ARROW, BE_KEYBOARD_KEY_CODE_KP_ZERO,    BE_KEYBOARD_KEY_CODE_KP_PERIOD,

    /**
      * Not a real key
      */
    BE_KEYBOARD_KEY_CODE_SIZE,

    BE_KEYBOARD_KEY_CODE_BACKQUOTE = BE_KEYBOARD_KEY_CODE_BACKTICK,      BE_KEYBOARD_KEY_CODE_GRAVE = BE_KEYBOARD_KEY_CODE_BACKTICK,             BE_KEYBOARD_KEY_CODE_GRAVE_ACCENT = BE_KEYBOARD_KEY_CODE_BACKTICK,
    BE_KEYBOARD_KEY_CODE_1 = BE_KEYBOARD_KEY_CODE_ONE,                   BE_KEYBOARD_KEY_CODE_2 = BE_KEYBOARD_KEY_CODE_TWO,                      BE_KEYBOARD_KEY_CODE_3 = BE_KEYBOARD_KEY_CODE_THREE,
    BE_KEYBOARD_KEY_CODE_4 = BE_KEYBOARD_KEY_CODE_FOUR,                  BE_KEYBOARD_KEY_CODE_5 = BE_KEYBOARD_KEY_CODE_FIVE,                     BE_KEYBOARD_KEY_CODE_6 = BE_KEYBOARD_KEY_CODE_SIX,
    BE_KEYBOARD_KEY_CODE_7 = BE_KEYBOARD_KEY_CODE_SEVEN,                 BE_KEYBOARD_KEY_CODE_8 = BE_KEYBOARD_KEY_CODE_EIGHT,                    BE_KEYBOARD_KEY_CODE_9 = BE_KEYBOARD_KEY_CODE_NINE,
    BE_KEYBOARD_KEY_CODE_0 = BE_KEYBOARD_KEY_CODE_ZERO,                  BE_KEYBOARD_KEY_CODE_DASH = BE_KEYBOARD_KEY_CODE_HYPHEN,                BE_KEYBOARD_KEY_CODE_MINUS = BE_KEYBOARD_KEY_CODE_HYPHEN,
    BE_KEYBOARD_KEY_CODE_KP_SLASH = BE_KEYBOARD_KEY_CODE_KP_DIVIDE,      BE_KEYBOARD_KEY_CODE_KP_FORWARD_SLASH = BE_KEYBOARD_KEY_CODE_KP_DIVIDE, BE_KEYBOARD_KEY_CODE_KP_ASTERISK = BE_KEYBOARD_KEY_CODE_KP_MULTIPLY,
    BE_KEYBOARD_KEY_CODE_KP_STAR = BE_KEYBOARD_KEY_CODE_KP_MULTIPLY,     BE_KEYBOARD_KEY_CODE_KP_HYPHEN = BE_KEYBOARD_KEY_CODE_KP_MINUS,         BE_KEYBOARD_KEY_CODE_KP_DASH = BE_KEYBOARD_KEY_CODE_KP_MINUS,
    BE_KEYBOARD_KEY_CODE_KP_HOME = BE_KEYBOARD_KEY_CODE_KP_SEVEN,        BE_KEYBOARD_KEY_CODE_KP_7 = BE_KEYBOARD_KEY_CODE_KP_SEVEN,              BE_KEYBOARD_KEY_CODE_KP_UP = BE_KEYBOARD_KEY_CODE_KP_EIGHT,
    BE_KEYBOARD_KEY_CODE_KP_8 = BE_KEYBOARD_KEY_CODE_KP_EIGHT,           BE_KEYBOARD_KEY_CODE_KP_PAGE_UP = BE_KEYBOARD_KEY_CODE_KP_NINE,         BE_KEYBOARD_KEY_CODE_KP_9 = BE_KEYBOARD_KEY_CODE_KP_NINE,
    BE_KEYBOARD_KEY_CODE_SINGLE_QUOTE = BE_KEYBOARD_KEY_CODE_APOSTROPHE, BE_KEYBOARD_KEY_CODE_ENTER = BE_KEYBOARD_KEY_CODE_RETURN,               BE_KEYBOARD_KEY_CODE_KP_LEFT = BE_KEYBOARD_KEY_CODE_KP_FOUR,
    BE_KEYBOARD_KEY_CODE_KP_4 = BE_KEYBOARD_KEY_CODE_KP_FOUR,            BE_KEYBOARD_KEY_CODE_KP_5 = BE_KEYBOARD_KEY_CODE_FIVE,                  BE_KEYBOARD_KEY_CODE_KP_6 = BE_KEYBOARD_KEY_CODE_KP_SIX,
    BE_KEYBOARD_KEY_CODE_DOT = BE_KEYBOARD_KEY_CODE_PERIOD,              BE_KEYBOARD_KEY_CODE_FORWARD_SLASH = BE_KEYBOARD_KEY_CODE_SLASH,        BE_KEYBOARD_KEY_CODE_DIVIDE = BE_KEYBOARD_KEY_CODE_SLASH,
    BE_KEYBOARD_KEY_CODE_KP_END = BE_KEYBOARD_KEY_CODE_KP_ONE,           BE_KEYBOARD_KEY_CODE_KP_1 = BE_KEYBOARD_KEY_CODE_KP_ONE,                BE_KEYBOARD_KEY_CODE_KP_DOWN = BE_KEYBOARD_KEY_CODE_KP_TWO,
    BE_KEYBOARD_KEY_CODE_KP_2 = BE_KEYBOARD_KEY_CODE_KP_TWO,             BE_KEYBOARD_KEY_CODE_KP_PAGE_DOWN = BE_KEYBOARD_KEY_CODE_KP_THREE,      BE_KEYBOARD_KEY_CODE_KP_3 = BE_KEYBOARD_KEY_CODE_KP_THREE,
    BE_KEYBOARD_KEY_CODE_KP_ENTER = BE_KEYBOARD_KEY_CODE_KP_RETURN,      BE_KEYBOARD_KEY_CODE_LCOMMAND = BE_KEYBOARD_KEY_CODE_LWINDOWS,          BE_KEYBOARD_KEY_CODE_LSUPER = BE_KEYBOARD_KEY_CODE_LWINDOWS,
    BE_KEYBOARD_KEY_CODE_LOPTION = BE_KEYBOARD_KEY_CODE_LALT,            BE_KEYBOARD_KEY_CODE_RCOMMAND = BE_KEYBOARD_KEY_CODE_RWINDOWS,          BE_KEYBOARD_KEY_CODE_RSUPER = BE_KEYBOARD_KEY_CODE_RWINDOWS,
    BE_KEYBOARD_KEY_CODE_ROPTION = BE_KEYBOARD_KEY_CODE_RALT,            BE_KEYBOARD_KEY_CODE_KP_INSERT = BE_KEYBOARD_KEY_CODE_KP_ZERO,          BE_KEYBOARD_KEY_CODE_KP_0 = BE_KEYBOARD_KEY_CODE_KP_ZERO,
    BE_KEYBOARD_KEY_CODE_KP_DELETE = BE_KEYBOARD_KEY_CODE_KP_PERIOD,     BE_KEYBOARD_KEY_CODE_KP_DOT = BE_KEYBOARD_KEY_CODE_KP_PERIOD
} BE_Keyboard_KeyCodes;

void BE_Keyboard_SetKeyDown(BE_Keyboard_KeyCodes keyCode, int down);
void BE_Keyboard_SetKeyPressed(BE_Keyboard_KeyCodes keyCode, int pressed);
char BE_Keyboard_ConvertKeyCodeToChar(BE_Keyboard_KeyCodes keyCode);
int BE_Keyboard_IsKeyDown(BE_Keyboard_KeyCodes keyCode);
int BE_Keyboard_IsKeyPressed(BE_Keyboard_KeyCodes keyCode);
int BE_Keyboard_IsKeyUp(BE_Keyboard_KeyCodes keyCode);
int BE_Keyboard_IsEngineKeyCodeValid(BE_Keyboard_KeyCodes keyCode);
int BE_Keyboard_IsKeyCodeFromKeyPad(BE_Keyboard_KeyCodes keyCode);
SEC_CPP_SUPPORT_GUARD_END()
