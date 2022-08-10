#include <SDL.h>
#include <SharedEngineCode/Internal/CppHeader.h>
#include <string.h>
#include <ctype.h>

#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/Window.h"
#include "BaconEngine/Debugging/StrictMode.h"

CPP_GUARD_START()
    Color3U clearColor = {0, 0, 0};
    RendererTypes currentType;

    void SetClearColor(Color3U color) {
        clearColor = color;
    }

    void InitializeRenderer(void) {
        static int initialized = 0;

        STRICT_CHECK_NO_RETURN_VALUE(!initialized, "Already initialized renderer");

        char* value = (char*) GetArgumentValue("--renderer");

        if (value != NULL) {
            {
                int length = 0;

                while (value[length] != '\0') {
                    value[length] = (char) tolower(value[length]);
                    length++;
                }
            }

            if (strcmp(value, "opengl") == 0) {
                currentType = RENDERER_TYPE_OPENGL;
                return;
            }

            if (strcmp(value, "vulkan") == 0) {
                currentType = RENDERER_TYPE_VULKAN;
                return;
            }

            if (strcmp(value, "text") == 0) {
                currentType = RENDERER_TYPE_TEXT;
                return;
            }
        }

        currentType = RENDERER_TYPE_AUTO;
    }

    int ClearScreen(void) {
        return GetInternalSDLRenderer() != NULL &&
               SDL_SetRenderDrawColor(GetInternalSDLRenderer(), (Uint8) clearColor.r, (Uint8) clearColor.g, (Uint8) clearColor.b, 255) == 0 &&
               SDL_RenderClear(GetInternalSDLRenderer()) == 0;
    }

    RendererTypes GetCurrentRenderer(void) {
        return currentType;
    }

    int IsSoftwareRendering(void) {
        static int software = -1;

        if (software == -1)
            software = GetArgumentIndex("--software") > -1;

        return software;
    }

    Color3U GetClearColor(void) {
        return clearColor;
    }

    int RendererDrawLine(Vector2I firstPoint, Vector2I secondPoint, Color4U color) {
        return GetInternalSDLRenderer() != NULL &&
               SDL_SetRenderDrawColor(GetInternalSDLRenderer(), (Uint8) color.r, (Uint8) color.g, (Uint8) color.b, (Uint8) color.a) == 0 &&
               SDL_RenderDrawLine(GetInternalSDLRenderer(), firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y) == 0;
    }

    int RendererDrawPoint(Vector2I position, Color4U color) {
        return GetInternalSDLRenderer() != NULL &&
               SDL_SetRenderDrawColor(GetInternalSDLRenderer(), (Uint8) color.r, (Uint8) color.g, (Uint8) color.b, (Uint8) color.a) == 0 &&
               SDL_RenderDrawPoint(GetInternalSDLRenderer(), position.x, position.y) == 0;
    }

    int RendererDrawRectangle(Vector2I position, Vector2U size, Color4U color) {
        SDL_Rect rectangle = {
            position.x,
            position.y,
            (int) size.x,
            (int) size.y
        };

        return GetInternalSDLRenderer() != NULL &&
               SDL_SetRenderDrawColor(GetInternalSDLRenderer(), (Uint8) color.r, (Uint8) color.g, (Uint8) color.b, (Uint8) color.a) == 0 &&
               SDL_RenderDrawRect(GetInternalSDLRenderer(), &rectangle) == 0;
    }

    int RendererFillRectangle(Vector2I position, Vector2U size, Color4U color) {
        SDL_Rect rectangle = {
            position.x,
            position.y,
            (int) size.x,
            (int) size.y
        };

        return GetInternalSDLRenderer() != NULL &&
               SDL_SetRenderDrawColor(GetInternalSDLRenderer(), (Uint8) color.r, (Uint8) color.g, (Uint8) color.b, (Uint8) color.a) == 0 &&
               SDL_RenderFillRect(GetInternalSDLRenderer(), &rectangle) == 0;
    }

    int RendererDrawFilledRectangle(Vector2I position, Vector2U size, Color4U borderColor, Color4U fillColor, int borderSize) {
        return RendererDrawRectangle(position, size, borderColor) &&
               RendererFillRectangle((Vector2I) {position.x + borderSize, position.y + borderSize}, (Vector2U) {size.x - borderSize * 2, size.y - borderSize * 2}, fillColor);
    }
CPP_GUARD_END()
