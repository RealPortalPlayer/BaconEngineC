#include <SharedEngineCode/Internal/CppHeader.h>
#include <string.h>
#include <ctype.h>

#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/Window.h"
#include "BaconEngine/Debugging/StrictMode.h"

CPP_GUARD_START()
    BE_Color3U clearColor = {0, 0, 0};
    BE_RendererTypes currentType;
    int renderCalls = 0;

    void BE_SetClearColor(BE_Color3U color) {
        clearColor = color;
    }

    void BE_InitializeRenderer(void) {
#ifndef BACON_ENGINE_DISABLE_SDL
        static int initialized = 0;

        BE_STRICT_CHECK_NO_RETURN_VALUE(!initialized, "Already initialized renderer");

        char* value = (char*) SEC_GetArgumentValue("--renderer");

        if (value != NULL) {
            {
                int length = 0;

                while (value[length] != '\0') {
                    value[length] = (char) tolower(value[length]);
                    length++;
                }
            }

            if (strcmp(value, "opengl") == 0) {
                SEC_LOG_DEBUG("Using OpenGL renderer");

                currentType = BE_RENDERER_TYPE_OPENGL;
                return;
            }

            if (strcmp(value, "vulkan") == 0) {
                SEC_LOG_DEBUG("Using Vulkan renderer");

                currentType = BE_RENDERER_TYPE_VULKAN;
                return;
            }

            if (strcmp(value, "text") == 0) {
                SEC_LOG_DEBUG("Using no renderer");

                currentType = BE_RENDERER_TYPE_TEXT;
                return;
            }

            SEC_LOG_ERROR("Unknown renderer type: %s", value);
        }

        currentType = BE_RENDERER_TYPE_AUTO;
#else
        SEC_LOG_INFO("SDL has been disabled, forcing text mode");

        currentType = BE_RENDERER_TYPE_TEXT;
#endif
    }

    int BE_ClearScreen(void) {
#ifndef BACON_ENGINE_DISABLE_SDL
        renderCalls = 0;

        return GetInternalSDLRenderer() != NULL &&
               SDL_SetRenderDrawColor(GetInternalSDLRenderer(), (Uint8) clearColor.r, (Uint8) clearColor.g, (Uint8) clearColor.b, 255) == 0 &&
               SDL_RenderClear(GetInternalSDLRenderer()) == 0;
#else
        return 0;
#endif
    }

    int BE_GetRendererCalls(void) {
        return renderCalls;
    }

    BE_RendererTypes BE_GetCurrentRenderer(void) {
        return currentType;
    }

    int BE_IsSoftwareRendering(void) {
        static int software = -1;

        if (software == -1)
            software = SEC_GetArgumentIndex("--software") > -1;

        return software;
    }

    BE_Color3U BE_GetClearColor(void) {
        return clearColor;
    }

    BE_Vector2U BE_GetRendererFontSize(TTF_Font* font, const char* text) {
        BE_Vector2U size = (BE_Vector2U) {5, 5};

#ifndef BACON_ENGINE_DISABLE_SDL_TTF
        if (font != NULL) {
            BE_Vector2U textSize = {0, 0};

            TTF_SizeText(font, text, (int*) &textSize.x, (int*) &textSize.y);

            size.x += textSize.x + 5;
            size.y += textSize.y + 5;
        }
#endif

        return size;
    }

    BE_Vector2I BE_GetCenterPosition(BE_Vector2I rectanglePosition, BE_Vector2U rectangleSize, BE_Vector2U objectSize) {
        return (BE_Vector2I) {
            rectanglePosition.x + (int) rectangleSize.x / 2 - (int) objectSize.x / 2,
            rectanglePosition.y + (int) rectangleSize.y / 2 - (int) objectSize.y / 2
        };
    }

    int BE_RendererDrawLine(BE_Vector2I firstPoint, BE_Vector2I secondPoint, BE_Color4U color) {
        if (GetInternalSDLRenderer() == NULL)
            return 0;

#ifndef BACON_ENGINE_DISABLE_SDL
        renderCalls++;

        if (color.a != 255)
            SDL_SetRenderDrawBlendMode(GetInternalSDLRenderer(), SDL_BLENDMODE_BLEND);

        return SDL_SetRenderDrawColor(GetInternalSDLRenderer(), (Uint8) color.r, (Uint8) color.g, (Uint8) color.b, (Uint8) color.a) == 0 &&
               SDL_RenderDrawLine(GetInternalSDLRenderer(), firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y) == 0;
#else
        return 0;
#endif
    }

    int BE_RendererDrawPoint(BE_Vector2I position, BE_Color4U color) {
        if (GetInternalSDLRenderer() == NULL)
            return 0;

#ifndef BACON_ENGINE_DISABLE_SDL
        renderCalls++;

        if (color.a != 255)
            SDL_SetRenderDrawBlendMode(GetInternalSDLRenderer(), SDL_BLENDMODE_BLEND);

        return SDL_SetRenderDrawColor(GetInternalSDLRenderer(), (Uint8) color.r, (Uint8) color.g, (Uint8) color.b, (Uint8) color.a) == 0 &&
               SDL_RenderDrawPoint(GetInternalSDLRenderer(), position.x, position.y) == 0;
#else
        return 0;
#endif
    }

    int BE_RendererDrawRectangle(BE_Vector2I position, BE_Vector2U size, BE_Color4U color) {
        if (GetInternalSDLRenderer() == NULL)
            return 0;

#ifndef BACON_ENGINE_DISABLE_SDL

        SDL_Rect rectangle = {
            position.x,
            position.y,
            (int) size.x,
            (int) size.y
        };

        renderCalls++;

        if (color.a != 255)
            SDL_SetRenderDrawBlendMode(GetInternalSDLRenderer(), SDL_BLENDMODE_BLEND);

        return SDL_SetRenderDrawColor(GetInternalSDLRenderer(), (Uint8) color.r, (Uint8) color.g, (Uint8) color.b, (Uint8) color.a) == 0 &&
               SDL_RenderDrawRect(GetInternalSDLRenderer(), &rectangle) == 0;
#else
        return 0;
#endif
    }

    int BE_RendererFillRectangle(BE_Vector2I position, BE_Vector2U size, BE_Color4U color) {
        if (GetInternalSDLRenderer() == NULL)
            return 0;

#ifndef BACON_ENGINE_DISABLE_SDL
        SDL_Rect rectangle = {
            position.x,
            position.y,
            (int) size.x,
            (int) size.y
        };

        renderCalls++;

        if (color.a != 255)
            SDL_SetRenderDrawBlendMode(GetInternalSDLRenderer(), SDL_BLENDMODE_BLEND);

        return SDL_SetRenderDrawColor(GetInternalSDLRenderer(), (Uint8) color.r, (Uint8) color.g, (Uint8) color.b, (Uint8) color.a) == 0 &&
               SDL_RenderFillRect(GetInternalSDLRenderer(), &rectangle) == 0;
#else
        return 0;
#endif
    }

    int BE_RendererDrawBorderedRectangle(BE_Vector2I position, BE_Vector2U size, BE_Color4U borderColor, BE_Color4U fillColor, int borderPadding) {
        return BE_RendererDrawRectangle((BE_Vector2I) {position.x - borderPadding, position.y - borderPadding},
                                        (BE_Vector2U) {size.x + borderPadding * 2, size.y + borderPadding * 2},
                                        borderColor) &&
                BE_RendererFillRectangle(position, size, fillColor);
    }
CPP_GUARD_END()
