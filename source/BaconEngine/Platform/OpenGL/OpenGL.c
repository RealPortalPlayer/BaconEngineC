// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <GLFW/glfw3.h>

#include "OpenGL.h"
#include "../SpecificPlatformFunctions.h"
#include "BaconEngine/Debugging/Assert.h"
#include "OpenGLWindow.h"
#include "OpenGLRenderer.h"
#include "OpenGLInput.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
static SEC_Boolean beOpenGLInitialized = SEC_BOOLEAN_FALSE;

void BE_OpenGL_Error(int error, const char* description) {
    SEC_LOGGER_ERROR("GLFW Error:\n"
                     "Error: %i\n"
                     "Description: %s\n", error, description);
}

double BE_OpenGL_GetTimer(void) {
    return glfwGetTime();
}

void BE_OpenGL_Initialize(void) {
    BE_ASSERT(!beOpenGLInitialized, "Already initialized OpenGL\n");
    BE_ASSERT(glfwInit(), "Failed to initialize GLFW\n");
    glfwSetErrorCallback(&BE_OpenGL_Error);
    BE_SpecificPlatformFunctions_Initialize(SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_SpecificPlatformFunctions,
                                                                                {
            &BE_OpenGLWindow_Create,
            &BE_OpenGLWindow_GetTitle,
            &BE_OpenGLWindow_GetSize,
            &BE_OpenGLWindow_GetPosition,
            &BE_OpenGLWindow_IsVisible,
            &BE_OpenGLWindow_IsStillOpened,
            &BE_OpenGLWindow_SetTitle,
            &BE_OpenGLWindow_SetSize,
            &BE_OpenGLWindow_SetPosition,
            &BE_OpenGLWindow_SetVisibility,
            &BE_OpenGLWindow_Close,
            &BE_OpenGLWindow_UpdateEvents
        },
                                                                                {
            &BE_OpenGLRenderer_ClearScreen,
            &BE_OpenGLRenderer_SetClearColor,
            &BE_OpenGLRenderer_GetClearColor,
            &BE_OpenGLRenderer_Render,
            &BE_OpenGLRenderer_DrawFilledRectangle
        },
                                                                                {
            &BE_OpenGLInput_IsKeyDown,
            &BE_OpenGLInput_IsKeyPressed,
            &BE_OpenGLInput_IsMouseDown
        },
                                                                                &BE_OpenGL_Destroy,
                                                                                &BE_OpenGL_GetTimer
    ));

    beOpenGLInitialized = SEC_BOOLEAN_TRUE;
}

void BE_OpenGL_Destroy(void) {
    BE_ASSERT(beOpenGLInitialized, "Already destroyed OpenGL\n");
    glfwTerminate();

    beOpenGLInitialized = SEC_BOOLEAN_FALSE;
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
