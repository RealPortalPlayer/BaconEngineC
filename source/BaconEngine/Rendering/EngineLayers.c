#include <SharedEngineCode/Internal/CppHeader.h>

#include "EngineLayers.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Rendering/Layer.h"
#include "BaconEngine/Rendering/UI.h"
#include "BaconEngine/Rendering/Renderer.h"

CPP_GUARD_START()
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

#define TITLE_COLORS (Color4U) {255,255,255,1}
#define WINDOW_COLORS (Color4U) {175,175,175,1}

    void UIManagerOnUpdate(LayerUpdateTypes updateType, double deltaTime) {
        if (updateType != LAYER_UPDATE_TYPE_BEFORE_RENDERING)
            return;

        (void) deltaTime;

        for (int i = 0; i < GetUIWindowAmount(); i++) {
            UIWindow* uiWindow = GetUIWindows()[i];
            Vector2I uiPosition = {
                uiWindow->position.x + 2,
                uiWindow->position.y + 2
            };
            Vector2I newPosition = uiPosition;
            Color4U mainColors = TITLE_COLORS;
            {
                int borderLengths = 21;
                int borderOffset = 0;

                if ((uiWindow->flags & UI_WINDOW_FLAG_NO_TITLE_BAR) != 0)
                    borderLengths = uiWindow->size.y;

                // Top border
                RendererDrawLine(uiWindow->position, (Vector2I) {uiWindow->position.x + uiWindow->size.x + 3, uiWindow->position.y}, mainColors);

                do {
                    // Left border
                    RendererDrawLine((Vector2I){uiWindow->position.x, uiWindow->position.y + borderOffset}, (Vector2I) {uiWindow->position.x, uiWindow->position.y + borderLengths}, mainColors);

                    // Right border
                    RendererDrawLine((Vector2I) {uiWindow->position.x + uiWindow->size.x + 3, uiWindow->position.y + borderOffset}, (Vector2I) {uiWindow->position.x + uiWindow->size.x + 3, uiWindow->position.y + borderLengths}, mainColors);

                    borderLengths = uiWindow->size.y;
                    borderOffset = 21;
                } while (borderLengths == 21);

            }


            if ((uiWindow->flags & UI_WINDOW_FLAG_NO_TITLE_BAR) == 0) {
                newPosition.y += 20;
                mainColors = TITLE_COLORS;

                // Title bar
                RendererFillRectangle(uiPosition, (Vector2I) {uiWindow->size.x, 20}, mainColors);

                Vector2I buttonPosition = {
                    .x = uiPosition.x + 2,
                    .y = uiPosition.y + 2
                };

                if ((uiWindow->flags & UI_WINDOW_FLAG_NO_CLOSE) == 0) {
                    RendererFillRectangle(buttonPosition, (Vector2I) {15, 15}, (Color4U) {
                        255,
                        87,
                        51,
                        1
                    });

                    buttonPosition.x += 20;
                }

                if ((uiWindow->flags & UI_WINDOW_FLAG_NO_MINIMIZE) == 0) {
                    RendererFillRectangle(buttonPosition, (Vector2I) {15, 15}, (Color4U) {
                        255,
                        195,
                        0,
                        1
                    });

                    buttonPosition.x += 20;
                }

                if ((uiWindow->flags & UI_WINDOW_FLAG_NO_MAXIMIZE) == 0)
                    RendererFillRectangle(buttonPosition, (Vector2I) {15, 15}, (Color4U) {
                        218,
                        247,
                        166,
                        1
                    });

                mainColors = WINDOW_COLORS;
            }

            // Main window
            RendererFillRectangle(newPosition, uiWindow->size, mainColors);
        }
    }

    int UIManagerOnEvent(SDL_Event event) {
        (void) event;
        return 0; // TODO: Move elements around.
    }
CPP_GUARD_END()
