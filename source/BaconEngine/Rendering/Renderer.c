// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/Internal/OperatingSystem.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <SharedEngineCode/Debugging/StrictMode.h>
#include <BaconAPI/Debugging/Assert.h>

#include "BaconEngine/Rendering/Renderer.h"

#ifndef BE_CLIENT_BINARY
#   include "../Platform/SpecificPlatformFunctions.h"
#   include "../Platform/TextMode/TextMode.h"
#   ifndef BE_DISABLE_OPENGL
#      include "../Platform/OpenGL/OpenGL.h"
#   endif
#   if BA_OPERATINGSYSTEM_WINDOWS
#      include "../Platform/Windows/Windows.h"
#   elif BA_OPERATINGSYSTEM_APPLE && !defined(BE_DISABLE_METAL)
#      include "../Platform/Metal/Metal.h"
#   endif
#endif

#include "../InterfaceFunctions.h"
#include "BaconEngine/Client/Information.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static BE_Renderer_Types beRendererCurrent;
static int renderCalls = 0;
#endif

void BE_Renderer_SetClearColor(BE_Color3_Unsigned color) {
#ifndef BE_CLIENT_BINARY
    BE_SpecificPlatformFunctions_Get().rendererFunctions.SetClearColor(color);
#else
    BE_INTERFACEFUNCTION(void, BE_Color3_Unsigned)(color);
#endif
}

#ifndef BE_CLIENT_BINARY
void BE_PrivateRenderer_Initialize(void) {
    static BA_Boolean initialized = BA_BOOLEAN_FALSE;

    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(!initialized, "Already initialized rendererFunctions\n");

    const char* specifiedRenderer = BA_ArgumentHandler_GetValue(SEC_BUILTINARGUMENTS_RENDERER, 0);
    BE_Renderer_Types temporaryRenderer = BE_RENDERER_TYPE_TEXT;

    if (BE_ClientInformation_IsServerModeEnabled() || (specifiedRenderer != NULL && BA_String_Equals(specifiedRenderer, "text", BA_BOOLEAN_TRUE)))
        goto textmode;
    
    if (specifiedRenderer != NULL) {
        if (BA_String_Equals(specifiedRenderer, "opengl", BA_BOOLEAN_TRUE))
            temporaryRenderer = BE_RENDERER_TYPE_OPENGL;
        else if (BA_String_Equals(specifiedRenderer, "vulkan", BA_BOOLEAN_TRUE))
            temporaryRenderer = BE_RENDERER_TYPE_VULKAN;
        else if (BA_String_Equals(specifiedRenderer, "metal", BA_BOOLEAN_TRUE))
            temporaryRenderer = BE_RENDERER_TYPE_METAL;
        else if (BA_String_Equals(specifiedRenderer, "directx", BA_BOOLEAN_TRUE))
             temporaryRenderer = BE_RENDERER_TYPE_DIRECTX;
        else if (BA_String_Equals(specifiedRenderer, "software", BA_BOOLEAN_TRUE))
            temporaryRenderer = BE_RENDERER_TYPE_SOFTWARE;
        else {
            BA_LOGGER_WARN("Invalid renderer: %s\n", specifiedRenderer);
            goto defaults;
        }
    } else {
        defaults:
#   if BA_OPERATINGSYSTEM_LINUX || BA_OPERATINGSYSTEM_UNIX
#       if !defined(BE_DISABLE_VULKAN)
        temporaryRenderer = BE_RENDERER_TYPE_VULKAN;
#       elif !defined(BE_DISABLE_OPENGL)
        temporaryRenderer = BE_RENDERER_TYPE_OPENGL;
#       else
        temporaryRenderer = BE_RENDERER_TYPE_TEXT;
#       endif
#   elif BA_OPERATINGSYSTEM_APPLE
#       if !defined(BE_DISABLE_METAL)
        temporaryRenderer = BE_RENDERER_TYPE_METAL;
#       elif !defined(BE_BE_DISABLE_OPENGL)
        temporaryRenderer = BE_RENDERER_TYPE_METAL;
#       else
        temporaryRenderer = BE_RENDERER_TYPE_TEXT;
#       endif
#   elif BA_OPERATINGSYSTEM_WINDOWS
#       if !defined(BE_DISABLE_DIRECTX)
        temporaryRenderer = BE_RENDERER_TYPE_DIRECTX;
#       else
        temporaryRenderer = BE_RENDERER_TYPE_TEXT;
#       endif
#   else
        temporaryRenderer = BE_RENDERER_TYPE_TEXT;
#   endif
    }

    switch (temporaryRenderer) {
        case BE_RENDERER_TYPE_OPENGL:
#ifndef BE_DISABLE_OPENGL
            BA_LOGGER_INFO("Using OpenGL as the renderer\n");
            BE_OpenGL_Initialize();
            break;
#else
            BA_LOGGER_WARN("This binary has OpenGL disabled\n");
            goto textmode;
#endif

        case BE_RENDERER_TYPE_VULKAN:
#if !BA_OPERATINGSYSTEM_APPLE
            BA_ASSERT_ALWAYS("Vulkan not implemented\n");
#else
            BA_LOGGER_WARN("Vulkan doesn't support Apple operating systems\n");
            goto textmode;
#endif

        case BE_RENDERER_TYPE_METAL:
#if BA_OPERATINGSYSTEM_APPLE
#   ifndef BBE_DISABLE_METAL
            BA_LOGGER_INFO("Using Metal as the renderer");
            BE_Metal_Initialize();
            break;
#   else
            BA_LOGGER_WARN("This binary has Metal disabled\n");
            goto textmode;
#   endif
#else
            BA_LOGGER_WARN("Metal only works on Apple operating systems\n");
            goto textmode;
#endif

        case BE_RENDERER_TYPE_DIRECTX:
#if BA_OPERATINGSYSTEM_WINDOWS
            BA_ASSERT_ALWAYS("DirectX not implemented\n");
#else
            BA_LOGGER_WARN("DirectX only works on Microsoft operating systems\n");
            goto textmode;
#endif

        case BE_RENDERER_TYPE_SOFTWARE:
            BA_LOGGER_INFO("Using software rendering\n");
            BA_LOGGER_WARN("This is going to lag; use a better renderer\n");
#if BA_OPERATINGSYSTEM_WINDOWS
            BE_Windows_Initialize();
            break;
#else
            BA_ASSERT_ALWAYS("No software renderer implementation for your OS\n");
#endif

        case BE_RENDERER_TYPE_TEXT:
        textmode:
            BA_LOGGER_INFO("Using no renderer\n");
            BE_TextMode_Initialize();
            
            temporaryRenderer = BE_RENDERER_TYPE_TEXT;
            break;
    }
    
    beRendererCurrent = temporaryRenderer;
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

BE_Color3_Unsigned BE_Renderer_GetClearColor(void) {
#ifndef BE_CLIENT_BINARY
    return BE_SpecificPlatformFunctions_Get().rendererFunctions.GetClearColor();
#else
    BE_INTERFACEFUNCTION(BE_Color3_Unsigned, void);
    return function();
#endif
}

//BE_Vector2_Unsigned BE_Renderer_GetFontSize(TTF_Font* font, const char* text) {
//  (void) font;
//  (void) text;
//
//  BE_Vector2_Unsigned size = (BE_Vector2_Unsigned) {5, 5};
//
//#ifndef BACON_ENGINE_DISABLE_SDL_TTF
//  if (font != NULL) {
//      BE_Vector2_Unsigned textSize = {0, 0};
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

BE_Vector2_Integer BE_Renderer_GetCenterPosition(BE_Vector2_Integer rectanglePosition, BE_Vector2_Unsigned rectangleSize, BE_Vector2_Unsigned objectSize) {
#ifndef BE_CLIENT_BINARY
    return BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Integer,
        rectanglePosition.x + (int) rectangleSize.x / 2 - (int) objectSize.x / 2,
        rectanglePosition.y + (int) rectangleSize.y / 2 - (int) objectSize.y / 2
    );
#else
    BE_INTERFACEFUNCTION(BE_Vector2_Integer, BE_Vector2_Integer, BE_Vector2_Unsigned, BE_Vector2_Unsigned);
    return function(rectanglePosition, rectangleSize, objectSize);
#endif
}

