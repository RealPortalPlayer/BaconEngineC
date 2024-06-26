// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <BaconAPI/Debugging/Assert.h>
#include <BaconAPI/Math/Bitwise.h>

#include "EngineLayers.h"
#include "PrivateUI.h"
#include "BaconEngine/Rendering/Layer.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/Window.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
int beEngineLayersRenderCount = 0;
BA_Boolean beEngineLayersMovingUi = BA_BOOLEAN_FALSE;
BE_Vector2_Integer beEngineLayersMoveOffset;

void BE_EngineLayers_UIRendering_OnUpdate(BE_Layer_UpdateTypes updateType);
int BE_EngineLayers_UIRendering_OnEvent(BE_Event event);
int BE_EngineLayers_ConsoleOpener_OnEvent(BE_Event event);

void BE_EngineLayers_Initialize(void) {
    static BA_Boolean initialized = BA_BOOLEAN_FALSE;

    BA_ASSERT(!initialized, "Engine layers are already initialized\n");

    initialized = BA_BOOLEAN_TRUE;

    static int disableUIRendering = -1;

    if (disableUIRendering == -1)
        disableUIRendering = BA_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING,
                                                                         SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING_SHORT, 0);

    BE_Layer_Register("BE_ConsoleOpener", 1, (BE_Layer_Functions) {
        .OnEvent = &BE_EngineLayers_ConsoleOpener_OnEvent
    });

    if (disableUIRendering)
        return;

    BE_Layer_Register("BE_UIRendering", BA_BOOLEAN_TRUE, (BE_Layer_Functions) {
        .OnUpdate = &BE_EngineLayers_UIRendering_OnUpdate,
        .OnEvent = &BE_EngineLayers_UIRendering_OnEvent
    });
}

int BE_EngineLayers_GetUIWindowRenderCount(void) {
    return beEngineLayersRenderCount;
}

BA_Boolean BE_EngineLayers_ConsoleOpener_OnEvent(BE_Event event) {
    if (event.type != BE_EVENT_TYPE_KEYBOARD_KEY_DOWN || event.data.keyboard.key != BE_KEYBOARD_KEY_CODE_BACKTICK)
        return BA_BOOLEAN_FALSE;

    BE_UI_ToggleWindowFlag(0, BE_UI_WINDOW_FLAG_CLOSED, BA_BOOLEAN_FALSE);
    return BA_BOOLEAN_TRUE;
}

#define TITLE_COLORS (uiWindow->currentRenderPosition == 0 ? BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Color4_Unsigned, 60, 60, 60, 255) : BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Color4_Unsigned, 44, 44, 44, 255))
#define WINDOW_COLORS (uiWindow->currentRenderPosition == 0 ? BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Color4_Unsigned, 102, 102, 102, 255) : INACTIVE_WINDOW_COLORS)
#define CLOSE_BUTTON_COLORS (uiWindow->currentRenderPosition == 0 ? BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Color4_Unsigned, 249, 112, 104, 255) : INACTIVE_CLOSE_BUTTON_COLORS)
#define MINIMIZE_BUTTON_COLORS (uiWindow->currentRenderPosition == 0 ? BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Color4_Unsigned, 255, 200, 87, 255) : INACTIVE_MINIMIZE_BUTTON_COLORS)
#define MAXIMIZE_BUTTON_COLORS (uiWindow->currentRenderPosition == 0 ? BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Color4_Unsigned, 78, 185, 99, 255) : BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Color4_Unsigned, 78, 185, 99, 175))
#define INACTIVE_WINDOW_COLORS BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Color4_Unsigned, 73, 73, 73, 255)
#define INACTIVE_CLOSE_BUTTON_COLORS BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Color4_Unsigned, 249, 112, 104, 175)
#define INACTIVE_MINIMIZE_BUTTON_COLORS BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Color4_Unsigned, 255, 200, 87, 175)

void BE_EngineLayers_RenderButtons(BE_PrivateUI_Window* uiWindow, BE_Vector2_Integer position) {
    BE_Vector2_Integer updatedPosition = position;

    updatedPosition.x += 5;
    updatedPosition.y += 5;

    if (!BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_CLOSE) || !BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_MINIMIZE) || !BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_MAXIMIZE)) {
        if (!BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_CLOSE))
            BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(updatedPosition, BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, 10, 10), CLOSE_BUTTON_COLORS, 2);
        else
            BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(updatedPosition, BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, 10, 10), uiWindow->currentRenderPosition == 0 ? WINDOW_COLORS : INACTIVE_WINDOW_COLORS, 2);

        updatedPosition.x += 17;
    }

    if (!BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_MINIMIZE) || !BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_MAXIMIZE)) {
        if (!BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_MINIMIZE))
            BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(updatedPosition, BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, 10, 10), MINIMIZE_BUTTON_COLORS, 2);
        else
            BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(updatedPosition, BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, 10, 10), uiWindow->currentRenderPosition == 0 ? WINDOW_COLORS : INACTIVE_WINDOW_COLORS, 2);

        updatedPosition.x += 17;
    }

    if (BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_MAXIMIZE))
        return;

    BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(updatedPosition, BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, 10, 10), MAXIMIZE_BUTTON_COLORS, 2);
}

