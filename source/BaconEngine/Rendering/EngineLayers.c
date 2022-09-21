#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/BuiltInArguments.h>

#include "BaconEngine/Debugging/Assert.h"
#include "EngineLayers.h"
#include "PrivateUI.h"
#include "BaconEngine/Rendering/Layer.h"
#include "BaconEngine/Math/Bitwise.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/Window.h"

SEC_CPP_GUARD_START()
    int renderCount = 0;
    int movingUi = 0;
    BE_Vector_2I moveOffset;
    void BE_EngineLayers_UIRendering_OnUpdate(BE_Layer_UpdateTypes updateType, double deltaTime);
    int BE_EngineLayers_UIRendering_OnEvent(BE_Event event);
    int BE_EngineLayers_ConsoleOpener_OnEvent(BE_Event event);

    void BE_EngineLayers_Initialize(void) {
#ifndef BACON_ENGINE_DISABLE_SDL
        static int initialized = 0;

        BE_ASSERT(!initialized, "Engine layers are already initialized");

        initialized = 1;

        static int disableUIRendering = -1;

        if (disableUIRendering == -1)
            disableUIRendering = SEC_ArgumentHandler_GetIndexWithShort(SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING, SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING_SHORT, 0, NULL, NULL);

        BE_Layer_Register("BE_ConsoleOpener", 1, (BE_Layer_Functions) {
            .OnEvent = &BE_EngineLayers_ConsoleOpener_OnEvent
        });

        if (disableUIRendering)
            return;

        BE_Layer_Register("BE_UIRendering", 1, (BE_Layer_Functions) {
            .OnUpdate = &BE_EngineLayers_UIRendering_OnUpdate,
            .OnEvent = &BE_EngineLayers_UIRendering_OnEvent
        });
#endif
    }

    int BE_EngineLayers_GetUIWindowRenderCount(void) {
        return renderCount;
    }

    int BE_EngineLayers_ConsoleOpener_OnEvent(BE_Event event) {
        if (event.type != BE_EVENT_TYPE_KEYBOARD_KEY_DOWN || event.keyboard.key != BE_KEYBOARD_KEY_CODE_BACKTICK)
            return 0;

        BE_UI_ToggleWindowFlag(0, BE_UI_WINDOW_FLAG_CLOSED, 0);
//        BE_UI_ToggleWindowClosed(0, 0);
        return 1;
    }