void BE_Renderer_DrawLine(BE_Vector2_Integer firstPoint, BE_Vector2_Integer secondPoint, BE_Color4_Unsigned color) {
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

void BE_Renderer_DrawPoint(BE_Vector2_Integer position, BE_Color4_Unsigned color) {
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

void BE_Renderer_DrawRectangle(BE_Vector2_Integer position, BE_Vector2_Unsigned size, BE_Color4_Unsigned color) {
#ifndef BE_CLIENT_BINARY
    BE_SpecificPlatformFunctions_Get().rendererFunctions.DrawFilledRectangle(position, size, color);
#else
    BE_INTERFACEFUNCTION(void, BE_Vector2_Integer, BE_Vector2_Unsigned, BE_Color4_Unsigned)(position, size, color);
#endif
}

void BE_Renderer_FillRectangle(BE_Vector2_Integer position, BE_Vector2_Unsigned size, BE_Color4_Unsigned color) {
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

void BE_Renderer_DrawBorderedRectangle(BE_Vector2_Integer position, BE_Vector2_Unsigned size, BE_Color4_Unsigned borderColor, BE_Color4_Unsigned fillColor, int borderPadding) {
    (void) position;
    (void) size;
    (void) borderColor;
    (void) fillColor;
    (void) borderPadding; // TODO: Padding.
//    BE_Renderer_DrawRectangle(position, size, fillColor);
//  return 0;
//  return BE_Renderer_DrawRectangle((BE_Vector2_Integer) {position.x - borderPadding, position.y - borderPadding},
//                                   (BE_Vector2_Unsigned) {size.x + borderPadding * 2, size.y + borderPadding * 2},
//                                   borderColor) &&
//         BE_Renderer_FillRectangle(position, size, fillColor);
    }
BA_CPLUSPLUS_SUPPORT_GUARD_END()