void BE_EngineLayers_UIRendering_OnUpdate(BE_Layer_UpdateTypes updateType) {
    if (updateType != BE_LAYER_UPDATE_TYPE_BEFORE_RENDERING)
        return;

    const BA_DynamicArray* uiWindows = BE_PrivateUI_GetRenderWindows();
    BE_Vector2_Unsigned windowSize = BE_Window_GetSize();

    beEngineLayersRenderCount = 0;

    // OPTIMIZE: Only render the maximized window.

    for (int windowId = uiWindows->used - 1; windowId >= 0; windowId--) {
        BE_PrivateUI_Window* uiWindow = BA_DYNAMICARRAY_GET_ELEMENT_POINTER(BE_PrivateUI_Window, uiWindows, windowId);

        if (BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_CLOSED))
            continue;

        if (uiWindow->position.x + uiWindow->size.x + 3 > windowSize.x)
            uiWindow->position.x -= uiWindow->position.x + (int) (uiWindow->size.x + 3 - windowSize.x);

        if (uiWindow->position.x < 3)
            uiWindow->position.x = 3;

        if (uiWindow->position.y < 3)
            uiWindow->position.y = 3;

        {
            int height = (int) uiWindow->size.y + 25;

            if (BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_TITLE_BAR))
                height -= 22;

            if (BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_MINIMIZED))
                height = 22;

            if (uiWindow->position.y + height + 1 > (int) windowSize.y)
                uiWindow->position.y -= uiWindow->position.y + 1 + (int) (height - windowSize.y);
        }

        beEngineLayersRenderCount++;

        BE_Vector2_Integer position = uiWindow->position;
        BE_Vector2_Unsigned size = uiWindow->size;

        // TODO: Calculate position and size depending on if it's maximized.

        if (BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_TITLE_BAR)) {
            BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(position, size, WINDOW_COLORS, 2);
            continue;
        }

        if (BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_MINIMIZED)) {
            BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(position, BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, size.x, 20), TITLE_COLORS, 2);
            BE_EngineLayers_RenderButtons(uiWindow, position);
            continue;
        }

        BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Integer, position.x, position.y + 23), size, WINDOW_COLORS, 2);
        BE_RENDERER_DRAW_BORDERED_RECTANGLE_SAME_COLOR(position, BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, size.x, 20), TITLE_COLORS, 2);
        BE_EngineLayers_RenderButtons(uiWindow, position);
    }
}

BA_Boolean BE_EngineLayers_UIRendering_OnEvent(BE_Event event) {
    const BA_DynamicArray* renderOrder = BE_PrivateUI_GetRenderWindows();

//        SDL_Point mousePoint = {
//            event.unionVariables.mouse.unionVariables.position.x,
//            event.unionVariables.mouse.unionVariables.position.y
//        };

    for (int windowId = 0; windowId < renderOrder->used; windowId++) {
        BE_PrivateUI_Window* uiWindow = BA_DYNAMICARRAY_GET_ELEMENT_POINTER(BE_PrivateUI_Window, renderOrder, windowId);

        if (BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_CLOSED))
            continue;

        switch (event.type) {
            case BE_EVENT_TYPE_MOUSE_BUTTON_DOWN:
                if (event.data.mouse.unionVariables.whichButton != BE_MOUSE_BUTTON_TYPE_LEFT)
                    return BA_BOOLEAN_FALSE;

//              if (!SDL_PointInRect(&mousePoint, &(SDL_Rect) {uiWindow->position.x, uiWindow->position.y, (int) uiWindow->size.x, (int) uiWindow->size.y + 24}))
//                  continue;

                BE_UI_SetActiveWindow(uiWindow->windowId);

                {
//                  int checkHeight = 24;

//                  if (BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_TITLE_BAR))
//                      checkHeight = (int) uiWindow->size.y;

//                  if (!BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NOT_MOVABLE) &&
//                      SDL_PointInRect(&mousePoint, &(SDL_Rect) {uiWindow->position.x, uiWindow->position.y, (int) uiWindow->size.x + 4, checkHeight})) {
//                      beEngineLayersMovingUi = 1;
//                      beEngineLayersMoveOffset = (BE_Vector2_Integer) {
//                          mousePoint.x - uiWindow->position.x,
//                          mousePoint.y - uiWindow->position.y
//                      };
//                  }
              }

//            if (SDL_PointInRect(&mousePoint, &(SDL_Rect) {uiWindow->position.x + 5, uiWindow->position.y + 5, 14, 14}) &&
//                !BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_CLOSE)) {
//                  BE_UI_ToggleWindowFlag(uiWindow->windowId, BE_UI_WINDOW_FLAG_CLOSED, 1);
//
//                  beEngineLayersMovingUi = 0;
//            }
//
//            if (SDL_PointInRect(&mousePoint, &(SDL_Rect) {uiWindow->position.x + 22, uiWindow->position.y + 5, 14, 14}) &&
//                !BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_NO_MINIMIZE)) {
//                  BA_BITWISE_TOGGLE_BIT(uiWindow->flags, BE_UI_WINDOW_FLAG_MINIMIZED);
//
//                  beEngineLayersMovingUi = 0;
//              }
                return BA_BOOLEAN_TRUE;

            case BE_EVENT_TYPE_MOUSE_BUTTON_UP:
                beEngineLayersMovingUi = event.data.mouse.unionVariables.whichButton != BE_MOUSE_BUTTON_TYPE_LEFT;
                return BA_BOOLEAN_TRUE;

            case BE_EVENT_TYPE_MOUSE_MOVED:
                if (!beEngineLayersMovingUi)
                    continue;

                uiWindow->position = (BE_Vector2_Integer) {
                    (int) event.data.mouse.position.x - beEngineLayersMoveOffset.x,
                    (int) event.data.mouse.position.y - beEngineLayersMoveOffset.y
                };
                return BA_BOOLEAN_TRUE;

            default:
                return BA_BOOLEAN_FALSE;
            }
        }

        return BA_BOOLEAN_FALSE;
    }
BA_CPLUSPLUS_SUPPORT_GUARD_END()
