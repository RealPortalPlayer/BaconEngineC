#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/Debugging/Assert.h>

#include "EngineLayers.h"
#include "BaconEngine/Rendering/Layer.h"
#include "BaconEngine/Rendering/UI.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/Window.h"

SEC_CPP_GUARD_START()
    int currentWindowIndex = 0;
    int renderCount = 0;
    int movingUIWindows = 0;
    BE_Vector_2I moveOffset = {0, 0};

#ifndef BACON_ENGINE_DISABLE_SDL
    int CurrentUIUpdaterOnEvent(BE_Event event);
    void UIManagerOnUpdate(BE_Layer_UpdateTypes updateType, double deltaTime);
    int UIManagerOnEvent(BE_Event event);
    void UIManagerOnStop(void);
#endif

    void InitializeEngineLayers(void) {
#ifndef BACON_ENGINE_DISABLE_SDL
        static int initialized = 0;

        SEC_ASSERT(!initialized, "Engine layers are already initialized");

        initialized = 1;

        SDL_SetRelativeMouseMode(SDL_TRUE);

        BE_Layer_Register("CurrentUIUpdater", 1, (BE_Layer_Functions) {
                .OnEvent = &CurrentUIUpdaterOnEvent
        });

        BE_Layer_Register("UIManager", 1, (BE_Layer_Functions) {
                .OnUpdate = &UIManagerOnUpdate,
                .OnEvent = &UIManagerOnEvent,
                .OnStop = &UIManagerOnStop
        });
#endif
    }

    int GetUIWindowRenderCount(void) {
        return renderCount;
    }

