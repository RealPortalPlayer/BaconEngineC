// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <BaconAPI/Internal/OperatingSystem.h>
#include <BaconAPI/Debugging/Assert.h>

#include "OpenGLWindow.h"
#include "BaconEngine/Input/Keyboard.h"
#include "BaconEngine/Rendering/Layer.h"
#include "OpenGLRenderer.h"
#include "../../Rendering/PrivateLayer.h"

#define BE_OPENGL_THREE_NULL BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL
#define BE_OPENGL_NINE_NULL BE_OPENGL_THREE_NULL, BE_OPENGL_THREE_NULL, BE_OPENGL_THREE_NULL

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static GLFWwindow* window;
static const char* windowTitle;
static const BE_Keyboard_KeyCodes glToEngineCodes[GLFW_KEY_LAST + 1] = {
        BE_OPENGL_NINE_NULL, BE_OPENGL_NINE_NULL, BE_OPENGL_NINE_NULL,
        BE_OPENGL_THREE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
        BE_KEYBOARD_KEY_CODE_SPACE, BE_OPENGL_THREE_NULL, BE_OPENGL_THREE_NULL,
        BE_KEYBOARD_KEY_CODE_APOSTROPHE, BE_OPENGL_THREE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
        BE_KEYBOARD_KEY_CODE_COMMA, BE_KEYBOARD_KEY_CODE_HYPHEN, BE_KEYBOARD_KEY_CODE_PERIOD,
        BE_KEYBOARD_KEY_CODE_SLASH, BE_KEYBOARD_KEY_CODE_ZERO, BE_KEYBOARD_KEY_CODE_ONE,
        BE_KEYBOARD_KEY_CODE_TWO, BE_KEYBOARD_KEY_CODE_THREE, BE_KEYBOARD_KEY_CODE_FOUR,
        BE_KEYBOARD_KEY_CODE_FIVE, BE_KEYBOARD_KEY_CODE_SIX, BE_KEYBOARD_KEY_CODE_SEVEN,
        BE_KEYBOARD_KEY_CODE_EIGHT, BE_KEYBOARD_KEY_CODE_NINE, BE_KEYBOARD_KEY_CODE_NULL,
        BE_KEYBOARD_KEY_CODE_SEMI_COLON, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_EQUAL,
        BE_OPENGL_THREE_NULL, BE_KEYBOARD_KEY_CODE_A, BE_KEYBOARD_KEY_CODE_B,
        BE_KEYBOARD_KEY_CODE_C, BE_KEYBOARD_KEY_CODE_D, BE_KEYBOARD_KEY_CODE_E,
        BE_KEYBOARD_KEY_CODE_F, BE_KEYBOARD_KEY_CODE_G, BE_KEYBOARD_KEY_CODE_H,
        BE_KEYBOARD_KEY_CODE_I, BE_KEYBOARD_KEY_CODE_J, BE_KEYBOARD_KEY_CODE_K,
        BE_KEYBOARD_KEY_CODE_L, BE_KEYBOARD_KEY_CODE_M, BE_KEYBOARD_KEY_CODE_N,
        BE_KEYBOARD_KEY_CODE_O, BE_KEYBOARD_KEY_CODE_P, BE_KEYBOARD_KEY_CODE_Q,
        BE_KEYBOARD_KEY_CODE_R, BE_KEYBOARD_KEY_CODE_S, BE_KEYBOARD_KEY_CODE_T,
        BE_KEYBOARD_KEY_CODE_U, BE_KEYBOARD_KEY_CODE_V, BE_KEYBOARD_KEY_CODE_W,
        BE_KEYBOARD_KEY_CODE_X, BE_KEYBOARD_KEY_CODE_Y, BE_KEYBOARD_KEY_CODE_Z,
        BE_KEYBOARD_KEY_CODE_LBRACKET, BE_KEYBOARD_KEY_CODE_BACKSLASH, BE_KEYBOARD_KEY_CODE_RBRACKET,
        BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_BACKTICK,
        BE_OPENGL_NINE_NULL, BE_OPENGL_NINE_NULL, BE_OPENGL_NINE_NULL,
        BE_OPENGL_NINE_NULL, BE_OPENGL_NINE_NULL, BE_OPENGL_NINE_NULL,
        BE_OPENGL_NINE_NULL, BE_OPENGL_NINE_NULL, BE_OPENGL_NINE_NULL,
        BE_OPENGL_NINE_NULL, BE_OPENGL_NINE_NULL, BE_OPENGL_NINE_NULL,
        BE_OPENGL_NINE_NULL, BE_OPENGL_NINE_NULL, BE_OPENGL_NINE_NULL,
        BE_OPENGL_NINE_NULL, BE_OPENGL_NINE_NULL, BE_OPENGL_THREE_NULL,
        BE_OPENGL_THREE_NULL, BE_KEYBOARD_KEY_CODE_ESCAPE, BE_KEYBOARD_KEY_CODE_RETURN,
        BE_KEYBOARD_KEY_CODE_TAB, BE_KEYBOARD_KEY_CODE_BACKSPACE, BE_KEYBOARD_KEY_CODE_INSERT,
        BE_KEYBOARD_KEY_CODE_DELETE, BE_KEYBOARD_KEY_CODE_RIGHT_ARROW, BE_KEYBOARD_KEY_CODE_LEFT_ARROW,
        BE_KEYBOARD_KEY_CODE_DOWN_ARROW, BE_KEYBOARD_KEY_CODE_UP_ARROW, BE_KEYBOARD_KEY_CODE_PAGE_UP,
        BE_KEYBOARD_KEY_CODE_PAGE_DOWN, BE_KEYBOARD_KEY_CODE_HOME, BE_KEYBOARD_KEY_CODE_END,
        BE_OPENGL_NINE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_CAPS_LOCK,
        BE_KEYBOARD_KEY_CODE_SCROLL_LOCK, BE_KEYBOARD_KEY_CODE_KEYPAD_NUM_LOCK, BE_KEYBOARD_KEY_CODE_PRINT_SCREEN,
        BE_KEYBOARD_KEY_CODE_PAUSE_BREAK, BE_OPENGL_THREE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
        BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_F1, BE_KEYBOARD_KEY_CODE_F2,
        BE_KEYBOARD_KEY_CODE_F3, BE_KEYBOARD_KEY_CODE_F4, BE_KEYBOARD_KEY_CODE_F5,
        BE_KEYBOARD_KEY_CODE_F6, BE_KEYBOARD_KEY_CODE_F7, BE_KEYBOARD_KEY_CODE_F8,
        BE_KEYBOARD_KEY_CODE_F9, BE_KEYBOARD_KEY_CODE_F10, BE_KEYBOARD_KEY_CODE_F11,
        BE_KEYBOARD_KEY_CODE_F12, BE_KEYBOARD_KEY_CODE_F13, BE_KEYBOARD_KEY_CODE_F14,
        BE_KEYBOARD_KEY_CODE_F15, BE_KEYBOARD_KEY_CODE_F16, BE_KEYBOARD_KEY_CODE_F17,
        BE_KEYBOARD_KEY_CODE_F18, BE_KEYBOARD_KEY_CODE_F19, BE_KEYBOARD_KEY_CODE_F20,
        BE_KEYBOARD_KEY_CODE_F21, BE_KEYBOARD_KEY_CODE_F22, BE_KEYBOARD_KEY_CODE_F23,
        BE_KEYBOARD_KEY_CODE_F24, BE_KEYBOARD_KEY_CODE_F25, BE_OPENGL_THREE_NULL,
        BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_KEYPAD_ZERO,
        BE_KEYBOARD_KEY_CODE_KEYPAD_ONE, BE_KEYBOARD_KEY_CODE_KEYPAD_TWO, BE_KEYBOARD_KEY_CODE_KEYPAD_THREE,
        BE_KEYBOARD_KEY_CODE_KEYPAD_FOUR, BE_KEYBOARD_KEY_CODE_KEYPAD_FIVE, BE_KEYBOARD_KEY_CODE_KEYPAD_SIX,
        BE_KEYBOARD_KEY_CODE_KEYPAD_SEVEN, BE_KEYBOARD_KEY_CODE_KEYPAD_EIGHT, BE_KEYBOARD_KEY_CODE_KEYPAD_NINE,
        BE_KEYBOARD_KEY_CODE_KEYPAD_PERIOD, BE_KEYBOARD_KEY_CODE_KEYPAD_DIVIDE, BE_KEYBOARD_KEY_CODE_KEYPAD_MULTIPLY,
        BE_KEYBOARD_KEY_CODE_KEYPAD_MINUS, BE_KEYBOARD_KEY_CODE_KEYPAD_PLUS, BE_KEYBOARD_KEY_CODE_KEYPAD_RETURN,
        BE_OPENGL_THREE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_LSHIFT,
        BE_KEYBOARD_KEY_CODE_LCTRL, BE_KEYBOARD_KEY_CODE_LALT, BE_KEYBOARD_KEY_CODE_LWINDOWS,
        BE_KEYBOARD_KEY_CODE_RSHIFT, BE_KEYBOARD_KEY_CODE_RCTRL, BE_KEYBOARD_KEY_CODE_RALT,
        BE_KEYBOARD_KEY_CODE_RWINDOWS, BE_KEYBOARD_KEY_CODE_NULL
};

