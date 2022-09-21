#include <SharedEngineCode/Internal/CppHeader.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "OpenGLWindow.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Input/Keyboard.h"
#include "BaconEngine/Rendering/Layer.h"
#include "OpenGLRenderer.h"

SEC_CPP_GUARD_START()
    GLFWwindow* window;
    const char* windowTitle;
    // This is retarded.
    const BE_Keyboard_KeyCodes glToEngineCodes[GLFW_KEY_LAST + 1] = {
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_SPACE,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_APOSTROPHE, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_COMMA,
            BE_KEYBOARD_KEY_CODE_HYPHEN, BE_KEYBOARD_KEY_CODE_PERIOD, BE_KEYBOARD_KEY_CODE_SLASH,
            BE_KEYBOARD_KEY_CODE_ZERO, BE_KEYBOARD_KEY_CODE_ONE, BE_KEYBOARD_KEY_CODE_TWO,
            BE_KEYBOARD_KEY_CODE_THREE, BE_KEYBOARD_KEY_CODE_FOUR, BE_KEYBOARD_KEY_CODE_FIVE,
            BE_KEYBOARD_KEY_CODE_SIX, BE_KEYBOARD_KEY_CODE_SEVEN, BE_KEYBOARD_KEY_CODE_EIGHT,
            BE_KEYBOARD_KEY_CODE_NINE, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_SEMI_COLON,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_EQUAL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_A,
            BE_KEYBOARD_KEY_CODE_B, BE_KEYBOARD_KEY_CODE_C, BE_KEYBOARD_KEY_CODE_D,
            BE_KEYBOARD_KEY_CODE_E, BE_KEYBOARD_KEY_CODE_F, BE_KEYBOARD_KEY_CODE_G,
            BE_KEYBOARD_KEY_CODE_H, BE_KEYBOARD_KEY_CODE_I, BE_KEYBOARD_KEY_CODE_J,
            BE_KEYBOARD_KEY_CODE_K, BE_KEYBOARD_KEY_CODE_L, BE_KEYBOARD_KEY_CODE_M,
            BE_KEYBOARD_KEY_CODE_N, BE_KEYBOARD_KEY_CODE_O, BE_KEYBOARD_KEY_CODE_P,
            BE_KEYBOARD_KEY_CODE_Q, BE_KEYBOARD_KEY_CODE_R, BE_KEYBOARD_KEY_CODE_S,
            BE_KEYBOARD_KEY_CODE_T, BE_KEYBOARD_KEY_CODE_U, BE_KEYBOARD_KEY_CODE_V,
            BE_KEYBOARD_KEY_CODE_W, BE_KEYBOARD_KEY_CODE_X, BE_KEYBOARD_KEY_CODE_Y,
            BE_KEYBOARD_KEY_CODE_Z, BE_KEYBOARD_KEY_CODE_LBRACKET, BE_KEYBOARD_KEY_CODE_BACKSLASH,
            BE_KEYBOARD_KEY_CODE_RBRACKET, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_BACKTICK, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_ESCAPE, BE_KEYBOARD_KEY_CODE_RETURN,
            BE_KEYBOARD_KEY_CODE_TAB, BE_KEYBOARD_KEY_CODE_BACKSPACE, BE_KEYBOARD_KEY_CODE_INSERT,
            BE_KEYBOARD_KEY_CODE_DELETE, BE_KEYBOARD_KEY_CODE_RIGHT_ARROW, BE_KEYBOARD_KEY_CODE_LEFT_ARROW,
            BE_KEYBOARD_KEY_CODE_DOWN_ARROW, BE_KEYBOARD_KEY_CODE_UP_ARROW, BE_KEYBOARD_KEY_CODE_PAGE_UP,
            BE_KEYBOARD_KEY_CODE_PAGE_DOWN, BE_KEYBOARD_KEY_CODE_HOME, BE_KEYBOARD_KEY_CODE_END,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_CAPS_LOCK, BE_KEYBOARD_KEY_CODE_SCROLL_LOCK,
            BE_KEYBOARD_KEY_CODE_KP_NUM_LOCK, BE_KEYBOARD_KEY_CODE_PRINT_SCREEN, BE_KEYBOARD_KEY_CODE_PAUSE_BREAK,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_F1,
            BE_KEYBOARD_KEY_CODE_F2, BE_KEYBOARD_KEY_CODE_F3, BE_KEYBOARD_KEY_CODE_F4,
            BE_KEYBOARD_KEY_CODE_F5, BE_KEYBOARD_KEY_CODE_F6, BE_KEYBOARD_KEY_CODE_F7,
            BE_KEYBOARD_KEY_CODE_F8, BE_KEYBOARD_KEY_CODE_F9, BE_KEYBOARD_KEY_CODE_F10,
            BE_KEYBOARD_KEY_CODE_F11, BE_KEYBOARD_KEY_CODE_F12, BE_KEYBOARD_KEY_CODE_F13,
            BE_KEYBOARD_KEY_CODE_F14, BE_KEYBOARD_KEY_CODE_F15, BE_KEYBOARD_KEY_CODE_F16,
            BE_KEYBOARD_KEY_CODE_F17, BE_KEYBOARD_KEY_CODE_F18, BE_KEYBOARD_KEY_CODE_F19,
            BE_KEYBOARD_KEY_CODE_F20, BE_KEYBOARD_KEY_CODE_F21, BE_KEYBOARD_KEY_CODE_F22,
            BE_KEYBOARD_KEY_CODE_F23, BE_KEYBOARD_KEY_CODE_F24, BE_KEYBOARD_KEY_CODE_F25,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_KP_ZERO,
            BE_KEYBOARD_KEY_CODE_KP_ONE, BE_KEYBOARD_KEY_CODE_KP_TWO, BE_KEYBOARD_KEY_CODE_KP_THREE,
            BE_KEYBOARD_KEY_CODE_KP_FOUR, BE_KEYBOARD_KEY_CODE_KP_FIVE, BE_KEYBOARD_KEY_CODE_KP_SIX,
            BE_KEYBOARD_KEY_CODE_KP_SEVEN, BE_KEYBOARD_KEY_CODE_KP_EIGHT, BE_KEYBOARD_KEY_CODE_KP_NINE,
            BE_KEYBOARD_KEY_CODE_KP_PERIOD, BE_KEYBOARD_KEY_CODE_KP_DIVIDE, BE_KEYBOARD_KEY_CODE_KP_MULTIPLY,
            BE_KEYBOARD_KEY_CODE_KP_MINUS, BE_KEYBOARD_KEY_CODE_KP_PLUS, BE_KEYBOARD_KEY_CODE_KP_RETURN,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_NULL,
            BE_KEYBOARD_KEY_CODE_NULL, BE_KEYBOARD_KEY_CODE_LSHIFT, BE_KEYBOARD_KEY_CODE_LCTRL,
            BE_KEYBOARD_KEY_CODE_LALT, BE_KEYBOARD_KEY_CODE_LWINDOWS, BE_KEYBOARD_KEY_CODE_RSHIFT,
            BE_KEYBOARD_KEY_CODE_RCTRL, BE_KEYBOARD_KEY_CODE_RALT, BE_KEYBOARD_KEY_CODE_RWINDOWS,
            BE_KEYBOARD_KEY_CODE_NULL
    };

    const int engineToGLCodes[BE_KEYBOARD_KEY_CODE_SIZE] = {
            GLFW_KEY_UNKNOWN, GLFW_KEY_ESCAPE, GLFW_KEY_F1,
            GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4,
            GLFW_KEY_F5, GLFW_KEY_F6, GLFW_KEY_F7,
            GLFW_KEY_F8, GLFW_KEY_F9, GLFW_KEY_F10,
            GLFW_KEY_F11, GLFW_KEY_F12, GLFW_KEY_F13,
            GLFW_KEY_F14, GLFW_KEY_F15, GLFW_KEY_F16,
            GLFW_KEY_F17, GLFW_KEY_F18, GLFW_KEY_F19,
            GLFW_KEY_F20, GLFW_KEY_F21, GLFW_KEY_F22,
            GLFW_KEY_F23, GLFW_KEY_F24, GLFW_KEY_F25,
            GLFW_KEY_PRINT_SCREEN, GLFW_KEY_SCROLL_LOCK, GLFW_KEY_PAUSE,
            GLFW_KEY_GRAVE_ACCENT, GLFW_KEY_1, GLFW_KEY_2,
            GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5,
            GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8,
            GLFW_KEY_9, GLFW_KEY_0, GLFW_KEY_MINUS,
            GLFW_KEY_EQUAL, GLFW_KEY_BACKSPACE, GLFW_KEY_INSERT,
            GLFW_KEY_HOME, GLFW_KEY_PAGE_UP, GLFW_KEY_NUM_LOCK,
            GLFW_KEY_KP_DIVIDE, GLFW_KEY_KP_MULTIPLY, GLFW_KEY_KP_SUBTRACT,
            GLFW_KEY_TAB, GLFW_KEY_Q, GLFW_KEY_W,
            GLFW_KEY_E, GLFW_KEY_R, GLFW_KEY_T,
            GLFW_KEY_Y, GLFW_KEY_U, GLFW_KEY_I,
            GLFW_KEY_O, GLFW_KEY_P, GLFW_KEY_LEFT_BRACKET,
            GLFW_KEY_RIGHT_BRACKET, GLFW_KEY_BACKSLASH, GLFW_KEY_DELETE,
            GLFW_KEY_END, GLFW_KEY_PAGE_DOWN, GLFW_KEY_KP_7,
            GLFW_KEY_KP_8, GLFW_KEY_KP_9, GLFW_KEY_KP_ADD,
            GLFW_KEY_CAPS_LOCK, GLFW_KEY_A, GLFW_KEY_S,
            GLFW_KEY_D, GLFW_KEY_F, GLFW_KEY_G,
            GLFW_KEY_H, GLFW_KEY_J, GLFW_KEY_K,
            GLFW_KEY_L, GLFW_KEY_SEMICOLON, GLFW_KEY_APOSTROPHE,
            GLFW_KEY_ENTER, GLFW_KEY_KP_4, GLFW_KEY_KP_5,
            GLFW_KEY_KP_6, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_Z,
            GLFW_KEY_X, GLFW_KEY_C, GLFW_KEY_V,
            GLFW_KEY_B, GLFW_KEY_N, GLFW_KEY_M,
            GLFW_KEY_COMMA, GLFW_KEY_PERIOD, GLFW_KEY_SLASH,
            GLFW_KEY_RIGHT_SHIFT, GLFW_KEY_UP, GLFW_KEY_KP_1,
            GLFW_KEY_KP_2, GLFW_KEY_KP_3, GLFW_KEY_KP_ENTER,
            GLFW_KEY_LEFT_CONTROL, GLFW_KEY_LEFT_SUPER, GLFW_KEY_LEFT_ALT,
            GLFW_KEY_SPACE, GLFW_KEY_RIGHT_ALT, GLFW_KEY_RIGHT_SUPER,
            GLFW_KEY_RIGHT_CONTROL, GLFW_KEY_LEFT, GLFW_KEY_DOWN,
            GLFW_KEY_RIGHT, GLFW_KEY_KP_0, GLFW_KEY_KP_DECIMAL
    };

    void* BE_OpenGLWindow_GetInternal(void) {
        return window;
    }

    void BE_OpenGLWindow_KeyEvent(GLFWwindow* theWindow, int key, int scanCode, int action, int mods) {
        (void) theWindow;
        (void) mods;

        if (key >= sizeof(glToEngineCodes))
            return;

        BE_Keyboard_KeyCodes keyCode = glToEngineCodes[key];

        if (!BE_Keyboard_IsEngineKeyCodeValid(keyCode))
            return;

        BE_Keyboard_SetKeyDown(scanCode, action != GLFW_RELEASE);
        BE_Keyboard_SetKeyPressed(scanCode, action != GLFW_RELEASE);
        BE_Layer_OnEvent((BE_Event) {
            .type = action != GLFW_RELEASE ? BE_EVENT_TYPE_KEYBOARD_KEY_DOWN : BE_EVENT_TYPE_KEYBOARD_KEY_UP,
            .keyboard = {
                .key = keyCode
            }
        });
    }

    void BE_OpenGLWindow_MouseButtonEvent(GLFWwindow* theWindow, int button, int action, int mods) {
        double x;
        double y;

        glfwGetCursorPos(window, &x, &y);
        BE_Mouse_SetButtonDown(button, action != GLFW_RELEASE);
        BE_Layer_OnEvent((BE_Event) {
            .type = action != GLFW_RELEASE ? BE_EVENT_TYPE_MOUSE_BUTTON_DOWN : BE_EVENT_TYPE_MOUSE_BUTTON_UP,
            .mouse = {
                .position = (BE_Vector_2F) {(float) x, (float) y},
                .button = {
                    button
                }
            }
        });
    }

    void BE_OpenGLWindow_CursorPositionMovedEvent(GLFWwindow* theWindow, double x, double y) {
        BE_Layer_OnEvent((BE_Event) {
            .type = BE_EVENT_TYPE_MOUSE_MOVED,
            .mouse = {
                .position = (BE_Vector_2F) {(float) x, (float) y}
            }
        });
    }

    void BE_OpenGLWindow_CursorEnterEvent(GLFWwindow* theWindow, int entered) {
        BE_Layer_OnEvent((BE_Event) {
            .type = entered ? BE_EVENT_TYPE_MOUSE_ENTER : BE_EVENT_TYPE_MOUSE_LEAVE
        });
    }

    void BE_OpenGLWindow_ScrollEvent(GLFWwindow* theWindow, double x, double y) {
        double xPos;
        double yPos;

        glfwGetCursorPos(window, &xPos, &yPos);
        BE_Layer_OnEvent((BE_Event) {
            .type = BE_EVENT_TYPE_MOUSE_WHEEL,
            .mouse = {
                .position = (BE_Vector_2F) {(float) xPos, (float) yPos},
                .wheel = {
                    .scrollAmount = (BE_Vector_2F) {(float) x, (float) y}
                }
            }
        });
    }

    void BE_OpenGLWindow_FocusEvent(GLFWwindow* theWindow, int focused) {
        BE_Layer_OnEvent((BE_Event) {
            .type = focused ? BE_EVENT_TYPE_WINDOW_FOCUS_GAINED : BE_EVENT_TYPE_WINDOW_FOCUS_LOST
        });
    }

    void BE_OpenGLWindow_CloseEvent(GLFWwindow* theWindow) {
        BE_Layer_OnEvent((BE_Event) {
            .type = BE_EVENT_TYPE_WINDOW_CLOSE
        });
    }

    void BE_OpenGLWindow_MaximizeEvent(GLFWwindow* theWindow, int maximized) {
        BE_Layer_OnEvent((BE_Event) {
            .type = maximized ? BE_EVENT_TYPE_WINDOW_MAXIMIZED : BE_EVENT_TYPE_WINDOW_RESTORED
        });
    }

    void BE_OpenGLWindow_MovedEvent(GLFWwindow* theWindow, int x, int y) {
        BE_Layer_OnEvent((BE_Event) {
            .type = BE_EVENT_TYPE_WINDOW_MOVED,
            .window = {
                .newPosition = (BE_Vector_2I) {x, y}
            }
        });
    }

    void BE_OpenGLWindow_ResizedEvent(GLFWwindow* theWindow, int width, int height) {
        glViewport(0, 0, width, height);
        BE_Layer_OnEvent((BE_Event) {
            .type = BE_EVENT_TYPE_WINDOW_RESIZED,
            .window = {
                .newSize = (BE_Vector_2I) {width, height}
            }
        });
    }

    void BE_OpenGLWindow_Create(const char* title, BE_Vector_2U size, int monitor) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
        BE_ASSERT(window = glfwCreateWindow(size.x, size.y, title, NULL, NULL), "Failed to create GLFW window");
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
        BE_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize Glad");
        glViewport(0, 0, (int) size.x, (int) size.y);
        BE_OpenGLRenderer_CompileShaders();

        windowTitle = title;
    }

    const char* BE_OpenGLWindow_GetTitle(void) {
        return windowTitle;
    }

    BE_Vector_2U BE_OpenGLWindow_GetSize(void) {
        unsigned width;
        unsigned height;

        glfwGetWindowSize(window, (int*) &width, (int*) &height);

        return (BE_Vector_2U) {width, height};
    }

    BE_Vector_2I BE_OpenGLWindow_GetPosition(void) {
        int x;
        int y;

        glfwGetWindowPos(window, &x, &y);

        return (BE_Vector_2I) {x, y};
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

    void BE_OpenGLWindow_SetSize(BE_Vector_2U newSize) {
        glfwSetWindowSize(window, (int) newSize.x, (int) newSize.y);
    }

    void BE_OpenGLWindow_SetPosition(BE_Vector_2I newPosition) {
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
SEC_CPP_GUARD_END()
