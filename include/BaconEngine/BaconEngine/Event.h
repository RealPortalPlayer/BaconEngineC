// Purpose: An event.
// Created on: 8/15/22 @ 4:57 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/SDLSupport.h"
#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/Input/Keyboard.h"
#include "BaconEngine/Input/Mouse.h"

CPP_GUARD_START()
    typedef enum {
        BE_EVENT_TYPE_CLIENT_QUIT,
        BE_EVENT_TYPE_KEYBOARD_KEY_DOWN,
        BE_EVENT_TYPE_KEYBOARD_KEY_UP,
        BE_EVENT_TYPE_INPUT_BOX_TEXT_EDITING,
        BE_EVENT_TYPE_INPUT_BOX_TEXT_INPUT,
        BE_EVENT_TYPE_MOUSE_MOVED,
        BE_EVENT_TYPE_MOUSE_BUTTON_DOWN,
        BE_EVENT_TYPE_MOUSE_BUTTON_UP,
        BE_EVENT_TYPE_MOUSE_WHEEL,
        BE_EVENT_TYPE_WINDOW_SHOWN,
        BE_EVENT_TYPE_WINDOW_HIDDEN,
        BE_EVENT_TYPE_WINDOW_MOVED,
        BE_EVENT_TYPE_WINDOW_RESIZED,
        BE_EVENT_TYPE_WINDOW_MINIMIZED,
        BE_EVENT_TYPE_WINDOW_MAXIMIZED,
        BE_EVENT_TYPE_WINDOW_RESTORED,
        BE_EVENT_TYPE_WINDOW_ENTER,
        BE_EVENT_TYPE_WINDOW_LEAVE,
        BE_EVENT_TYPE_WINDOW_FOCUS_GAINED,
        BE_EVENT_TYPE_WINDOW_FOCUS_LOST,
        BE_EVENT_TYPE_WINDOW_CLOSE
    } BE_EventType;

    typedef union {
        struct {
            union {
                BE_Vector2I newPosition;
                BE_Vector2I newSize;
            } unionVariables;
        } window;
        struct {
            BE_KeyCodes key;
            unsigned repeat;
        } keyboard;
        struct {
            union {
                struct {
                    BE_Vector2I position;
                    union {
                        struct {
                            BE_Vector2I relativePosition;
                        } movement;
                        struct {
                            BE_MouseButtonType which;
                        } button;
                    } unionVariables;
                };
                struct {
                    BE_Vector2I scrollAmount;
                    BE_Vector2F preciseScrollAmount;
                    int flipped;
                } wheel;
            } unionVariables;
        } mouse;
        struct {
            char* text;
            union {
                struct {
                    int start;
                    int length;
                } editing;
            } unionVariables;
        } inputBox;
    } BE_EventUnionVariables;

    typedef struct {
        BE_EventType type;
        BE_EventUnionVariables unionVariables;
    } BE_Event;

    int BE_ConvertSDLToEngineEvent(BE_Event* engineEvent, SDL_Event sdlEvent);
CPP_GUARD_END()
