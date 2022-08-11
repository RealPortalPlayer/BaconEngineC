#include <SharedEngineCode/Internal/CppHeader.h>

#include "EngineLayers.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Rendering/Layer.h"
#include "BaconEngine/Rendering/UI.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/Window.h"

CPP_GUARD_START()
    int currentWindowIndex = 0;
    int renderCount = 0;
    int movingUIWindows = 0;
    Vector2I moveOffset = {0,0};
    int totalRenderCallsCount = 0;

    int CurrentUIUpdaterOnEvent(SDL_Event event);
    void UIManagerOnUpdate(LayerUpdateTypes updateType, double deltaTime);
    int UIManagerOnEvent(SDL_Event event);

    void InitializeEngineLayers(void) {
        static int initialized = 0;

        ASSERT(!initialized, "Engine layers are already initialized");

        initialized = 1;

        SDL_SetRelativeMouseMode(SDL_TRUE);

        RegisterLayer("CurrentUIUpdater", 1, (ClientLayerFunctions) {
            .LayerOnEvent = &CurrentUIUpdaterOnEvent
        });

        RegisterLayer("UIManager", 1, (ClientLayerFunctions) {
            .LayerOnUpdate = &UIManagerOnUpdate,
            .LayerOnEvent = &UIManagerOnEvent
        });
    }

    int GetUIWindowRenderCount(void) {
        return renderCount;
    }

    int GetUIWindowTotalRenderCallCount(void) {
        return totalRenderCallsCount;
    }