void* BE_OpenGLWindow_GetWindow(void) {
    return window;
}

static void BE_OpenGLWindow_KeyEvent(GLFWwindow* theWindow, int key, int scanCode, int action, int mods) {
    (void) theWindow;
    (void) mods;
    (void) scanCode;

    if (key >= sizeof(glToEngineCodes) / sizeof(glToEngineCodes[0]))
        return;

    BE_Keyboard_KeyCodes keyCode = glToEngineCodes[key];

    if (!BE_Keyboard_IsEngineKeyCodeValid(keyCode))
        return;

    BE_PrivateLayer_OnEvent(BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Event,
        .type = action != GLFW_RELEASE ? BE_EVENT_TYPE_KEYBOARD_KEY_DOWN : BE_EVENT_TYPE_KEYBOARD_KEY_UP,
        .data = {
            .keyboard = {
                .key = keyCode
            }
        }
    ));
}

static void BE_OpenGLWindow_MouseButtonEvent(GLFWwindow* theWindow, int button, int action, int mods) {
    (void) theWindow;
    (void) mods;

    double x;
    double y;

    glfwGetCursorPos(window, &x, &y);
    BE_PrivateLayer_OnEvent(BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Event,
        .type = action != GLFW_RELEASE ? BE_EVENT_TYPE_MOUSE_BUTTON_DOWN : BE_EVENT_TYPE_MOUSE_BUTTON_UP,
        .data = {
            .mouse = {
                .position = BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Float, (float) x, (float) y),
                .unionVariables = {
                    .whichButton = button
                }
            }
        }
    ));
}

