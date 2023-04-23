// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/Internal/OperatingSystem.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <SharedEngineCode/StringExtension.h>

#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Rendering/Renderer.h"

#ifndef BE_CLIENT_BINARY
#   include "../Platform/SpecificPlatformFunctions.h"
#   include "../Platform/TextMode/TextMode.h"
#   ifndef BE_DISABLE_OPENGL
#      include "../Platform/OpenGL/OpenGL.h"
#   endif
#   if SEC_OPERATINGSYSTEM_WINDOWS
#      include "../Platform/Windows/Windows.h"
#   elif SEC_OPERATINGSYSTEM_APPLE && !defined(BE_DISABLE_METAL)
#      include "../Platform/Metal/Metal.h"
#   endif
#endif

#include "BaconEngine/Debugging/Assert.h"
#include "../InterfaceFunctions.h"

SEC_CPP_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static BE_Renderer_Types beRendererCurrent;
static int renderCalls = 0;
#endif

void BE_Renderer_SetClearColor(BE_Color_3U color) {
#ifndef BE_CLIENT_BINARY
    BE_SpecificPlatformFunctions_Get().rendererFunctions.SetClearColor(color);
#else
    BE_INTERFACEFUNCTION(void, BE_Color_3U)(color);
#endif
}

#ifndef BE_CLIENT_BINARY
void BE_PrivateRenderer_Initialize(void) {
    static SEC_Boolean initialized = SEC_FALSE;

    BE_STRICTMODE_CHECK_NO_RETURN_VALUE(!initialized, "Already initialized rendererFunctions\n");

    const char* specifiedRenderer = SEC_ArgumentHandler_GetValue(SEC_BUILTINARGUMENTS_RENDERER, 0);

    if (BE_ClientInformation_IsServerModeEnabled() || (specifiedRenderer != NULL && SEC_StringExtension_CompareCaseless(specifiedRenderer, "text") == 0)) {
        textmode:
        SEC_LOGGER_INFO("Using no renderer\n");
        BE_TextMode_Initialize();

        beRendererCurrent = BE_RENDERER_TYPE_TEXT;
        return;
    }

    if (specifiedRenderer != NULL) {
        if (SEC_StringExtension_CompareCaseless(specifiedRenderer, "opengl") == 0) {
#   ifndef BE_DISABLE_OPENGL
//            opengl:
            SEC_LOGGER_INFO("Using OpenGL as the renderer\n");
            BE_OpenGL_Initialize();

            beRendererCurrent = BE_RENDERER_TYPE_OPENGL;
#   else
            SEC_LOGGER_WARN("This binary has OpenGL disabled\n");
            goto textmode;
#   endif
            return;
        }

        if (SEC_StringExtension_CompareCaseless(specifiedRenderer, "vulkan") == 0) {
#   if !SEC_OPERATINGSYSTEM_APPLE
//            vulkan:
            BE_ASSERT_ALWAYS("Renderer not implemented\n");
#   else
            SEC_LOGGER_WARN("Vulkan doesn't support Apple operating systems\n");
            goto textmode;
#   endif
        }

        if (SEC_StringExtension_CompareCaseless(specifiedRenderer, "metal") == 0) {
#   if SEC_OPERATINGSYSTEM_APPLE
#       ifndef BE_DISABLE_METAL
//            metal:
            SEC_LOGGER_INFO("Using Metal as the renderer");
            BE_Metal_Initialize();
#       else
            SEC_LOGGER_WARN("This binary has Metal disabled\n");
            goto textmode;
#       endif
#   else
            SEC_LOGGER_WARN("Metal only works on Apple operating systems\n");
            goto textmode;
#   endif

            return;
        }

        if (SEC_StringExtension_CompareCaseless(specifiedRenderer, "directx") == 0) {
#   if SEC_OPERATINGSYSTEM_WINDOWS
//            directx:
            BE_ASSERT_ALWAYS("Renderer not implemented\n");
#   else
            SEC_LOGGER_WARN("DirectX only works on Microsoft operating systems\n");
            goto textmode;
#   endif
            return;
        }

        if (SEC_StringExtension_CompareCaseless(specifiedRenderer, "software") == 0) {
            SEC_LOGGER_INFO("Using software rendering\n");
            SEC_LOGGER_WARN("This is going to lag; use a better renderer\n");

#   if SEC_OPERATINGSYSTEM_WINDOWS
            BE_Windows_Initialize();
#   else
            BE_ASSERT_ALWAYS("No software renderer implementation for your OS\n");
#   endif
            return;
        }

        SEC_LOGGER_WARN("Invalid renderer: %s\n", specifiedRenderer);
    }

    // TODO: Automatically select renderer
    goto textmode;
}
#endif

void BE_Renderer_ClearScreen(void) {
#ifndef BE_CLIENT_BINARY
    BE_SpecificPlatformFunctions_Get().rendererFunctions.ClearScreen();
#else
    BE_INTERFACEFUNCTION(void, void)();
#endif
}

int BE_Renderer_GetCalls(void) {
#ifndef BE_CLIENT_BINARY
    return renderCalls;
#else
    BE_INTERFACEFUNCTION(int, void);
    return function();
#endif
}

BE_Renderer_Types BE_Renderer_GetCurrentType(void) {
#ifndef BE_CLIENT_BINARY
    return beRendererCurrent;
#else
    BE_INTERFACEFUNCTION(BE_Renderer_Types, void);
    return function();
#endif
}

BE_Color_3U BE_Renderer_GetClearColor(void) {
#ifndef BE_CLIENT_BINARY
    return BE_SpecificPlatformFunctions_Get().rendererFunctions.GetClearColor();
#else
    BE_INTERFACEFUNCTION(BE_Color_3U, void);
    return function();
#endif
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
#ifndef BE_CLIENT_BINARY
    return SEC_CPP_SUPPORT_CREATE_STRUCT(BE_Vector_2I,
        rectanglePosition.x + (int) rectangleSize.x / 2 - (int) objectSize.x / 2,
        rectanglePosition.y + (int) rectangleSize.y / 2 - (int) objectSize.y / 2
    );
#else
    BE_INTERFACEFUNCTION(BE_Vector_2I, BE_Vector_2I, BE_Vector_2U, BE_Vector_2U);
    return function(rectanglePosition, rectangleSize, objectSize);
#endif
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
#ifndef BE_CLIENT_BINARY
    BE_SpecificPlatformFunctions_Get().rendererFunctions.DrawFilledRectangle(position, size, color);
#else
    BE_INTERFACEFUNCTION(void, BE_Vector_2I, BE_Vector_2U, BE_Color_4U)(position, size, color);
#endif
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
    (void) position;
    (void) size;
    (void) borderColor;
    (void) fillColor;
    (void) borderPadding; // TODO: Padding.
//    BE_Renderer_DrawRectangle(position, size, fillColor);
//  return 0;
//  return BE_Renderer_DrawRectangle((BE_Vector_2I) {position.x - borderPadding, position.y - borderPadding},
//                                   (BE_Vector_2U) {size.x + borderPadding * 2, size.y + borderPadding * 2},
//                                   borderColor) &&
//         BE_Renderer_FillRectangle(position, size, fillColor);
    }
SEC_CPP_SUPPORT_GUARD_END()
