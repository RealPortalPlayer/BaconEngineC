#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Event.h"

CPP_GUARD_START()
    int BE_ConvertSDLToEngineEvent(BE_Event* engineEvent, SDL_Event sdlEvent) {
        (void) engineEvent;
        (void) sdlEvent;

#ifndef BACON_ENGINE_DISABLE_SDL
        if ((sdlEvent.type >= 257 && sdlEvent.type < 512) || sdlEvent.type > 1027)
            return 0;

        switch (sdlEvent.type) {
            case SDL_QUIT:
                engineEvent->type = BE_EVENT_TYPE_CLIENT_QUIT;
                return 1;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
                BE_KeyCodes keyCode = BE_SDLToEngineKeyCode(sdlEvent.key.keysym.scancode);

                if (keyCode == BE_KEY_CODE_NULL)
                    return 0;

                engineEvent->type = sdlEvent.type == SDL_KEYDOWN ? BE_EVENT_TYPE_KEYBOARD_KEY_DOWN : BE_EVENT_TYPE_KEYBOARD_KEY_UP;
                engineEvent->unionVariables = (BE_EventUnionVariables) {
                    .keyboard = {keyCode, sdlEvent.key.repeat}
                };
            }
                return 1;

            case SDL_TEXTEDITING:
                engineEvent->type =BE_EVENT_TYPE_INPUT_BOX_TEXT_EDITING;
                engineEvent->unionVariables = (BE_EventUnionVariables) {
                    .inputBox = {
                        sdlEvent.edit.text,
                        {{sdlEvent.edit.start, sdlEvent.edit.length}}
                    }
                };
                return 1;

            case SDL_TEXTINPUT:
                engineEvent->type = BE_EVENT_TYPE_INPUT_BOX_TEXT_INPUT;
                engineEvent->unionVariables = (BE_EventUnionVariables) {
                    .inputBox = {sdlEvent.text.text}
                };
                return 1;

            case SDL_MOUSEMOTION:
                engineEvent->type = BE_EVENT_TYPE_MOUSE_MOVED;
                engineEvent->unionVariables = (BE_EventUnionVariables) {
                    .mouse = {
                        {
                            {
                                {sdlEvent.motion.x, sdlEvent.motion.y},
                                {
                                    .movement = {
                                        {sdlEvent.motion.xrel, sdlEvent.motion.yrel}
                                    }
                                }
                            }
                        }
                    }
                };
                return 1;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                engineEvent->type = sdlEvent.type == SDL_MOUSEBUTTONDOWN ? BE_EVENT_TYPE_MOUSE_BUTTON_DOWN : BE_EVENT_TYPE_MOUSE_BUTTON_UP;
                engineEvent->unionVariables = (BE_EventUnionVariables) {
                    .mouse = {
                        {
                            {
                                {sdlEvent.button.x, sdlEvent.button.y},
                                {
                                    .button = {
                                        sdlEvent.button.button
                                    }
                                }
                            }
                        }
                    }
                };
                return 1;

            case SDL_MOUSEWHEEL:
                engineEvent->type = BE_EVENT_TYPE_MOUSE_WHEEL;
                engineEvent->unionVariables = (BE_EventUnionVariables) {
                    .mouse = {
                        {
                                .wheel = {
                                        {sdlEvent.wheel.x, sdlEvent.wheel.y},
                                        {sdlEvent.wheel.preciseX, sdlEvent.wheel.preciseY},
                                        sdlEvent.wheel.direction == SDL_MOUSEWHEEL_FLIPPED
                                }
                        }
                    }
                };
                return 1;

            case SDL_WINDOWEVENT_SHOWN:
                engineEvent->type = BE_EVENT_TYPE_WINDOW_SHOWN;
                return 1;

            case SDL_WINDOWEVENT_HIDDEN:
                engineEvent->type = BE_EVENT_TYPE_WINDOW_HIDDEN;
                return 1;

            case SDL_WINDOWEVENT_MOVED:
                engineEvent->type = BE_EVENT_TYPE_WINDOW_MOVED;
                engineEvent->unionVariables = (BE_EventUnionVariables) {
                    .window = {{{sdlEvent.window.data1, sdlEvent.window.data2}}}
                };
                return 1;

            case SDL_WINDOWEVENT_RESIZED:
                engineEvent->type = BE_EVENT_TYPE_WINDOW_RESIZED;
                engineEvent->unionVariables = (BE_EventUnionVariables) {
                    .window = {
                            {
                                .newSize = {sdlEvent.window.data1, sdlEvent.window.data2}
                            }
                    }
                };
                return 1;

            case SDL_WINDOWEVENT_MINIMIZED:
                engineEvent->type = BE_EVENT_TYPE_WINDOW_MINIMIZED;
                return 1;

            case SDL_WINDOWEVENT_MAXIMIZED:
                engineEvent->type = BE_EVENT_TYPE_WINDOW_MAXIMIZED;
                return 1;

            case SDL_WINDOWEVENT_RESTORED:
                engineEvent->type = BE_EVENT_TYPE_WINDOW_RESTORED;
                return 1;

            case SDL_WINDOWEVENT_ENTER:
                engineEvent->type = BE_EVENT_TYPE_WINDOW_ENTER;
                return 1;

            case SDL_WINDOWEVENT_LEAVE:
                engineEvent->type = BE_EVENT_TYPE_WINDOW_LEAVE;
                return 1;

            case SDL_WINDOWEVENT_FOCUS_GAINED:
                engineEvent->type = BE_EVENT_TYPE_WINDOW_FOCUS_GAINED;
                return 1;

            case SDL_WINDOWEVENT_FOCUS_LOST:
                engineEvent->type = BE_EVENT_TYPE_WINDOW_FOCUS_LOST;
                return 1;

            case SDL_WINDOWEVENT_CLOSE:
                engineEvent->type = BE_EVENT_TYPE_WINDOW_CLOSE;
                return 1;

            default:
                return 0;
        }
#else
        return 0;
#endif
    }
CPP_GUARD_END()
