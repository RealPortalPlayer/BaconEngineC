#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/Debugging/Assert.h>

#include "BaconEngine/Rendering/UIElement.h"
#include "BaconEngine/Rendering/UI.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/Window.h"

SEC_CPP_GUARD_START()
    int UIElementsNoOperation(void) {
        return 0;
    }

    void LabelOnRender(BE_UIElement* this, BE_UI_Window* uiWindow, double deltaTime) {
        (void) this;
        (void) uiWindow;
        (void) deltaTime;

        if (BE_UI_GetWindowFont() == NULL)
            return;

#ifndef BACON_ENGINE_DISABLE_SDL_TTF
        BE_Vector_2I uiPosition = BE_UIElement_GetUIElementPosition(this, *uiWindow);
        SDL_Surface* contentsSurface = TTF_RenderText_Solid(BE_UI_GetWindowFont(), this->variables.str1, (SDL_Color) {255, 255, 255, 255});
        SDL_Texture* contentsTexture = SDL_CreateTextureFromSurface(BE_Window_GetInternalSDLRenderer(), contentsSurface);

        SDL_RenderCopy(BE_Window_GetInternalSDLRenderer(), contentsTexture, NULL, &(SDL_Rect) {uiPosition.x, uiPosition.y, (int) contentsSurface->w, (int) contentsSurface->h});
        SDL_DestroyTexture(contentsTexture);
        SDL_FreeSurface(contentsSurface);
#endif
    }

    void ButtonOnRender(BE_UIElement* this, BE_UI_Window* uiWindow, double deltaTime) {
        (void) deltaTime;

        BE_Vector_2I uiPosition = BE_UIElement_GetUIElementPosition(this, *uiWindow);

        BE_UI_WINDOW_RENDERER_DRAW_RECTANGLE_SAME_COLOR(uiWindow, ((BE_Vector_2I) {uiPosition.x + 5, uiPosition.y + 5}), this->size, ((BE_Color_4U) {255, 255, 255, 255}), 2);

#ifndef BACON_ENGINE_DISABLE_SDL_TTF
        if (BE_UI_GetWindowFont() != NULL) {
            SDL_Surface* contentsSurface = TTF_RenderText_Solid(BE_UI_GetWindowFont(), this->variables.str1, (SDL_Color) {0, 0, 0, 255});
            SDL_Texture* contentsTexture = SDL_CreateTextureFromSurface(BE_Window_GetInternalSDLRenderer(), contentsSurface);
            BE_Vector_2I centered = BE_Renderer_GetCenterPosition((BE_Vector_2I) {uiPosition.x + 5, uiPosition.y + 5},
                                                                  this->size, (BE_Vector_2U) {contentsSurface->w,
                                                                                              contentsSurface->h});

            SDL_RenderCopy(BE_Window_GetInternalSDLRenderer(), contentsTexture, NULL, &(SDL_Rect) {centered.x, centered.y, contentsSurface->w, contentsSurface->h});
            SDL_DestroyTexture(contentsTexture);
            SDL_FreeSurface(contentsSurface);
        }
#endif
    }

    int ButtonOnEvent(BE_UIElement* this, BE_UI_Window* uiWindow, BE_Event event) {
        if (!BE_UIElement_IsUserTargetingElement(this, *uiWindow) || !this->enabled || this->variables.any1 == NULL || event.type != BE_EVENT_TYPE_MOUSE_BUTTON_DOWN || event.unionVariables.mouse.unionVariables.unionVariables.button.which != BE_MOUSE_BUTTON_TYPE_LEFT)
            return 0;

        printf("Clicked!\n");

        this->variables.func1((BE_UIElement_PublicVariables) {.any1 = NULL});
        return 1;
    }

    void BoxOnRender(BE_UIElement* this, BE_UI_Window* uiWindow, double deltaTime) {
        (void) deltaTime;
        BE_UI_WINDOW_RENDERER_DRAW_RECTANGLE_SAME_COLOR(uiWindow, BE_UIElement_GetUIElementPosition(this, *uiWindow), this->size,
                                                        this->color, 2);
    }

    BE_UIElement* BE_UIElement_Create(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color, int enabled, BE_UIElement_Functions functions, BE_UIElement_PublicVariables variables) {
        (void) position;
        (void) size;
        (void) color;
        (void) enabled;
        (void) functions;
        (void) variables;

#ifndef BACON_ENGINE_DISABLE_SDL
        BE_UIElement* element;

        SEC_ASSERT_MALLOC(element, sizeof(BE_UIElement), "a UI element");

        element->position = position;
        element->size = size;
        element->color = color;
        element->enabled = enabled;
        element->functions = (BE_UIElement_Functions) {
            .OnRender = functions.OnRender != NULL ? functions.OnRender : (void (*)(BE_UIElement*, BE_UI_Window*, double)) &UIElementsNoOperation,
            .OnEvent = functions.OnEvent != NULL ? functions.OnEvent : (int (*)(BE_UIElement*, BE_UI_Window*, BE_Event)) &UIElementsNoOperation
        };
        element->variables = variables;

        return element;
#else
        return NULL;
#endif
    }

    BE_UIElement* BE_UIElement_CreateLabel(const char* contents, BE_Vector_2I position) {
        BE_Vector_2U size = BE_Renderer_GetFontSize(BE_UI_GetWindowFont(), contents);

        return BE_UIElement_Create(position, size, (BE_Color_4U) {0, 0, 0, 0}, 1, (BE_UIElement_Functions) {
                .OnRender = &LabelOnRender
        }, (BE_UIElement_PublicVariables) {
                .str1 = (char *) contents
        });
    }

    BE_UIElement* BE_UIElement_CreateButton(const char* contents, BE_Vector_2I position, BE_Vector_2U size, void (*UIButtonOnClick)(void)) {
        BE_Vector_2U fontSize = BE_Renderer_GetFontSize(BE_UI_GetWindowFont(), contents);

        if (BE_UI_GetWindowFont() == NULL) {
            fontSize.x = 75;
            fontSize.y = 25;
        }

        if (size.x < fontSize.x)
            size.x = fontSize.x;

        if (size.y < fontSize.y)
            size.y = fontSize.y;

        return BE_UIElement_Create(position, size, (BE_Color_4U) {0, 0, 0, 0}, 1, (BE_UIElement_Functions) {
                .OnRender = &ButtonOnRender,
                .OnEvent = &ButtonOnEvent
        }, (BE_UIElement_PublicVariables) {
                .str1 = (char *) contents,
                .func1 = (void *(*)(struct BE_UIElement_PublicVariables variables)) UIButtonOnClick
        });
    }

    BE_UIElement* BE_UIElement_CreateInputBox(const char* input, const char* placeholder, BE_Vector_2I position, BE_Vector_2I size) {
        (void)input;
        (void)placeholder;
        (void)position;
        (void)size;
        return NULL;
    }

    BE_UIElement* BE_UIElement_CreateBox(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color) {
        return BE_UIElement_Create(position, size, color, 1, (BE_UIElement_Functions) {
                .OnRender = &BoxOnRender
        }, (BE_UIElement_PublicVariables) {
                .any1 = NULL
        });
    }

    int BE_UIElement_IsUserTargetingElement(BE_UIElement* element, BE_UI_Window uiWindow) {
        (void) element;
        (void) uiWindow;

#ifndef BACON_ENGINE_DISABLE_SDL
        SDL_Point mousePosition = {0, 0};
        BE_Vector_2I uiPosition = BE_UIElement_GetUIElementPosition(element, uiWindow);

        SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

        return SDL_PointInRect(&mousePosition, &(SDL_Rect) {uiPosition.x, uiPosition.y, (int) uiWindow.size.x, (int) uiWindow.size.y});
#else
        return 0;
#endif
    }

    BE_Vector_2I BE_UIElement_GetUIElementPosition(BE_UIElement* element, BE_UI_Window uiWindow) {
        BE_Vector_2I position = uiWindow.position;

        if ((uiWindow.flags & BE_UI_WINDOW_FLAG_MAXIMIZED) != 0)
            position = (BE_Vector_2I) {0, 0};

        if ((uiWindow.flags & BE_UI_WINDOW_FLAG_NO_BORDER) == 0) {
            if ((uiWindow.flags & BE_UI_WINDOW_FLAG_MAXIMIZED) != 0)
                position = (BE_Vector_2I) {5, 5};

            position.x += 2;
            position.y += 2;
        }

        if ((uiWindow.flags & BE_UI_WINDOW_FLAG_NO_TITLE_BAR) == 0)
            position.y += 20;

        position.x += element->position.x + 5;
        position.y += element->position.y + 5;

        return position;
    }
SEC_CPP_GUARD_END()