static void BE_OpenGLWindow_CursorPositionMovedEvent(GLFWwindow* theWindow, double x, double y) {
    (void) theWindow;

    BE_PrivateLayer_OnEvent(BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Event,
        .type = BE_EVENT_TYPE_MOUSE_MOVED,
        .data = {
            .mouse = {
                .position = BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Float, (float) x, (float) y)
            }
        }
    ));
}

static void BE_OpenGLWindow_CursorEnterEvent(GLFWwindow* theWindow, int entered) {
    (void) theWindow;

    BE_PrivateLayer_OnEvent(BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Event,
        .type = entered ? BE_EVENT_TYPE_MOUSE_ENTER : BE_EVENT_TYPE_MOUSE_LEAVE
    ));
}

static void BE_OpenGLWindow_ScrollEvent(GLFWwindow* theWindow, double x, double y) {
    (void) theWindow;

    double xPos;
    double yPos;

    glfwGetCursorPos(window, &xPos, &yPos);
    BE_PrivateLayer_OnEvent(BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Event,
            .type = BE_EVENT_TYPE_MOUSE_WHEEL,
            .data = {
            .mouse = {
                .position = BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Float, (float) xPos, (float) yPos),
                .unionVariables = {
                    .wheelScrollAmount = BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Float, (float) x, (float) y)
                }
            }
        }
    ));
}

static void BE_OpenGLWindow_FocusEvent(GLFWwindow* theWindow, int focused) {
    (void) theWindow;

    BE_PrivateLayer_OnEvent(BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Event,
        .type = focused ? BE_EVENT_TYPE_WINDOW_FOCUS_GAINED : BE_EVENT_TYPE_WINDOW_FOCUS_LOST
    ));
}

static void BE_OpenGLWindow_CloseEvent(GLFWwindow* theWindow) {
    (void) theWindow;

    BE_PrivateLayer_OnEvent(BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Event,
        .type = BE_EVENT_TYPE_WINDOW_CLOSE
    ));
}

static void BE_OpenGLWindow_MaximizeEvent(GLFWwindow* theWindow, int maximized) {
    (void) theWindow;

    BE_PrivateLayer_OnEvent(BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Event,
        .type = maximized ? BE_EVENT_TYPE_WINDOW_MAXIMIZED : BE_EVENT_TYPE_WINDOW_RESTORED
    ));
}