#ifndef BACON_ENGINE_DISABLE_SDL
#   define TITLE_COLORS (BE_Color_4U) {60,60,60,255}
#   define WINDOW_COLORS (BE_Color_4U) {102,102,102,255}
#   define CLOSE_BUTTON_COLORS (BE_Color_4U) {249,112,104,255}
#   define MINIMIZE_BUTTON_COLORS (BE_Color_4U) {255,200,87,255}
#   define MAXIMIZE_BUTTON_COLORS (BE_Color_4U) {78,185,99,255}
#   define INACTIVE_TITLE_COLORS (BE_Color_4U) {44,44,44,255}
#   define INACTIVE_WINDOW_COLORS (BE_Color_4U) {73,73,73,255}
#   define INACTIVE_CLOSE_BUTTON_COLORS (BE_Color_4U) {249,112,104,175}
#   define INACTIVE_MINIMIZE_BUTTON_COLORS (BE_Color_4U) {255,200,87,175}
#   define INACTIVE_MAXIMIZE_BUTTON_COLORS (BE_Color_4U) {78,185,99,175}

    void RenderUIWindow(BE_UI_Window* uiWindow) { // TODO: Cut out unnecessary code.
        BE_Vector_2U windowSize = BE_Window_GetSize();

        if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MAXIMIZED) == 0) {
            if (uiWindow->position.x <= 0)
                uiWindow->position.x = 0;

            int offset;
            unsigned height = uiWindow->size.y;

            if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_BORDER) == 0)
                offset = 5;
            else
                offset = 16;


            if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MINIMIZED) != 0)
                height = 20 - offset;

            if (uiWindow->position.x + offset + uiWindow->size.x >= windowSize.x)
                uiWindow->position.x -= uiWindow->position.x + offset + (int) uiWindow->size.x - (int) windowSize.x;

            if (uiWindow->position.y <= 0)
                uiWindow->position.y = 0;


            if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_BORDER) == 0)
                offset += 5;

            if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_TITLE_BAR) == 0)
                offset += 5;

            if (uiWindow->position.y + offset + height >= windowSize.y)
                uiWindow->position.y -= uiWindow->position.y + offset + (int) height - (int) windowSize.y;
        }

        BE_Vector_2I uiPosition = uiWindow->position;
        BE_Vector_2U uiSize = uiWindow->size;

        renderCount++;

        if ((uiWindow->flags & BE_UI_WINDOW_FLAG_INVISIBLE_WINDOW) == 0) {
            if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MAXIMIZED) != 0) {
                uiPosition = (BE_Vector_2I) {0, 0};
                uiSize = (BE_Vector_2U) {windowSize.x, windowSize.y};
            }

            uiPosition.x += 1;
            uiPosition.y += 1;

            if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_BORDER) == 0) {
                if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MAXIMIZED) != 0) {
                    uiPosition = (BE_Vector_2I) {5, 5};
                    uiSize = (BE_Vector_2U) {uiSize.x - 15, uiSize.y - 35};
                }

                if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MINIMIZED) == 0) {
                    BE_Color_4U currentColor = uiWindow == BE_UI_GetCurrentWindow() ? TITLE_COLORS : INACTIVE_TITLE_COLORS;
                    unsigned borderHeights = 21;
                    unsigned bottomLinePosition = 0;
                    int topBarOffset = 0;

                    if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_TITLE_BAR) != 0) {
                        currentColor = uiWindow == BE_UI_GetCurrentWindow() ? WINDOW_COLORS : INACTIVE_WINDOW_COLORS;
                        borderHeights = uiSize.y + 2;
                        bottomLinePosition = uiSize.y + 3;
                    }

                    BE_Renderer_DrawLine(uiPosition, (BE_Vector_2I) {uiPosition.x + (int) uiSize.x + 3, uiPosition.y},
                                         currentColor);

                    while (1) {
                        BE_Renderer_DrawLine((BE_Vector_2I) {uiPosition.x, uiPosition.y + topBarOffset},
                                             (BE_Vector_2I) {uiPosition.x, uiPosition.y + (int) borderHeights},
                                             currentColor);
                        BE_Renderer_DrawLine(
                                (BE_Vector_2I) {uiPosition.x + (int) uiSize.x + 3, uiPosition.y + topBarOffset},
                                (BE_Vector_2I) {uiPosition.x + (int) uiSize.x + 3, uiPosition.y + (int) borderHeights},
                                currentColor);

                        if (borderHeights == 21) {
                            borderHeights = uiSize.y + 22;
                            bottomLinePosition = uiSize.y + 23;
                            topBarOffset = 22;
                            currentColor = uiWindow == BE_UI_GetCurrentWindow() ? WINDOW_COLORS : INACTIVE_WINDOW_COLORS;
                            continue;
                        }

                        break;
                    }

                    BE_Renderer_DrawLine((BE_Vector_2I) {uiPosition.x, uiPosition.y + (int) bottomLinePosition},
                                         (BE_Vector_2I) {uiPosition.x + (int) uiSize.x + 3,
                                                         uiPosition.y + (int) bottomLinePosition}, currentColor);
                }

                uiPosition.x += 2;
                uiPosition.y += 2;
            }

            int titleBarButtons = 0;

            (void) titleBarButtons;

            if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_CLOSE) == 0 || (uiWindow->flags & BE_UI_WINDOW_FLAG_NO_MINIMIZE) == 0 || (uiWindow->flags & BE_UI_WINDOW_FLAG_NO_MAXIMIZE) == 0)
                titleBarButtons++;

            if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_MINIMIZE) == 0 || (uiWindow->flags & BE_UI_WINDOW_FLAG_NO_MAXIMIZE) == 0)
                titleBarButtons++;

            if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_MAXIMIZE) == 0)
                titleBarButtons++;

            if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_TITLE_BAR) == 0) {
                if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MINIMIZED))
                    BE_UI_WINDOW_RENDERER_DRAW_RECTANGLE_SAME_COLOR(uiWindow, uiPosition, ((BE_Vector_2U) {(int) uiSize.x, 20}), uiWindow ==
                                                                                                                                         BE_UI_GetCurrentWindow() ? TITLE_COLORS : INACTIVE_TITLE_COLORS, 2);
                else
                    BE_Renderer_FillRectangle(uiPosition, (BE_Vector_2U) {(int) uiSize.x, 20},
                                              uiWindow == BE_UI_GetCurrentWindow() ? TITLE_COLORS
                                                                                   : INACTIVE_TITLE_COLORS);

#ifndef BACON_ENGINE_DISABLE_SDL_TTF
                if (BE_UI_GetWindowFont() != NULL) {
                    SDL_Surface* titleMessageSurface = TTF_RenderText_Solid(BE_UI_GetWindowFont(), uiWindow->name, (SDL_Color) {255, 255, 255, 255});
                    SDL_Texture* titleMessageTexture = SDL_CreateTextureFromSurface(BE_Window_GetInternalSDLRenderer(), titleMessageSurface);
                    int positionX = uiWindow->position.x;

                    if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MAXIMIZED) != 0)
                        positionX = 0;

                    BE_Vector_2I centered = BE_Renderer_GetCenterPosition((BE_Vector_2I) {positionX, uiPosition.y},
                                                                          uiSize,
                                                                          (BE_Vector_2U) {titleMessageSurface->w,
                                                                                          titleMessageSurface->h});

                    if (uiSize.x + 2 + (18 * titleBarButtons) > (unsigned) titleMessageSurface->w)
                        SDL_RenderCopy(BE_Window_GetInternalSDLRenderer(), titleMessageTexture, NULL, &(SDL_Rect){centered.x, uiPosition.y + 1, (int) titleMessageSurface->w, (int) titleMessageSurface->h});

                    SDL_DestroyTexture(titleMessageTexture);
                    SDL_FreeSurface(titleMessageSurface);
                }
