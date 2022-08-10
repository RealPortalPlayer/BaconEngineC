#include <SharedEngineCode/Internal/CppHeader.h>

#include "EngineLayers.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Rendering/Layer.h"
#include "BaconEngine/Rendering/UI.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/Window.h"

CPP_GUARD_START()
    int currentWindowIndex = 0;
    int renderCount;

    void UIManagerOnUpdate(LayerUpdateTypes updateType, double deltaTime);
    int UIManagerOnEvent(SDL_Event event);

    void InitializeEngineLayers(void) {
        static int initialized = 0;

        ASSERT(!initialized, "Engine layers are already initialized");

        initialized = 1;

        RegisterLayer("UIManager", 1, (ClientLayerFunctions) {
            .LayerOnUpdate = &UIManagerOnUpdate,
            .LayerOnEvent = &UIManagerOnEvent
        });
    }

    int GetUIWindowRenderCount(void) {
        return renderCount;
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
            unsigned int bottomPosition;
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

            RendererDrawLine(contentsPosition, (Vector2I) {contentsPosition.x + uiSize.x + 3, contentsPosition.y}, mainColors);

            while (1) {
                RendererDrawLine((Vector2I){contentsPosition.x, contentsPosition.y + borderOffset}, (Vector2I) {contentsPosition.x, contentsPosition.y + borderLengths}, mainColors);
                RendererDrawLine((Vector2I) {contentsPosition.x + uiSize.x + 3, contentsPosition.y + borderOffset}, (Vector2I) {contentsPosition.x + uiSize.x + 3, contentsPosition.y + borderLengths}, mainColors);

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

            RendererDrawLine((Vector2I) {contentsPosition.x, contentsPosition.y + bottomPosition + bottomOffset}, (Vector2I) {contentsPosition.x + uiSize.x + 3, contentsPosition.y + bottomPosition + bottomOffset}, mainColors);
        }

        contentsPosition = uiPosition;

        if ((uiWindow->flags & UI_WINDOW_FLAG_NO_TITLE_BAR) == 0) {
            RendererFillRectangle(contentsPosition, (Vector2U) {(int) uiSize.x, 20}, TITLE_COLORS);

            contentsPosition.y += 20;

            Vector2I buttonPosition = {
                    .x = uiPosition.x + 2,
                    .y = uiPosition.y + 2
            };

            if ((uiWindow->flags & UI_WINDOW_FLAG_NO_CLOSE) == 0 || (uiWindow->flags & UI_WINDOW_FLAG_NO_MINIMIZE) == 0) {
                if ((uiWindow->flags & UI_WINDOW_FLAG_NO_BORDER) == 0)
                    RENDERER_DRAW_FILLED_RECTANGLE_SAME_COLOR(buttonPosition, ((Vector2U) {15, 15}),
                                                              CLOSE_BUTTON_COLORS, 2);
                else
                    RendererFillRectangle(buttonPosition, (Vector2U) {15, 15}, CLOSE_BUTTON_COLORS);

                buttonPosition.x += 18;
            }

            if ((uiWindow->flags & UI_WINDOW_FLAG_NO_MINIMIZE) == 0 || (uiWindow->flags & UI_WINDOW_FLAG_NO_MAXIMIZE) == 0) {
                if ((uiWindow->flags & UI_WINDOW_FLAG_NO_BORDER) == 0)
                    RENDERER_DRAW_FILLED_RECTANGLE_SAME_COLOR(buttonPosition, ((Vector2U) {15, 15}),
                                                              MINIMIZE_BUTTON_COLORS, 2);
                else
                    RendererFillRectangle(buttonPosition, (Vector2U) {15, 15}, MINIMIZE_BUTTON_COLORS);

                buttonPosition.x += 18;
            }

            if ((uiWindow->flags & UI_WINDOW_FLAG_NO_MAXIMIZE) == 0) {
                if ((uiWindow->flags & UI_WINDOW_FLAG_NO_BORDER) == 0)
                    RENDERER_DRAW_FILLED_RECTANGLE_SAME_COLOR(buttonPosition, ((Vector2U) {15, 15}),
                                                              MAXIMIZE_BUTTON_COLORS, 2);
                else
                    RendererFillRectangle(buttonPosition, (Vector2U) {15, 15}, MAXIMIZE_BUTTON_COLORS);
            }
        }

        if ((uiWindow->flags & UI_WINDOW_FLAG_MINIMIZED) == 0)
            RendererFillRectangle(contentsPosition, uiSize, WINDOW_COLORS);

        if ((uiWindow->flags & UI_WINDOW_FLAG_MAXIMIZED) != 0)
            return;
    }

    void UIManagerOnUpdate(LayerUpdateTypes updateType, double deltaTime) {
        if (updateType != LAYER_UPDATE_TYPE_BEFORE_RENDERING)
            return;

        (void) deltaTime;

        renderCount = 0;

        if (GetCurrentUIWindow() != NULL && (GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_MAXIMIZED) != 0) {
            RenderUIWindow(GetCurrentUIWindow());
            return;
        }

        for (int uiWindowId = 0; uiWindowId < GetUIWindowAmount(); uiWindowId++)
            RenderUIWindow(GetUIWindows()[uiWindowId]);
    }

    int UIManagerOnEvent(SDL_Event event) {
        int block = 0;
        (void) block;

        Vector2I mousePosition = {0, 0};
        Vector2I uiPosition;
        Vector2U uiSize;

        SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

        if (GetCurrentUIWindow() != NULL) {
            uiPosition = GetCurrentUIWindow()->position;
            uiSize = GetCurrentUIWindow()->size;

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

            if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_MAXIMIZED) == 0 && !SDL_PointInRect(&(SDL_Point) {mousePosition.x, mousePosition.y}, &(SDL_Rect) {uiPosition.x, uiPosition.y, (int) uiSize.x, (int) uiSize.y})) {
                SetCurrentUIWindow(NULL);
                return 0;
            }
        }

        if (GetCurrentUIWindow() == NULL) {
            for (int uiWindowId = 0; uiWindowId < GetUIWindowAmount(); uiWindowId++) {
                UIWindow* uiWindow = GetUIWindows()[uiWindowId];

                uiPosition = uiWindow->position;
                uiSize = uiWindow->size;

                if ((uiWindow->flags & UI_WINDOW_FLAG_MAXIMIZED) != 0) {
                    uiPosition = (Vector2I){0, 0};
                    uiSize = GetWindowSize();
                    uiPosition.x = 5;
                    uiPosition.y = 5;
                    uiSize.x -= 15;
                    uiSize.y -= 35;
                }

                if ((uiWindow->flags & UI_WINDOW_FLAG_MINIMIZED) != 0)
                    uiSize.y = 20;

                if ((uiWindow->flags & UI_WINDOW_FLAG_NO_BORDER) == 0) {
                    uiPosition.x += 4;
                    uiSize.y += 5;
                }

                if (!SDL_PointInRect(&(SDL_Point) {mousePosition.x, mousePosition.y}, &(SDL_Rect) {uiPosition.x, uiPosition.y, (int) uiSize.x, (int) uiSize.y}))
                    continue;

                SetCurrentUIWindow(uiWindow);
                currentWindowIndex = uiWindowId;
                break;
            }
        }

        if (GetCurrentUIWindow() == NULL)
            return 0;

        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
            {
                SDL_Rect buttonRectangle = {
                    .x = uiPosition.x,
                    .y = uiPosition.y,
                    .w = 15,
                    .h = 15
                };

                for (int buttonId = 0; buttonId < 3; buttonId++) {
                    if (buttonId != 0)
                        buttonRectangle.x += 18;

                    if (!SDL_PointInRect(&(SDL_Point) {mousePosition.x, mousePosition.y}, &buttonRectangle))
                        continue;

                    if (buttonId == 0) {
                        if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_NO_CLOSE) == 0)
                            DeleteWindowAt(currentWindowIndex);

                        return 1;
                    }

                    if (buttonId == 1) {
                        if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_NO_MINIMIZE) == 0) {
                            if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_MAXIMIZED))
                                GetCurrentUIWindow()->flags &= ~UI_WINDOW_FLAG_MAXIMIZED;

                            if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_MINIMIZED) == 0)
                                GetCurrentUIWindow()->flags |= UI_WINDOW_FLAG_MINIMIZED;
                            else
                                GetCurrentUIWindow()->flags &= ~UI_WINDOW_FLAG_MINIMIZED;
                        }

                        return 1;
                    }

                    if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_NO_MAXIMIZE) == 0) {
                        if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_MINIMIZED))
                            GetCurrentUIWindow()->flags &= ~UI_WINDOW_FLAG_MINIMIZED;

                        if ((GetCurrentUIWindow()->flags & UI_WINDOW_FLAG_MAXIMIZED) == 0)
                            GetCurrentUIWindow()->flags |= UI_WINDOW_FLAG_MAXIMIZED;
                        else
                            GetCurrentUIWindow()->flags &= ~UI_WINDOW_FLAG_MAXIMIZED;

                        return 1;
                    }
                }
            }

            break;
        }

        return 0;
    }
CPP_GUARD_END()