static void BE_OpenGLWindow_MovedEvent(GLFWwindow* theWindow, int x, int y) {
    (void) theWindow;

    BE_PrivateLayer_OnEvent(BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Event,
        .type = BE_EVENT_TYPE_WINDOW_MOVED,
        .data = {
            .window = {
                .unionVariables = {
                    .newPosition = BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Integer, x, y)
                }
            }
        }
    ));
}

static void BE_OpenGLWindow_ResizedEvent(GLFWwindow* theWindow, int width, int height) {
    (void) theWindow;

    BE_PrivateLayer_OnEvent(BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Event,
        .type = BE_EVENT_TYPE_WINDOW_RESIZED,
        .data = {
            .window = {
                .unionVariables = {
                    .newSize = BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Integer, width, height)
                }
            }
        }
    ));
}

static void BE_OpenGLWindow_FramebufferResizedEvent(GLFWwindow* theWindow, int width, int height) {
    glViewport(0, 0, width, height);
}

void BE_OpenGLWindow_Create(const char* title, BE_Vector2_Unsigned size, int monitor) {
    (void) monitor;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if BA_OPERATINGSYSTEM_APPLE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow((int) size.x, (int) size.y, title, NULL, NULL);

    BA_ASSERT(window != NULL, "Failed to create GLFW window\n");
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, &BE_OpenGLWindow_KeyEvent);
    glfwSetMouseButtonCallback(window, &BE_OpenGLWindow_MouseButtonEvent);
    glfwSetCursorPosCallback(window, &BE_OpenGLWindow_CursorPositionMovedEvent);
    glfwSetCursorEnterCallback(window, &BE_OpenGLWindow_CursorEnterEvent);
    glfwSetScrollCallback(window, &BE_OpenGLWindow_ScrollEvent);
    glfwSetWindowFocusCallback(window, &BE_OpenGLWindow_FocusEvent);
    glfwSetWindowCloseCallback(window, &BE_OpenGLWindow_CloseEvent);
    glfwSetWindowMaximizeCallback(window, &BE_OpenGLWindow_MaximizeEvent);
    glfwSetWindowPosCallback(window, &BE_OpenGLWindow_MovedEvent);
    glfwSetWindowSizeCallback(window, &BE_OpenGLWindow_ResizedEvent);
    glfwSetFramebufferSizeCallback(window, &BE_OpenGLWindow_FramebufferResizedEvent);
    BA_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize Glad\n");
    glViewport(0, 0, (int) size.x, (int) size.y);
    BE_OpenGLRenderer_CompileShaders();

    windowTitle = title;
}

const char* BE_OpenGLWindow_GetTitle(void) {
    return windowTitle;
}

BE_Vector2_Unsigned BE_OpenGLWindow_GetSize(void) {
    int width;
    int height;

    glfwGetWindowSize(window, &width, &height);

    return BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, (unsigned) width, (unsigned) height);
}

BE_Vector2_Integer BE_OpenGLWindow_GetPosition(void) {
    int x;
    int y;

    glfwGetWindowPos(window, &x, &y);

    return BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Integer, x, y);
}

int BE_OpenGLWindow_IsVisible(void) {
    return glfwGetWindowAttrib(window, GLFW_VISIBLE);
}

int BE_OpenGLWindow_IsStillOpened(void) {
    return window != NULL && !glfwWindowShouldClose(window);
}

void BE_OpenGLWindow_SetTitle(const char* title) {
    glfwSetWindowTitle(window, title);

    windowTitle = title;
}

void BE_OpenGLWindow_SetSize(BE_Vector2_Unsigned newSize) {
    glfwSetWindowSize(window, (int) newSize.x, (int) newSize.y);
}

void BE_OpenGLWindow_SetPosition(BE_Vector2_Integer newPosition) {
    glfwSetWindowPos(window, newPosition.x, newPosition.y);
}

void BE_OpenGLWindow_SetVisibility(int visible) {
    if (visible)
        glfwShowWindow(window);
    else
        glfwHideWindow(window);
}

void BE_OpenGLWindow_Close(void) {
    if (window == NULL)
        return;

    glfwDestroyWindow(window);

    window = NULL;
}

void BE_OpenGLWindow_UpdateEvents(void) {
    glfwPollEvents();
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
