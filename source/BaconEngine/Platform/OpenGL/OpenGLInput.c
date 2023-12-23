// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <GLFW/glfw3.h>

#include "OpenGLInput.h"
#include "OpenGLWindow.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Boolean BE_OpenGLInput_IsKeyDown(BE_Keyboard_KeyCodes keyCode) {
    (void) keyCode;
    return BA_BOOLEAN_FALSE; // TODO
}

BA_Boolean BE_OpenGLInput_IsKeyPressed(BE_Keyboard_KeyCodes keyCode) {
    return glfwGetKey(BE_OpenGLWindow_GetWindow(), keyCode) == GLFW_PRESS;
}

BA_Boolean BE_OpenGLInput_IsMouseDown(BE_Mouse_ButtonType button) {
    return glfwGetMouseButton(BE_OpenGLWindow_GetWindow(), button) == GLFW_PRESS;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