#ifndef BACON_ENGINE_DISABLE_SDL
#   define TITLE_COLORS (uiWindow->currentRenderPosition == 0 ? (BE_Color_4U) {60,60,60,255} : (BE_Color_4U) {44,44,44,255})
#   define WINDOW_COLORS (uiWindow->currentRenderPosition == 0 ? (BE_Color_4U) {102,102,102,255} : INACTIVE_WINDOW_COLORS)
#   define CLOSE_BUTTON_COLORS (uiWindow->currentRenderPosition == 0 ? (BE_Color_4U) {249,112,104,255} : INACTIVE_CLOSE_BUTTON_COLORS)
#   define MINIMIZE_BUTTON_COLORS (uiWindow->currentRenderPosition == 0 ? (BE_Color_4U) {255,200,87,255} : INACTIVE_MINIMIZE_BUTTON_COLORS)
#   define MAXIMIZE_BUTTON_COLORS (uiWindow->currentRenderPosition == 0 ? (BE_Color_4U) {78,185,99,255} : (BE_Color_4U) {78,185,99,175})
#   define INACTIVE_WINDOW_COLORS (BE_Color_4U) {73,73,73,255}
#   define INACTIVE_CLOSE_BUTTON_COLORS (BE_Color_4U) {249,112,104,175}
#   define INACTIVE_MINIMIZE_BUTTON_COLORS (BE_Color_4U) {255,200,87,175}

    void BE_EngineLayers_RenderButtons(BE_PrivateUI_Window* uiWindow, BE_Vector_2I position) {
        BE_Vector_2I updatedPosition = position;

        updatedPosition.x += 5;
        updatedPosition.y += 5;

        if (!BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_CLOSE) || !BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_MINIMIZE) || !BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_MAXIMIZE)) {
            if (!BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_CLOSE))
                BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(updatedPosition, ((BE_Vector_2U) {10, 10}), CLOSE_BUTTON_COLORS, 2);
            else
                BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(updatedPosition, ((BE_Vector_2U) {10, 10}), uiWindow->currentRenderPosition == 0 ? WINDOW_COLORS : INACTIVE_WINDOW_COLORS, 2);

            updatedPosition.x += 17;
        }

        if (!BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_MINIMIZE) || !BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_MAXIMIZE)) {
            if (!BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_MINIMIZE))
                BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(updatedPosition, ((BE_Vector_2U) {10, 10}), MINIMIZE_BUTTON_COLORS, 2);
            else
                BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(updatedPosition, ((BE_Vector_2U) {10, 10}), uiWindow->currentRenderPosition == 0 ? WINDOW_COLORS : INACTIVE_WINDOW_COLORS, 2);

            updatedPosition.x += 17;
        }

        if (BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_MAXIMIZE))
            return;

        BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(updatedPosition, ((BE_Vector_2U) {10, 10}), MAXIMIZE_BUTTON_COLORS, 2);
    }

    void BE_EngineLayers_UIRendering_OnUpdate(BE_Layer_UpdateTypes updateType, double deltaTime) {
        if (updateType != BE_LAYER_UPDATE_TYPE_BEFORE_RENDERING)
            return;

        const BE_DynamicArray* uiWindows = BE_PrivateUI_GetRenderWindows();
        BE_Vector_2U windowSize = BE_Window_GetSize();

        renderCount = 0;

        // OPTIMIZE: Only render the maximized window.

        for (int windowId = uiWindows->used - 1; windowId >= 0; windowId--) {
            BE_PrivateUI_Window* uiWindow = BE_DYNAMICARRAY_GET_ELEMENT_PTR(BE_PrivateUI_Window, uiWindows, windowId);

            if (BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_CLOSED))
                continue;

            if (uiWindow->position.x + uiWindow->size.x + 3 > windowSize.x)
                uiWindow->position.x -= uiWindow->position.x + (int) (uiWindow->size.x + 3 - windowSize.x);

            if (uiWindow->position.x < 3)
                uiWindow->position.x = 3;

            if (uiWindow->position.y < 3)
                uiWindow->position.y = 3;

            {
                int height = (int) uiWindow->size.y + 25;

                if (BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_TITLE_BAR))
                    height -= 22;

                if (BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_MINIMIZED))
                    height = 22;

                if (uiWindow->position.y + height + 1 > windowSize.y)
                    uiWindow->position.y -= uiWindow->position.y + 1 + (int) (height - windowSize.y);
            }

            renderCount++;

            BE_Vector_2I position = uiWindow->position;
            BE_Vector_2U size = uiWindow->size;

            // TODO: Calculate position and size depending on if it's maximized.

            if (BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_TITLE_BAR)) {
                BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(position, size, WINDOW_COLORS, 2);
                continue;
            }

            if (BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_MINIMIZED)) {
                BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(position, ((BE_Vector_2U) {size.x, 20}), TITLE_COLORS, 2);
                BE_EngineLayers_RenderButtons(uiWindow, position);
                continue;
            }

            BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(((BE_Vector_2I){position.x, position.y + 23}), size, WINDOW_COLORS, 2);
            BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(position, ((BE_Vector_2U) {size.x, 20}), TITLE_COLORS, 2);
            BE_EngineLayers_RenderButtons(uiWindow, position);
        }
    }

    int BE_EngineLayers_UIRendering_OnEvent(BE_Event event) {
        const BE_DynamicArray* renderOrder = BE_PrivateUI_GetRenderWindows();

//        SDL_Point mousePoint = {
//            event.unionVariables.mouse.unionVariables.position.x,
//            event.unionVariables.mouse.unionVariables.position.y
//        };

        for (int windowId = 0; windowId < renderOrder->used; windowId++) {
            BE_PrivateUI_Window* uiWindow = BE_DYNAMICARRAY_GET_ELEMENT_PTR(BE_PrivateUI_Window, renderOrder, windowId);

            if (BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_CLOSED))
                continue;

            switch (event.type) {
                case BE_EVENT_TYPE_MOUSE_BUTTON_DOWN:
                    if (event.mouse.button.which != BE_MOUSE_BUTTON_TYPE_LEFT)
                        return 0;

//                    if (!SDL_PointInRect(&mousePoint, &(SDL_Rect) {uiWindow->position.x, uiWindow->position.y, (int) uiWindow->size.x, (int) uiWindow->size.y + 24}))
//                        continue;

                    BE_UI_SetActiveWindow(uiWindow->windowId);

                    {
//                        int checkHeight = 24;

//                        if (BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_TITLE_BAR))
//                            checkHeight = (int) uiWindow->size.y;

//                        if (!BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NOT_MOVABLE) &&
//                            SDL_PointInRect(&mousePoint, &(SDL_Rect) {uiWindow->position.x, uiWindow->position.y, (int) uiWindow->size.x + 4, checkHeight})) {
//                            movingUi = 1;
//                            moveOffset = (BE_Vector_2I) {
//                                mousePoint.x - uiWindow->position.x,
//                                mousePoint.y - uiWindow->position.y
//                            };
//                        }
                    }

//                    if (SDL_PointInRect(&mousePoint, &(SDL_Rect) {uiWindow->position.x + 5, uiWindow->position.y + 5, 14, 14}) &&
//                        !BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_CLOSE)) {
//                        BE_UI_ToggleWindowFlag(uiWindow->windowId, BE_UI_WINDOW_FLAG_CLOSED, 1);
//
//                        movingUi = 0;
//                    }
//
//                    if (SDL_PointInRect(&mousePoint, &(SDL_Rect) {uiWindow->position.x + 22, uiWindow->position.y + 5, 14, 14}) &&
//                        !BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_MINIMIZE)) {
//                        BE_BITWISE_TOGGLE_BIT(uiWindow->flags, BE_UI_WINDOW_FLAG_MINIMIZED);
//
//                        movingUi = 0;
//                    }

                    return 1;

                case BE_EVENT_TYPE_MOUSE_BUTTON_UP:
                    movingUi = event.mouse.button.which != BE_MOUSE_BUTTON_TYPE_LEFT;
                    return 1;

                case BE_EVENT_TYPE_MOUSE_MOVED:
                    if (!movingUi)
                        continue;

                    uiWindow->position = (BE_Vector_2I) {
                        (int) event.mouse.position.x - moveOffset.x,
                        (int) event.mouse.position.y - moveOffset.y
                    };
                    return 1;

                default:
                    return 0;
            }
        }

        return 0;
    }
#endif
SEC_CPP_GUARD_END()
