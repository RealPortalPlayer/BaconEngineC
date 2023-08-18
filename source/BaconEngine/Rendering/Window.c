// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <string.h>
#include <SharedEngineCode/Debugging/Assert.h>

#include "BaconEngine/Rendering/Window.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "../InterfaceFunctions.h"

#ifndef BE_CLIENT_BINARY
#   include "PrivateWindow.h"
#   include "../Platform/SpecificPlatformFunctions.h"
#endif

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
void BE_PrivateWindow_Initialize(const char* windowTitle, BE_Vector2_Unsigned windowSize) {
    int monitorNumber = 0;
    {
        const char* preParsedMonitor = SEC_ArgumentHandler_GetValue(SEC_BUILTINARGUMENTS_MONITOR, 0);

        if (preParsedMonitor != NULL) {
            char* error;
            int parsedMonitor = (int) strtol(preParsedMonitor, &error, 0);

            if (error != NULL && strlen(error) != 0) {
                SEC_LOGGER_ERROR("Invalid display was supplied, ignoring...\n");

                parsedMonitor = 0;
            }

            monitorNumber = parsedMonitor;
        }
    }

    SEC_LOGGER_INFO("Creating window\nTitle: %s\nSize: (%u, %u)\nMonitor: %i\n", windowTitle, windowSize.x, windowSize.y, monitorNumber);
    BE_SpecificPlatformFunctions_Get().windowFunctions.CreateWindow(windowTitle, windowSize, monitorNumber);
}

void BE_PrivateWindow_Destroy(void) {
    BE_SpecificPlatformFunctions_Get().windowFunctions.CloseWindow();
}
#endif

const char* BE_Window_GetTitle(void) {
#ifndef BE_CLIENT_BINARY
    return BE_SpecificPlatformFunctions_Get().windowFunctions.GetTitle();
#else
    BE_INTERFACEFUNCTION(const char*, void);
    return function();
#endif
}

BE_Vector2_Unsigned BE_Window_GetSize(void) {
#ifndef BE_CLIENT_BINARY
    return BE_SpecificPlatformFunctions_Get().windowFunctions.GetSize();
#else
    BE_INTERFACEFUNCTION(BE_Vector2_Unsigned, void);
    return function();
#endif
}

BE_Vector2_Integer BE_Window_GetPosition(void) {
#ifndef BE_CLIENT_BINARY
    return BE_SpecificPlatformFunctions_Get().windowFunctions.GetPosition();
#else
    BE_INTERFACEFUNCTION(BE_Vector2_Integer, void);
    return function();
#endif
}

SEC_Boolean BE_Window_IsVisible(void) {
#ifndef BE_CLIENT_BINARY
    return BE_SpecificPlatformFunctions_Get().windowFunctions.IsVisible();
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, void);
    return function();
#endif
}

SEC_Boolean BE_Window_IsStillOpened(void) {
#ifndef BE_CLIENT_BINARY
    return BE_SpecificPlatformFunctions_Get().windowFunctions.IsStillOpened();
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, void);
    return function();
#endif
}

void BE_Window_SetTitle(const char* newTitle) {
#ifndef BE_CLIENT_BINARY
    BE_SpecificPlatformFunctions_Get().windowFunctions.SetTitle(newTitle);
#else
    BE_INTERFACEFUNCTION(void, const char*)(newTitle);
#endif
}

void BE_Window_SetSize(BE_Vector2_Unsigned newSize) {
#ifndef BE_CLIENT_BINARY
    BE_SpecificPlatformFunctions_Get().windowFunctions.SetSize(newSize);
#else
    BE_INTERFACEFUNCTION(void, BE_Vector2_Unsigned)(newSize);
#endif
}

void BE_Window_SetWidth(unsigned newWidth) {
#ifndef BE_CLIENT_BINARY
    BE_Window_SetSize(SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, newWidth, BE_Window_GetSize().y));
#else
    BE_INTERFACEFUNCTION(void, unsigned)(newWidth);
#endif
}

void BE_Window_SetHeight(unsigned newHeight) {
#ifndef BE_CLIENT_BINARY
    BE_Window_SetSize(SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, BE_Window_GetSize().x, newHeight));
#else
    BE_INTERFACEFUNCTION(void, unsigned)(newHeight);
#endif
}

void BE_Window_SetPosition(BE_Vector2_Integer newPosition) {
#ifndef BE_CLIENT_BINARY
    BE_SpecificPlatformFunctions_Get().windowFunctions.SetPosition(newPosition);
#else
    BE_INTERFACEFUNCTION(void, BE_Vector2_Integer)(newPosition);
#endif
}

void BE_Window_SetXPosition(int newX) {
#ifndef BE_CLIENT_BINARY
    BE_Window_SetPosition(SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Integer, newX, BE_Window_GetPosition().y));
#else
    BE_INTERFACEFUNCTION(void, int)(newX);
#endif
}

void BE_Window_SetYPosition(int newY) {
#ifndef BE_CLIENT_BINARY
    BE_Window_SetPosition(SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Integer, BE_Window_GetPosition().x, newY));
#else
    BE_INTERFACEFUNCTION(void, int)(newY);
#endif
}

void BE_Window_SetVisibility(SEC_Boolean visible) {
#ifndef BE_CLIENT_BINARY
    BE_SpecificPlatformFunctions_Get().windowFunctions.SetVisibility(visible);
#else
    BE_INTERFACEFUNCTION(void, SEC_Boolean)(visible);
#endif
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
