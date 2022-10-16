#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <string.h>

#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Rendering/Window.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "PrivateWindow.h"
#include "../Platform/SpecificPlatformFunctions.h"

SEC_CPP_SUPPORT_GUARD_START()
void BE_PrivateWindow_Initialize(const char* windowTitle, BE_Vector_2U windowSize) {
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

const char* BE_Window_GetTitle(void) {
    return BE_SpecificPlatformFunctions_Get().windowFunctions.GetTitle();
}

BE_Vector_2U BE_Window_GetSize(void) {
    return BE_SpecificPlatformFunctions_Get().windowFunctions.GetSize();
}

BE_Vector_2I BE_Window_GetPosition(void) {
    return BE_SpecificPlatformFunctions_Get().windowFunctions.GetPosition();
}

int BE_Window_IsVisible(void) {
    return BE_SpecificPlatformFunctions_Get().windowFunctions.IsVisible();
}

int BE_Window_IsStillOpened(void) {
    return BE_SpecificPlatformFunctions_Get().windowFunctions.IsStillOpened();
}

void BE_Window_SetTitle(const char* newTitle) {
    BE_SpecificPlatformFunctions_Get().windowFunctions.SetTitle(newTitle);
}

void BE_Window_SetSize(BE_Vector_2U newSize) {
    BE_SpecificPlatformFunctions_Get().windowFunctions.SetSize(newSize);
}

void BE_Window_SetWidth(unsigned newWidth) {
	BE_Window_SetSize(SEC_CPP_SUPPORT_CREATE_STRUCT(BE_Vector_2U, newWidth, BE_Window_GetSize().y));
}

void BE_Window_SetHeight(unsigned newHeight) {
    BE_Window_SetSize(SEC_CPP_SUPPORT_CREATE_STRUCT(BE_Vector_2U, BE_Window_GetSize().x, newHeight));
}

void BE_Window_SetPosition(BE_Vector_2I newPosition) {
    BE_SpecificPlatformFunctions_Get().windowFunctions.SetPosition(newPosition);
}

void BE_Window_SetXPosition(int newX) {
    BE_Window_SetPosition(SEC_CPP_SUPPORT_CREATE_STRUCT(BE_Vector_2I, newX, BE_Window_GetPosition().y));
}

void BE_Window_SetYPosition(int newY) {
    BE_Window_SetPosition(SEC_CPP_SUPPORT_CREATE_STRUCT(BE_Vector_2I, BE_Window_GetPosition().x, newY));
}

void BE_Window_SetVisibility(int visible) {
    BE_SpecificPlatformFunctions_Get().windowFunctions.SetVisibility(visible);
}
SEC_CPP_SUPPORT_GUARD_END()