#endif

                int buttonPadding = 0;

                if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_CLOSE) == 0 || (uiWindow->flags & BE_UI_WINDOW_FLAG_NO_MINIMIZE) == 0 || (uiWindow->flags & BE_UI_WINDOW_FLAG_NO_MAXIMIZE) == 0) {
                    BE_Color_4U color = CLOSE_BUTTON_COLORS;

                    if (uiWindow != BE_UI_GetCurrentWindow() || (uiWindow->flags & BE_UI_WINDOW_FLAG_NO_CLOSE) != 0)
                        color = (uiWindow->flags & BE_UI_WINDOW_FLAG_NO_CLOSE) == 0 ? INACTIVE_CLOSE_BUTTON_COLORS : uiWindow ==
                                                                                                                             BE_UI_GetCurrentWindow() ? WINDOW_COLORS : INACTIVE_WINDOW_COLORS;

                    BE_UI_WINDOW_RENDERER_DRAW_RECTANGLE_SAME_COLOR(uiWindow, ((BE_Vector_2I) {uiPosition.x + 4, uiPosition.y + 4}), ((BE_Vector_2U) {12, 12}), color, 2);

                    buttonPadding += 18;
                }

                if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_MINIMIZE) == 0 || (uiWindow->flags & BE_UI_WINDOW_FLAG_NO_MAXIMIZE) == 0) {
                    BE_Color_4U color = MINIMIZE_BUTTON_COLORS;

                    if (uiWindow != BE_UI_GetCurrentWindow() || (uiWindow->flags & BE_UI_WINDOW_FLAG_NO_MINIMIZE) != 0)
                        color = (uiWindow->flags & BE_UI_WINDOW_FLAG_NO_MINIMIZE) == 0 ? INACTIVE_MINIMIZE_BUTTON_COLORS : uiWindow ==
                                                                                                                                   BE_UI_GetCurrentWindow() ? WINDOW_COLORS : INACTIVE_WINDOW_COLORS;

                    BE_UI_WINDOW_RENDERER_DRAW_RECTANGLE_SAME_COLOR(uiWindow, ((BE_Vector_2I) {uiPosition.x + 4 + buttonPadding, uiPosition.y + 4}), ((BE_Vector_2U) {12, 12}), color, 2);

                    buttonPadding += 18;
                }

                if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_MAXIMIZE) == 0) {
                    BE_Color_4U color = MAXIMIZE_BUTTON_COLORS;

                    if (uiWindow != BE_UI_GetCurrentWindow())
                        color = INACTIVE_MAXIMIZE_BUTTON_COLORS;

                    BE_UI_WINDOW_RENDERER_DRAW_RECTANGLE_SAME_COLOR(uiWindow,
                                                                    ((BE_Vector_2I) {uiPosition.x + 4 + buttonPadding,
                                                                                     uiPosition.y + 4}),
                                                                    ((BE_Vector_2U) {12, 12}), color, 2);
                }

                uiPosition.y += 20;
            }
        }

        if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MINIMIZED) == 0) {
            if ((uiWindow->flags & BE_UI_WINDOW_FLAG_INVISIBLE_WINDOW) == 0)
                BE_Renderer_FillRectangle(uiPosition, uiSize,
                                          uiWindow == BE_UI_GetCurrentWindow() ? WINDOW_COLORS : INACTIVE_WINDOW_COLORS);

            for (int uiElementId = 0; uiElementId < uiWindow->elements.used; uiElementId++) {
                BE_UIElement* uiElement = uiWindow->elements.internalArray[uiElementId];

                if (uiElement == NULL)
                    continue;

                // TODO: Check if the UI element is still in the window.
                // TODO: Get the deltaTime.

                uiElement->functions.OnRender(uiElement, uiWindow, 0);
            }
        }
    }

    int CurrentUIUpdaterOnEvent(BE_Event event) {
        SDL_Point mousePosition = {0, 0};
        SDL_Rect uiRectangle = {0, 0, 0, 0};

        SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

        if (BE_UI_GetCurrentWindow() != NULL) {
            uiRectangle.x = BE_UI_GetCurrentWindow()->position.x;
            uiRectangle.y = BE_UI_GetCurrentWindow()->position.y;
            uiRectangle.w = (int) BE_UI_GetCurrentWindow()->size.x;
            uiRectangle.h = (int) BE_UI_GetCurrentWindow()->size.y;

            if ((BE_UI_GetCurrentWindow()->flags & BE_UI_WINDOW_FLAG_MINIMIZED) != 0)
                uiRectangle.h = 26;
        }

        if (event.type == BE_EVENT_TYPE_MOUSE_BUTTON_DOWN && (BE_UI_GetCurrentWindow() == NULL || ((BE_UI_GetCurrentWindow()->flags & BE_UI_WINDOW_FLAG_MAXIMIZED) == 0 && !SDL_PointInRect(&mousePosition, &uiRectangle)))) {
            SEC_LOGGER_TRACE("Looking for new window");

            for (int windowId = 0; windowId < BE_UI_GetWindowAmount(); windowId++) {
                BE_UI_Window* uiWindow = BE_UI_GetWindows()[windowId];

                if ((uiWindow->flags & BE_UI_WINDOW_FLAG_CLOSED) != 0)
                    continue;

                uiRectangle.x = uiWindow->position.x;
                uiRectangle.y = uiWindow->position.y;
                uiRectangle.w = (int) uiWindow->size.x;
                uiRectangle.h = (int) uiWindow->size.y;

                if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MINIMIZED) != 0)
                    uiRectangle.h = 26;

                if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MAXIMIZED) == 0)
                    if (!SDL_PointInRect(&mousePosition, &uiRectangle))
                        continue;

                currentWindowIndex = windowId;

                BE_UI_SetCurrentWindow(uiWindow);
                break;
            }
        }

        return 0;
    }

    void UIManagerOnUpdate(BE_Layer_UpdateTypes updateType, double deltaTime) {
        if (updateType != LAYER_UPDATE_TYPE_BEFORE_RENDERING)
            return;

        (void) deltaTime;

        renderCount = 0;

        if (BE_UI_GetCurrentWindow() != NULL && (BE_UI_GetCurrentWindow()->flags & BE_UI_WINDOW_FLAG_MAXIMIZED) != 0 && (BE_UI_GetCurrentWindow()->flags & BE_UI_WINDOW_FLAG_INVISIBLE_WINDOW) != 0) {
            RenderUIWindow(BE_UI_GetCurrentWindow());
            return;
        }

        for (int uiWindowId = 0; uiWindowId < BE_UI_GetWindowAmount(); uiWindowId++)
            if (BE_UI_GetWindows()[uiWindowId] != BE_UI_GetCurrentWindow() && (BE_UI_GetWindows()[uiWindowId]->flags & BE_UI_WINDOW_FLAG_CLOSED) == 0)
                RenderUIWindow(BE_UI_GetWindows()[uiWindowId]);

        if (BE_UI_GetCurrentWindow() != NULL)
            RenderUIWindow(BE_UI_GetCurrentWindow());
    }

    int UIManagerOnEvent(BE_Event event) {
        int block = 0;
        (void) block;

        BE_Vector_2I mousePosition = {0, 0};
        BE_Vector_2I uiPosition;
        BE_Vector_2U uiSize;
        BE_UI_Window* uiWindow = BE_UI_GetCurrentWindow();

        SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

        if (uiWindow == NULL)
            return 0;

        uiPosition = (BE_Vector_2I) {
            uiWindow->position.x,
            uiWindow->position.y
        };

        uiSize = (BE_Vector_2U) {
            uiWindow->size.x,
            uiWindow->size.y
        };

        if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MAXIMIZED) != 0) {
            uiPosition = (BE_Vector_2I){0, 0};
            uiSize = BE_Window_GetSize();
            uiPosition.x = 5;
            uiPosition.y = 5;
            uiSize.x -= 15;
            uiSize.y -= 35;
        }

        if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MINIMIZED) != 0)
            uiSize.y = 20;

        if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_BORDER) == 0) {
            uiPosition.x += 4;
            uiSize.y += 5;
        }

        switch (event.type) {
            case BE_EVENT_TYPE_MOUSE_BUTTON_DOWN:
            {
                if (event.unionVariables.mouse.unionVariables.unionVariables.button.which != BE_MOUSE_BUTTON_TYPE_LEFT)
                    return 1;

                SDL_Rect buttonRectangle = {
                    uiPosition.x + 4,
                    uiPosition.y + 4,
                    12,
                    12
                };

                if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_TITLE_BAR) == 0) {
                    for (int buttonId = 0; buttonId < 3; buttonId++) {
                        if (buttonId != 0)
                            buttonRectangle.x += 18;

                        if (!SDL_PointInRect(&(SDL_Point) {mousePosition.x, mousePosition.y}, &buttonRectangle))
                            continue;

                        if (buttonId == 0) {
                            if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_CLOSE) == 0)
                                BE_UI_CloseWindowAt(currentWindowIndex);

                            return 1;
                        }

                        if (buttonId == 1) {
                            if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_MINIMIZE) == 0) {
                                if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MAXIMIZED) != 0)
                                    uiWindow->flags &= ~BE_UI_WINDOW_FLAG_MAXIMIZED;

                                if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MINIMIZED) == 0)
                                    uiWindow->flags |= BE_UI_WINDOW_FLAG_MINIMIZED;
                                else
                                    uiWindow->flags &= ~BE_UI_WINDOW_FLAG_MINIMIZED;
                            }

                            return 1;
                        }

                        if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_MAXIMIZE) == 0) {
                            if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MINIMIZED) != 0)
                                uiWindow->flags &= ~BE_UI_WINDOW_FLAG_MINIMIZED;

                            if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MAXIMIZED) == 0)
                                uiWindow->flags |= BE_UI_WINDOW_FLAG_MAXIMIZED;
                            else
                                uiWindow->flags &= ~BE_UI_WINDOW_FLAG_MAXIMIZED;

                            return 1;
                        }
                    }
                }

                int checkHeight = 22;

                if ((uiWindow->flags & BE_UI_WINDOW_FLAG_NO_TITLE_BAR) != 0)
                    checkHeight = (int) uiWindow->size.y;

                movingUIWindows = (uiWindow->flags & BE_UI_WINDOW_FLAG_NOT_MOVABLE) == 0 && SDL_PointInRect(&(SDL_Point) {mousePosition.x, mousePosition.y}, &(SDL_Rect) {uiPosition.x - 2, uiPosition.y - 2, (int) uiSize.x + 4, checkHeight});

                if (movingUIWindows)
                    moveOffset = (BE_Vector_2I) {
                        mousePosition.x - uiPosition.x,
                        mousePosition.y - uiPosition.y
                    };
            }
                break;

            case BE_EVENT_TYPE_MOUSE_BUTTON_UP:
                movingUIWindows = 0;
                break;

            case BE_EVENT_TYPE_MOUSE_MOVED:
            {
                if (movingUIWindows)
                    uiWindow->position = (BE_Vector_2I) {
                        event.unionVariables.mouse.unionVariables.position.x - moveOffset.x,
                        event.unionVariables.mouse.unionVariables.position.y - moveOffset.y
                    };
            }
                break;

            default:
                break;
        }

        if ((uiWindow->flags & BE_UI_WINDOW_FLAG_MINIMIZED) == 0) {
            for (int uiElementId = 0; uiElementId < uiWindow->elements.used; ++uiElementId) {
                BE_UIElement* uiElement = uiWindow->elements.internalArray[uiElementId];

                if (uiElement == NULL)
                    continue; // TODO: Update the used array

                if (uiElement->functions.OnEvent(uiElement, uiWindow, event))
                    return 1;
            }
        }

        return (event.type == BE_EVENT_TYPE_MOUSE_BUTTON_DOWN || event.type == BE_EVENT_TYPE_MOUSE_BUTTON_UP || event.type == BE_EVENT_TYPE_MOUSE_WHEEL || event.type == BE_EVENT_TYPE_MOUSE_MOVED) && SDL_PointInRect(&(SDL_Point) {mousePosition.x, mousePosition.y}, &(SDL_Rect) {uiPosition.x, uiPosition.y, (int) uiSize.x, (int) uiSize.y});
    }

    void UIManagerOnStop(void) {
        renderCount = 0;
    }
#endif
SEC_CPP_GUARD_END()
