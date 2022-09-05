#include <SharedEngineCode/Internal/CppHeader.h>
#include <ctype.h>
#include <SharedEngineCode/Debugging/StrictMode.h>
#include <string.h>

#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/Window.h"

SEC_CPP_GUARD_START()
    BE_Color_3U clearColor = {0, 0, 0};
    BE_Renderer_Types currentType;
    int renderCalls = 0;

    void BE_Renderer_SetClearColor(BE_Color_3U color) {
        clearColor = color;
    }

    void BE_Renderer_Initialize(void) {
#ifndef BACON_ENGINE_DISABLE_SDL
        static int initialized = 0;

        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(!initialized, "Already initialized renderer");

        char* value = (char*) SEC_ArgumentHandler_GetValue("--renderer");

        if (value != NULL) {
            for (int i = 0; i < strlen(value); i++)
                value[i] = (char) tolower(value[i]);

            if (strcmp(value, "opengl") == 0) {
                SEC_LOGGER_DEBUG("Using OpenGL renderer");

                currentType = BE_RENDERER_TYPE_OPENGL;
                return;
            }

            if (strcmp(value, "vulkan") == 0) {
                SEC_LOGGER_DEBUG("Using Vulkan renderer");

                currentType = BE_RENDERER_TYPE_VULKAN;
                return;
            }

            if (strcmp(value, "text") == 0) {
                SEC_LOGGER_DEBUG("Using no renderer");

                currentType = BE_RENDERER_TYPE_TEXT;
                return;
            }

            SEC_LOGGER_ERROR("Unknown renderer type: %s", value);
        }

        currentType = BE_RENDERER_TYPE_AUTO;
#else
        SEC_LOG_INFO("SDL has been disabled, forcing text mode");

        currentType = BE_RENDERER_TYPE_TEXT;
#endif
    }

    int BE_Renderer_ClearScreen(void) {
#ifndef BACON_ENGINE_DISABLE_SDL
        renderCalls = 0;

        return BE_Window_GetInternalSDLRenderer() != NULL &&
               SDL_SetRenderDrawColor(BE_Window_GetInternalSDLRenderer(), (Uint8) clearColor.r, (Uint8) clearColor.g, (Uint8) clearColor.b, 255) == 0 &&
               SDL_RenderClear(BE_Window_GetInternalSDLRenderer()) == 0;
#else
        return 0;
#endif
    }

    int BE_Renderer_GetCalls(void) {
        return renderCalls;
    }

    BE_Renderer_Types BE_Renderer_GetCurrentType(void) {
        return currentType;
    }

    int BE_Renderer_IsSoftwareMode(void) {
        static int software = -1;

        if (software == -1)
            software = SEC_ArgumentHandler_GetIndex("--software") > -1;

        return software;
    }

    BE_Color_3U BE_Renderer_GetClearColor(void) {
        return clearColor;
    }

    BE_Vector_2U BE_Renderer_GetFontSize(TTF_Font* font, const char* text) {
        (void) font;
        (void) text;

        BE_Vector_2U size = (BE_Vector_2U) {5, 5};

#ifndef BACON_ENGINE_DISABLE_SDL_TTF
        if (font != NULL) {
            BE_Vector_2U textSize = {0, 0};

            TTF_SizeText(font, text, (int*) &textSize.x, (int*) &textSize.y);

            size.x += textSize.x + 5;
            size.y += textSize.y + 5;
        }
#endif

        return size;
    }

    BE_Vector_2I BE_Renderer_GetCenterPosition(BE_Vector_2I rectanglePosition, BE_Vector_2U rectangleSize, BE_Vector_2U objectSize) {
        return (BE_Vector_2I) {
            rectanglePosition.x + (int) rectangleSize.x / 2 - (int) objectSize.x / 2,
            rectanglePosition.y + (int) rectangleSize.y / 2 - (int) objectSize.y / 2
        };
    }

    int BE_Renderer_DrawLine(BE_Vector_2I firstPoint, BE_Vector_2I secondPoint, BE_Color_4U color) {
        (void) firstPoint;
        (void) secondPoint;
        (void) color;

        if (BE_Window_GetInternalSDLRenderer() == NULL)
            return 0;

#ifndef BACON_ENGINE_DISABLE_SDL
        renderCalls++;

        if (color.a != 255)
            SDL_SetRenderDrawBlendMode(BE_Window_GetInternalSDLRenderer(), SDL_BLENDMODE_BLEND);

        return SDL_SetRenderDrawColor(BE_Window_GetInternalSDLRenderer(), (Uint8) color.r, (Uint8) color.g, (Uint8) color.b, (Uint8) color.a) == 0 &&
               SDL_RenderDrawLine(BE_Window_GetInternalSDLRenderer(), firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y) == 0;
#else
        return 0;
#endif
    }

    int BE_Renderer_DrawPoint(BE_Vector_2I position, BE_Color_4U color) {
        (void) position;
        (void) color;

        if (BE_Window_GetInternalSDLRenderer() == NULL)
            return 0;

#ifndef BACON_ENGINE_DISABLE_SDL
        renderCalls++;

        if (color.a != 255)
            SDL_SetRenderDrawBlendMode(BE_Window_GetInternalSDLRenderer(), SDL_BLENDMODE_BLEND);

        return SDL_SetRenderDrawColor(BE_Window_GetInternalSDLRenderer(), (Uint8) color.r, (Uint8) color.g, (Uint8) color.b, (Uint8) color.a) == 0 &&
               SDL_RenderDrawPoint(BE_Window_GetInternalSDLRenderer(), position.x, position.y) == 0;
#else
        return 0;
#endif
    }

    int BE_Renderer_DrawRectangle(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color) {
        (void) position;
        (void) size;
        (void) color;

        if (BE_Window_GetInternalSDLRenderer() == NULL)
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
            SDL_SetRenderDrawBlendMode(BE_Window_GetInternalSDLRenderer(), SDL_BLENDMODE_BLEND);

        return SDL_SetRenderDrawColor(BE_Window_GetInternalSDLRenderer(), (Uint8) color.r, (Uint8) color.g, (Uint8) color.b, (Uint8) color.a) == 0 &&
               SDL_RenderDrawRect(BE_Window_GetInternalSDLRenderer(), &rectangle) == 0;
#else
        return 0;
#endif
    }

    int BE_Renderer_FillRectangle(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color) {
        (void) position;
        (void) size;
        (void) color;

        if (BE_Window_GetInternalSDLRenderer() == NULL)
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
            SDL_SetRenderDrawBlendMode(BE_Window_GetInternalSDLRenderer(), SDL_BLENDMODE_BLEND);

        return SDL_SetRenderDrawColor(BE_Window_GetInternalSDLRenderer(), (Uint8) color.r, (Uint8) color.g, (Uint8) color.b, (Uint8) color.a) == 0 &&
               SDL_RenderFillRect(BE_Window_GetInternalSDLRenderer(), &rectangle) == 0;
#else
        return 0;
#endif
    }

    int BE_Renderer_DrawBorderedRectangle(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U borderColor, BE_Color_4U fillColor, int borderPadding) {
        return BE_Renderer_DrawRectangle((BE_Vector_2I) {position.x - borderPadding, position.y - borderPadding},
                                         (BE_Vector_2U) {size.x + borderPadding * 2, size.y + borderPadding * 2},
                                         borderColor) &&
                BE_Renderer_FillRectangle(position, size, fillColor);
    }
SEC_CPP_GUARD_END()
