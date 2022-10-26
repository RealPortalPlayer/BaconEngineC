// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/Internal/OperatingSystem.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <SharedEngineCode/StringExtension.h>

#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "../Platform/SpecificPlatformFunctions.h"
#include "../Platform/TextMode/TextMode.h"

#ifndef BE_DISABLE_OPENGL
#   include "../Platform/OpenGL/OpenGL.h"
#endif

#if SEC_OPERATINGSYSTEM_WINDOWS
#   include "../Platform/Windows/Windows.h"
#endif

#include "BaconEngine/Debugging/Assert.h"

SEC_CPP_SUPPORT_GUARD_START()
BE_Renderer_Types beRendererCurrent;
int renderCalls = 0;

void BE_Renderer_SetClearColor(BE_Color_3U color) {
    BE_SpecificPlatformFunctions_Get().rendererFunctions.SetClearColor(color);
}

// TODO: Make this private.
void BE_PrivateRenderer_Initialize(void) {
    static int initialized = 0;

    BE_STRICTMODE_CHECK_NO_RETURN_VALUE(!initialized, "Already initialized rendererFunctions\n");

    const char* specifiedRenderer = SEC_ArgumentHandler_GetValue(SEC_BUILTINARGUMENTS_RENDERER, 0);

    if (BE_ClientInformation_IsServerModeEnabled() || (specifiedRenderer != NULL && SEC_StringExtension_CompareCaseless(specifiedRenderer, "text") == 0)) {
        SEC_LOGGER_INFO("Using no renderer\n");
        BE_TextMode_Initialize();

        beRendererCurrent = BE_RENDERER_TYPE_TEXT;
        return;
    }

    if (specifiedRenderer != NULL) {
        if (SEC_StringExtension_CompareCaseless(specifiedRenderer, "opengl") == 0) {
            opengl:
#ifndef BE_DISABLE_OPENGL
            SEC_LOGGER_INFO("Using OpenGL as the renderer\n");
            BE_OpenGL_Initialize();

            beRendererCurrent = BE_RENDERER_TYPE_OPENGL;
#else
            SEC_LOGGER_WARN("This binary has OpenGL disabled, defaulting to text-mode\n");
            BE_TextMode_Initialize();

            beRendererCurrent = BE_RENDERER_TYPE_TEXT;
#endif

            return;
        }

        if (SEC_StringExtension_CompareCaseless(specifiedRenderer, "vulkan") == 0) {
            BE_ASSERT_ALWAYS("Renderer not implemented\n");
        }

        if (SEC_StringExtension_CompareCaseless(specifiedRenderer, "metal") == 0) {
#if SEC_OPERATINGSYSTEM_APPLE
            BE_ASSERT_NOT_IMPLEMENTED();
#else
            SEC_LOGGER_WARN("Metal only works on Apple operating systems, defaulting to text-mode\n");
            BE_TextMode_Initialize();

            beRendererCurrent = BE_RENDERER_TYPE_TEXT;
#endif
        }

        if (SEC_StringExtension_CompareCaseless(specifiedRenderer, "directx") == 0) {
            BE_ASSERT_ALWAYS("Renderer not implemented\n");
        }

        if (SEC_StringExtension_CompareCaseless(specifiedRenderer, "software") == 0) {
            SEC_LOGGER_INFO("Using software rendering\n");
            SEC_LOGGER_WARN("This is going to lag; use a better renderer\n");

#if SEC_OPERATINGSYSTEM_WINDOWS
            BE_Windows_Initialize();
#else
            BE_ASSERT_ALWAYS("No software renderer implementation for your OS\n");
#endif
        }

        SEC_LOGGER_WARN("Invalid renderer: %s\n", specifiedRenderer);
    }

    goto opengl; // TODO: Get the best one depending on OS
}

void BE_Renderer_ClearScreen(void) {
    BE_SpecificPlatformFunctions_Get().rendererFunctions.ClearScreen();
}

int BE_Renderer_GetCalls(void) {
    return renderCalls;
}

BE_Renderer_Types BE_Renderer_GetCurrentType(void) {
    return beRendererCurrent;
}

BE_Color_3U BE_Renderer_GetClearColor(void) {
    return BE_SpecificPlatformFunctions_Get().rendererFunctions.GetClearColor();
}

