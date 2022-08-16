#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Rendering/UIElements.h"
#include "BaconEngine/Rendering/UI.h"
#include "BaconEngine/Rendering/Window.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Rendering/Renderer.h"

CPP_GUARD_START()
    int UIElementsNoOperation(void) {
        return 0;
    }

    void LabelOnRender(BE_UIElement* this, BE_UIWindow* uiWindow, double deltaTime) {
        (void) deltaTime;

        if (BE_GetUIWindowFont() == NULL)
            return;

#ifndef BACON_ENGINE_DISABLE_SDL_TTF
        BE_Vector2I uiPosition = BE_GetUIElementPosition(this, *uiWindow);
        SDL_Surface* contentsSurface = TTF_RenderText_Solid(BE_GetUIWindowFont(), this->variables.str1, (SDL_Color) {255, 255, 255, 255});
        SDL_Texture* contentsTexture = SDL_CreateTextureFromSurface(GetInternalSDLRenderer(), contentsSurface);

        SDL_RenderCopy(GetInternalSDLRenderer(), contentsTexture, NULL, &(SDL_Rect) {uiPosition.x, uiPosition.y, (int) contentsSurface->w, (int) contentsSurface->h});
        SDL_DestroyTexture(contentsTexture);
        SDL_FreeSurface(contentsSurface);
#endif
    }

    void ButtonOnRender(BE_UIElement* this, BE_UIWindow* uiWindow, double deltaTime) {
        (void) deltaTime;

        BE_Vector2I uiPosition = BE_GetUIElementPosition(this, *uiWindow);

        BE_UI_WINDOW_RENDERER_DRAW_RECTANGLE_SAME_COLOR(uiWindow, ((BE_Vector2I) {uiPosition.x + 5, uiPosition.y + 5}), this->size, ((BE_Color4U) {255, 255, 255, 255}), 2);

#ifndef BACON_ENGINE_DISABLE_SDL_TTF
        if (BE_GetUIWindowFont() != NULL) {
            SDL_Surface* contentsSurface = TTF_RenderText_Solid(BE_GetUIWindowFont(), this->variables.str1, (SDL_Color) {0, 0, 0, 255});
            SDL_Texture* contentsTexture = SDL_CreateTextureFromSurface(GetInternalSDLRenderer(), contentsSurface);
            BE_Vector2I centered = BE_GetCenterPosition((BE_Vector2I) {uiPosition.x + 5, uiPosition.y + 5}, this->size, (BE_Vector2U) {contentsSurface->w, contentsSurface->h});

            SDL_RenderCopy(GetInternalSDLRenderer(), contentsTexture, NULL, &(SDL_Rect) {centered.x, centered.y, contentsSurface->w, contentsSurface->h});
            SDL_DestroyTexture(contentsTexture);
            SDL_FreeSurface(contentsSurface);
        }
#endif
    }

    int ButtonOnEvent(BE_UIElement* this, BE_UIWindow* uiWindow, BE_Event event) {
#ifndef BACON_ENGINE_DISABLE_SDL
        if (!BE_IsUserTargetingElement(this, *uiWindow) || !this->enabled || this->variables.any1 == NULL || event.type != BE_EVENT_TYPE_MOUSE_BUTTON_DOWN || event.unionVariables.mouse.unionVariables.unionVariables.button.which != BE_MOUSE_BUTTON_TYPE_LEFT)
            return 0;

        printf("Clicked!\n");

        ((void (*)(void)) this->variables.any1)();
        return 1;
#else
        return 0;
#endif
    }

    void BoxOnRender(BE_UIElement* this, BE_UIWindow* uiWindow, double deltaTime) {
        (void) deltaTime;
        BE_UI_WINDOW_RENDERER_DRAW_RECTANGLE_SAME_COLOR(uiWindow, BE_GetUIElementPosition(this, *uiWindow), this->size,
                                                        this->color, 2);
    }

    BE_UIElement* BE_CreateUIElement(BE_Vector2I position, BE_Vector2U size, BE_Color4U color, int enabled, UIElementFunctions functions, BE_UIElementPublicVariables variables) {
#ifndef BACON_ENGINE_DISABLE_SDL
        BE_UIElement* element;

        BE_ASSERT_MALLOC(element, sizeof(BE_UIElement), "a UI element");

        element->position = position;
        element->size = size;
        element->color = color;
        element->enabled = enabled;
        element->functions = (UIElementFunctions) {
            .UIElementOnRender = functions.UIElementOnRender != NULL ? functions.UIElementOnRender : (void (*)(BE_UIElement*,BE_UIWindow*,double)) &UIElementsNoOperation,
            .UIElementOnEvent = functions.UIElementOnEvent != NULL ? functions.UIElementOnEvent : (int (*)(BE_UIElement*,BE_UIWindow*,BE_Event)) &UIElementsNoOperation
        };
        element->variables = variables;

        return element;
#else
        return NULL;
#endif
    }

    BE_UIElement* BE_CreateUILabel(const char* contents, BE_Vector2I position) {
        BE_Vector2U size = BE_GetRendererFontSize(BE_GetUIWindowFont(), contents);

        return BE_CreateUIElement(position, size, (BE_Color4U) {0, 0, 0, 0}, 1, (UIElementFunctions) {
                .UIElementOnRender = &LabelOnRender
        }, (BE_UIElementPublicVariables) {
                .str1 = (char *) contents
        });
    }

    BE_UIElement* BE_CreateUIButton(const char* contents, BE_Vector2I position, BE_Vector2U size, void (*UIButtonOnClick)(void)) {
        BE_Vector2U fontSize = BE_GetRendererFontSize(BE_GetUIWindowFont(), contents);

        if (BE_GetUIWindowFont() == NULL) {
            fontSize.x = 75;
            fontSize.y = 25;
        }

        if (size.x < fontSize.x)
            size.x = fontSize.x;

        if (size.y < fontSize.y)
            size.y = fontSize.y;

        return BE_CreateUIElement(position, size, (BE_Color4U) {0, 0, 0, 0}, 1, (UIElementFunctions) {
                .UIElementOnRender = &ButtonOnRender,
                .UIElementOnEvent = &ButtonOnEvent
        }, (BE_UIElementPublicVariables) {
                .str1 = (char *) contents,
                .any1 = (void *) UIButtonOnClick
        });
    }

    BE_UIElement* BE_CreateUIInputBox(const char* input, const char* placeholder, BE_Vector2I position, BE_Vector2I size) {
        return NULL;
    }

    BE_UIElement* BE_CreateUIBox(BE_Vector2I position, BE_Vector2U size, BE_Color4U color) {
        return BE_CreateUIElement(position, size, color, 1, (UIElementFunctions) {
                .UIElementOnRender = &BoxOnRender
        }, (BE_UIElementPublicVariables) {
                .any1 = NULL
        });
    }

    int BE_IsUserTargetingElement(BE_UIElement* element, BE_UIWindow uiWindow) {
#ifndef BACON_ENGINE_DISABLE_SDL
        SDL_Point mousePosition = {0, 0};
        BE_Vector2I uiPosition = BE_GetUIElementPosition(element, uiWindow);

        SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

        return SDL_PointInRect(&mousePosition, &(SDL_Rect) {uiPosition.x, uiPosition.y, (int) uiWindow.size.x, (int) uiWindow.size.y});
#else
        return 0;
#endif
    }

    BE_Vector2I BE_GetUIElementPosition(BE_UIElement* element, BE_UIWindow uiWindow) {
        BE_Vector2I position = uiWindow.position;

        if ((uiWindow.flags & BE_UI_WINDOW_FLAG_MAXIMIZED) != 0)
            position = (BE_Vector2I) {0, 0};

        if ((uiWindow.flags & BE_UI_WINDOW_FLAG_NO_BORDER) == 0) {
            if ((uiWindow.flags & BE_UI_WINDOW_FLAG_MAXIMIZED) != 0)
                position = (BE_Vector2I) {5, 5};

            position.x += 2;
            position.y += 2;
        }

        if ((uiWindow.flags & BE_UI_WINDOW_FLAG_NO_TITLE_BAR) == 0)
            position.y += 20;

        position.x += element->position.x + 5;
        position.y += element->position.y + 5;

        return position;
    }
CPP_GUARD_END()