#define TITLE_COLORS (Color4U) {60,60,60,1}
#define WINDOW_COLORS (Color4U) {102,102,102,1}
#define CLOSE_BUTTON_COLORS (Color4U) {249,112,104,1}
#define MINIMIZE_BUTTON_COLORS (Color4U) {255,200,87,1}
#define MAXIMIZE_BUTTON_COLORS (Color4U) {78,185,99,1}

    void RenderUIWindow(UIWindow* uiWindow) {
        Vector2I uiPosition = uiWindow->position;
        Vector2U uiSize = uiWindow->size;
        Vector2I contentsPosition = uiPosition;

        renderCount++;

        if ((uiWindow->flags & UI_WINDOW_FLAG_MAXIMIZED) != 0) {
            Vector2U windowSize = GetWindowSize();

            uiPosition.x = 0;
            uiPosition.y = 0;
            contentsPosition.x = 0;
            contentsPosition.y = 0;
            uiSize = (Vector2U) {
                windowSize.x,
                windowSize.y
            };
        }

        if ((uiWindow->flags & UI_WINDOW_FLAG_NO_BORDER) == 0) {
            Color4U mainColors = TITLE_COLORS;
            unsigned int borderLengths = 21;
            unsigned int bottomPosition = 0;
            int borderOffset = 0;
            int bottomOffset = 0;

            if ((uiWindow->flags & UI_WINDOW_FLAG_MAXIMIZED) != 0) {
                uiPosition.x = 5;
                uiPosition.y = 5;
                contentsPosition.x = 5;
                contentsPosition.y = 5;
                uiSize.x -= 15;
                uiSize.y -= 35;
            }

            uiPosition.x += 2;
            uiPosition.y += 2;

            if ((uiWindow->flags & UI_WINDOW_FLAG_NO_TITLE_BAR) != 0) {
                mainColors = WINDOW_COLORS;
                borderLengths = uiSize.y + 2;
                bottomPosition = uiSize.y;
                bottomOffset = 3;
            }

            if ((uiWindow->flags & UI_WINDOW_FLAG_MINIMIZED) != 0) {
                borderLengths = 23;
                bottomPosition = 23;
            }

            RendererDrawLine(contentsPosition, (Vector2I) {contentsPosition.x + (int) uiSize.x + 3, contentsPosition.y}, mainColors);

            totalRenderCallsCount++;

            while (1) {
                RendererDrawLine((Vector2I){contentsPosition.x, contentsPosition.y + borderOffset}, (Vector2I) {contentsPosition.x, contentsPosition.y + (int) borderLengths}, mainColors);
                RendererDrawLine((Vector2I) {contentsPosition.x + (int) uiSize.x + 3, contentsPosition.y + borderOffset}, (Vector2I) {contentsPosition.x + (int) uiSize.x + 3, contentsPosition.y + (int) borderLengths}, mainColors);

                totalRenderCallsCount += 2;

                if (borderLengths == 21) {
                    borderLengths = uiSize.y + 22;
                    bottomPosition = uiSize.y;
                    borderOffset = 22;
                    bottomOffset = 23;
                    mainColors = WINDOW_COLORS;
                    continue;
                }

                break;
            }

            RendererDrawLine((Vector2I) {contentsPosition.x, contentsPosition.y + (int) bottomPosition + bottomOffset}, (Vector2I) {contentsPosition.x + (int) uiSize.x + 3, contentsPosition.y + (int) bottomPosition + bottomOffset}, mainColors);

            totalRenderCallsCount++;
        }

        contentsPosition = uiPosition;

        int titleBarButtons = 0;

        if ((uiWindow->flags & UI_WINDOW_FLAG_NO_CLOSE) == 0 || (uiWindow->flags & UI_WINDOW_FLAG_NO_MINIMIZE) == 0)
            titleBarButtons++;

        if ((uiWindow->flags & UI_WINDOW_FLAG_NO_MINIMIZE) == 0 || (uiWindow->flags & UI_WINDOW_FLAG_NO_MAXIMIZE) == 0)
            titleBarButtons++;

        if ((uiWindow->flags & UI_WINDOW_FLAG_NO_MAXIMIZE) == 0)
            titleBarButtons++;

        if ((uiWindow->flags & UI_WINDOW_FLAG_NO_TITLE_BAR) == 0) {
            RendererFillRectangle(contentsPosition, (Vector2U) {(int) uiSize.x, 20}, TITLE_COLORS);

            totalRenderCallsCount++;

            if (GetUIWindowFont() != NULL) {
                SDL_Surface* titleMessageSurface = TTF_RenderText_Solid(GetUIWindowFont(), uiWindow->name, (SDL_Color) {255, 255, 255, 255});
                SDL_Texture* titleMessageTexture = SDL_CreateTextureFromSurface(GetInternalSDLRenderer(), titleMessageSurface);
                Vector2U titleSize = {0, 0};
                int positionX = uiWindow->position.x;
                int padding = 2 + 18 * titleBarButtons;

                if ((uiWindow->flags & UI_WINDOW_FLAG_MAXIMIZED) != 0)
                    positionX = 0;

                TTF_SizeText(GetUIWindowFont(), uiWindow->name, (int*) &titleSize.x, (int*) &titleSize.y);

                if (uiSize.x + padding > titleSize.x) {
                    SDL_RenderCopy(GetInternalSDLRenderer(), titleMessageTexture, NULL, &(SDL_Rect){positionX + (int) uiSize.x / 2 - (int) titleSize.x / 2, uiPosition.y + 1, (int) titleSize.x, (int) titleSize.y});

                    totalRenderCallsCount++;
                }

                SDL_DestroyTexture(titleMessageTexture);
                SDL_FreeSurface(titleMessageSurface);
            }

            contentsPosition.y += 20;

            Vector2I buttonPosition = {
                    .x = uiPosition.x + 2,
                    .y = uiPosition.y + 2
            };

            if (titleBarButtons >= 1) {
                if ((uiWindow->flags & UI_WINDOW_FLAG_NO_BORDER) == 0) {
                    RENDERER_DRAW_FILLED_RECTANGLE_SAME_COLOR(buttonPosition, ((Vector2U) {15, 15}),
                                                              CLOSE_BUTTON_COLORS, 2);

                    totalRenderCallsCount += 2;
                } else {
                    RendererFillRectangle(buttonPosition, (Vector2U) {15, 15}, CLOSE_BUTTON_COLORS);

                    totalRenderCallsCount++;
                }

                buttonPosition.x += 18;
            }

            if (titleBarButtons >= 2) {
                if ((uiWindow->flags & UI_WINDOW_FLAG_NO_BORDER) == 0) {
                    RENDERER_DRAW_FILLED_RECTANGLE_SAME_COLOR(buttonPosition, ((Vector2U) {15, 15}),
                                                              MINIMIZE_BUTTON_COLORS, 2);

                    totalRenderCallsCount += 2;
                } else {
                    RendererFillRectangle(buttonPosition, (Vector2U) {15, 15}, MINIMIZE_BUTTON_COLORS);

                    totalRenderCallsCount++;
                }

                buttonPosition.x += 18;
            }

            if (titleBarButtons >= 3) {
                if ((uiWindow->flags & UI_WINDOW_FLAG_NO_BORDER) == 0) {
                    RENDERER_DRAW_FILLED_RECTANGLE_SAME_COLOR(buttonPosition, ((Vector2U) {15, 15}),
                                                              MAXIMIZE_BUTTON_COLORS, 2);

                    totalRenderCallsCount += 2;
                } else {
                    RendererFillRectangle(buttonPosition, (Vector2U) {15, 15}, MAXIMIZE_BUTTON_COLORS);

                    totalRenderCallsCount++;
                }
            }
        }

        if ((uiWindow->flags & UI_WINDOW_FLAG_MINIMIZED) == 0) {
            RendererFillRectangle(contentsPosition, uiSize, WINDOW_COLORS);


            totalRenderCallsCount++;
        }

        if ((uiWindow->flags & UI_WINDOW_FLAG_MAXIMIZED) != 0)
            return;
    }

    int CurrentUIUpdaterOnEvent(SDL_Event event) {
        SDL_Point mousePosition = {0, 0};
        SDL_Rect uiRectangle = {0, 0, 0, 0};

        SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

        if (GetCurrentUIWindow() != NULL) {
            uiRectangle.x = GetCurrentUIWindow()->position.x;
            uiRectangle.y = GetCurrentUIWindow()->position.y;
            uiRectangle.w = (int) GetCurrentUIWindow()->size.x;
            uiRectangle.h = (int) GetCurrentUIWindow()->size.y;

            if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_MINIMIZED) != 0)
                uiRectangle.h = 26;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN && (GetCurrentUIWindow() == NULL || ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_MAXIMIZED) == 0 && !SDL_PointInRect(&mousePosition, &uiRectangle)))) {
            LOG_TRACE("Looking for new window");

            for (int windowId = 0; windowId < GetUIWindowAmount(); windowId++) {
                UIWindow* uiWindow = GetUIWindows()[windowId];

                if ((uiWindow->flags & UI_WINDOW_FLAG_CLOSED) != 0)
                    continue;

                uiRectangle.x = uiWindow->position.x;
                uiRectangle.y = uiWindow->position.y;
                uiRectangle.w = (int) uiWindow->size.x;
                uiRectangle.h = (int) uiWindow->size.y;

                if ((uiWindow->flags & UI_WINDOW_FLAG_MINIMIZED) != 0)
                    uiRectangle.h = 26;

                if ((uiWindow->flags & UI_WINDOW_FLAG_MAXIMIZED) == 0)
                    if (!SDL_PointInRect(&mousePosition, &uiRectangle))
                        continue;

                currentWindowIndex = windowId;

                SetCurrentUIWindow(uiWindow);
                break;
            }
        }

        return 0;
    }

    void UIManagerOnUpdate(LayerUpdateTypes updateType, double deltaTime) {
        if (updateType != LAYER_UPDATE_TYPE_BEFORE_RENDERING)
            return;

        (void) deltaTime;

        renderCount = 0;
        totalRenderCallsCount = 0;

        if (GetCurrentUIWindow() != NULL && (GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_MAXIMIZED) != 0) {
            RenderUIWindow(GetCurrentUIWindow());
            return;
        }

        for (int uiWindowId = 0; uiWindowId < GetUIWindowAmount(); uiWindowId++)
            if (GetUIWindows()[uiWindowId] != GetCurrentUIWindow() && (GetUIWindows()[uiWindowId]->flags & UI_WINDOW_FLAG_CLOSED) == 0)
                RenderUIWindow(GetUIWindows()[uiWindowId]);

        if (GetCurrentUIWindow() != NULL)
            RenderUIWindow(GetCurrentUIWindow());
    }

    int UIManagerOnEvent(SDL_Event event) {
        int block = 0;
        (void) block;

        Vector2I mousePosition = {0, 0};
        Vector2I uiPosition;
        Vector2U uiSize;

        SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

        if (GetCurrentUIWindow() == NULL)
            return 0;

        uiPosition = (Vector2I) {
            GetCurrentUIWindow()->position.x,
            GetCurrentUIWindow()->position.y
        };

        uiSize = (Vector2U) {
            GetCurrentUIWindow()->size.x,
            GetCurrentUIWindow()->size.y
        };

        if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_MAXIMIZED) != 0) {
            uiPosition = (Vector2I){0, 0};
            uiSize = GetWindowSize();
            uiPosition.x = 5;
            uiPosition.y = 5;
            uiSize.x -= 15;
            uiSize.y -= 35;
        }

        if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_MINIMIZED) != 0)
            uiSize.y = 20;

        if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_NO_BORDER) == 0) {
            uiPosition.x += 4;
            uiSize.y += 5;
        }

        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
            {
                if (event.button.button != 1)
                    return 1;

                SDL_Rect buttonRectangle = {
                    uiPosition.x + 2,
                    uiPosition.y + 2,
                    15,
                    15
                };

                if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_NO_TITLE_BAR) == 0) {
                    for (int buttonId = 0; buttonId < 3; buttonId++) {
                        if (buttonId != 0)
                            buttonRectangle.x += 18;

                        if (!SDL_PointInRect(&(SDL_Point) {mousePosition.x, mousePosition.y}, &buttonRectangle))
                            continue;

                        if (buttonId == 0) {
                            if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_NO_CLOSE) == 0)
                                CloseUIWindowAt(currentWindowIndex);

                            return 1;
                        }

                        if (buttonId == 1) {
                            if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_NO_MINIMIZE) == 0) {
                                if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_MAXIMIZED) != 0)
                                    GetCurrentUIWindow()->flags &= ~UI_WINDOW_FLAG_MAXIMIZED;

                                if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_MINIMIZED) == 0)
                                    GetCurrentUIWindow()->flags |= UI_WINDOW_FLAG_MINIMIZED;
                                else
                                    GetCurrentUIWindow()->flags &= ~UI_WINDOW_FLAG_MINIMIZED;
                            }

                            return 1;
                        }

                        if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_NO_MAXIMIZE) == 0) {
                            if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_MINIMIZED) != 0)
                                GetCurrentUIWindow()->flags &= ~UI_WINDOW_FLAG_MINIMIZED;

                            if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_MAXIMIZED) == 0)
                                GetCurrentUIWindow()->flags |= UI_WINDOW_FLAG_MAXIMIZED;
                            else
                                GetCurrentUIWindow()->flags &= ~UI_WINDOW_FLAG_MAXIMIZED;

                            return 1;
                        }
                    }
                }

                int checkHeight = 22;

                if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_NO_TITLE_BAR) != 0)
                    checkHeight = (int) GetCurrentUIWindow()->size.y;

                movingUIWindows = (GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_NOT_MOVABLE) == 0 && SDL_PointInRect(&(SDL_Point) {event.motion.x, event.motion.y}, &(SDL_Rect) {uiPosition.x - 2, uiPosition.y - 2, (int) uiSize.x + 4, checkHeight});

                if (movingUIWindows)
                    moveOffset = (Vector2I) {
                        mousePosition.x - uiPosition.x,
                        mousePosition.y - uiPosition.y
                    };
            }
                break;

            case SDL_MOUSEBUTTONUP:
                movingUIWindows = 0;
                break;

            case SDL_MOUSEMOTION:
            {
                UIWindow* currentWindow = GetCurrentUIWindow();

                if (movingUIWindows)
                    currentWindow->position = (Vector2I) {
                        event.motion.x - moveOffset.x,
                        event.motion.y - moveOffset.y
                    };
            }
                break;
        }

        return 0;
    }
CPP_GUARD_END()