//BE_Vector_2U BE_Renderer_GetFontSize(TTF_Font* font, const char* text) {
//  (void) font;
//  (void) text;
//
//  BE_Vector_2U size = (BE_Vector_2U) {5, 5};
//
//#ifndef BACON_ENGINE_DISABLE_SDL_TTF
//  if (font != NULL) {
//      BE_Vector_2U textSize = {0, 0};
//
//      TTF_SizeText(font, text, (int*) &textSize.x, (int*) &textSize.y);
//
//      size.x += textSize.x + 5;
//      size.y += textSize.y + 5;
//  }
//#endif
//
//  return size;
//}

BE_Vector_2I BE_Renderer_GetCenterPosition(BE_Vector_2I rectanglePosition, BE_Vector_2U rectangleSize, BE_Vector_2U objectSize) {
    return (BE_Vector_2I) {
        rectanglePosition.x + (int) rectangleSize.x / 2 - (int) objectSize.x / 2,
        rectanglePosition.y + (int) rectangleSize.y / 2 - (int) objectSize.y / 2
    };
}

void BE_Renderer_DrawLine(BE_Vector_2I firstPoint, BE_Vector_2I secondPoint, BE_Color_4U color) {
    (void) firstPoint;
    (void) secondPoint;
    (void) color;

//  if (BE_Window_GetInternalSDLRenderer() == NULL)
//      return 0;
//
//#ifndef BACON_ENGINE_DISABLE_SDL
//  renderCalls++;
//
//  if (color.a != 255)
//      SDL_SetRenderDrawBlendMode(BE_Window_GetInternalSDLRenderer(), SDL_BLENDMODE_BLEND);
//
//  return SDL_SetRenderDrawColor(BE_Window_GetInternalSDLRenderer(), (Uint8) color.r, (Uint8) color.g, (Uint8) color.b, (Uint8) color.a) == 0 &&
//         SDL_RenderDrawLine(BE_Window_GetInternalSDLRenderer(), firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y) == 0;
//#else
//  return 0;
//#endif
    }

void BE_Renderer_DrawPoint(BE_Vector_2I position, BE_Color_4U color) {
    (void) position;
    (void) color;

//  if (BE_Window_GetInternalSDLRenderer() == NULL)
//      return 0;
//
//#ifndef BACON_ENGINE_DISABLE_SDL
//  renderCalls++;
//
//  if (color.a != 255)
//      SDL_SetRenderDrawBlendMode(BE_Window_GetInternalSDLRenderer(), SDL_BLENDMODE_BLEND);
//
//  return SDL_SetRenderDrawColor(BE_Window_GetInternalSDLRenderer(), (Uint8) color.r, (Uint8) color.g, (Uint8) color.b, (Uint8) color.a) == 0 &&
//         SDL_RenderDrawPoint(BE_Window_GetInternalSDLRenderer(), position.x, position.y) == 0;
//#else
//  return 0;
//#endif
}

void BE_Renderer_DrawRectangle(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color) {
    BE_SpecificPlatformFunctions_Get().rendererFunctions.DrawFilledRectangle(position, size, color);
}

void BE_Renderer_FillRectangle(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color) {
    (void) position;
    (void) size;
    (void) color;

//  if (BE_Window_GetInternalSDLRenderer() == NULL)
//      return 0;
//
//#ifndef BACON_ENGINE_DISABLE_SDL
//  SDL_Rect rectangle = {
//      position.x,
//      position.y,
//      (int) size.x,
//      (int) size.y
//  };
//
//  renderCalls++;
//
//  if (color.a != 255)
//      SDL_SetRenderDrawBlendMode(BE_Window_GetInternalSDLRenderer(), SDL_BLENDMODE_BLEND);
//
//  return SDL_SetRenderDrawColor(BE_Window_GetInternalSDLRenderer(), (Uint8) color.r, (Uint8) color.g, (Uint8) color.b, (Uint8) color.a) == 0 &&
//         SDL_RenderFillRect(BE_Window_GetInternalSDLRenderer(), &rectangle) == 0;
//#else
//  return 0;
//#endif
}

void BE_Renderer_DrawBorderedRectangle(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U borderColor, BE_Color_4U fillColor, int borderPadding) {
    (void) borderColor;
    (void) borderPadding; // TODO: Padding.
    BE_Renderer_DrawRectangle(position, size, fillColor);
//  return 0;
//  return BE_Renderer_DrawRectangle((BE_Vector_2I) {position.x - borderPadding, position.y - borderPadding},
//                                   (BE_Vector_2U) {size.x + borderPadding * 2, size.y + borderPadding * 2},
//                                   borderColor) &&
//         BE_Renderer_FillRectangle(position, size, fillColor);
    }
SEC_CPP_SUPPORT_GUARD_END()
