#include <SharedEngineCode/Internal/CppHeader.h>
#include <string.h>
#include <zlib.h>

#include "BaconEngine/Rendering/UI.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "EngineUIs.h"
#include "BaconEngine/Rendering/Window.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/Layer.h"

CPP_GUARD_START()
    BE_DynamicArray uiArray;
    BE_UIWindow* currentWindow = NULL;
    TTF_Font* windowFont;
    BE_UIWindow* registeringWindow = NULL;

    void BE_InitializeUISystem(void) {
        static int initialized = 0;

        if (GetInternalSDLRenderer() == NULL)
            return;

        BE_STRICT_CHECK_NO_RETURN_VALUE(!initialized, "Already initialized UI system.");
        SEC_LOG_INFO("Initializing UI");

        initialized = 1;

        BE_CreateDynamicArray(&uiArray, 50);
        SEC_LOG_INFO("Registering engine UIs");
        InitializeEngineUIs();

//        STRICT_CHECK_NO_RETURN_VALUE((fontMemory = SDL_RWFromConstMem(file->contents, strlen((const char*) file->contents))) != NULL, "Failed to get window font: %s", SDL_GetError());
//        STRICT_CHECK_NO_RETURN_VALUE((windowFont = TTF_OpenFontRW(fontMemory, 1, 15)) != NULL, "Failed to get window font: %s", SDL_GetError()); // FIXME: Find a royalty free font!
    }

    void BE_RegisterUIWindow(const char* name, BE_Vector2I position, BE_Vector2U size, BE_UIWindowFlags flags) {
        BE_STRICT_CHECK_NO_RETURN_VALUE(registeringWindow == NULL, "You're already registering another window");

        for (int i = 0; i < uiArray.used; i++)
            BE_STRICT_CHECK_NO_RETURN_VALUE(strcmp(BE_ARRAY_GET_ELEMENT(BE_UIWindow, uiArray, i)->name, name) != 0, "The window '%s' is already registered, to have more than window with the same name, append one with \"###\" with anything else");

        BE_UIWindow* window;

        BE_ASSERT_MALLOC(window, sizeof(BE_UIWindow), "a UI window");
        BE_STRICT_CHECK_NO_RETURN_VALUE((flags & BE_UI_WINDOW_FLAG_NO_TITLE_BAR) == 0 || (flags & BE_UI_WINDOW_FLAG_MINIMIZED) == 0,
                                        "Invalid flags, minimized only works on windows with a title bar");
        BE_STRICT_CHECK_NO_RETURN_VALUE(currentWindow == NULL || (flags & BE_UI_WINDOW_FLAG_MAXIMIZED) == 0,
                                        "Invalid flags, only one window can be maximized");

        char* parsedName;

        BE_ASSERT_MALLOC(parsedName, sizeof(char) * strlen(name) + 1, "UI window name");
        memcpy(parsedName, name, strlen(name));
        {
            char* parsing = strstr(parsedName, "###");

            if (parsing != NULL)
                *parsing = '\0';
        }

        window->name = parsedName;
        window->originalName = name;
        window->position = position;
        window->size = size;
        window->flags = flags;

        BE_CreateDynamicArray(&window->elements, 100);

        registeringWindow = window;
    }

    void BE_RegisterUIElement(BE_UIElement* element) {
        BE_STRICT_CHECK_NO_RETURN_VALUE(registeringWindow != NULL, "There is no window to finish");

        BE_ArrayAddElementToLast(&registeringWindow->elements, element);
    }

    void BE_FinishRegisteringUIWindow(void) {
        BE_STRICT_CHECK_NO_RETURN_VALUE(registeringWindow != NULL, "There is no window to finish");

        if (currentWindow == NULL || (currentWindow->flags & BE_UI_WINDOW_FLAG_MAXIMIZED) == 0)
            currentWindow = registeringWindow;

        BE_ArrayAddElementToLast(&uiArray, registeringWindow);

        registeringWindow = NULL;
    }

    int BE_CloseUIWindow(const char* name) {
        for (unsigned int windowId = 0; (int) windowId < uiArray.used; windowId++) {
            BE_UIWindow* window = BE_ARRAY_GET_ELEMENT(BE_UIWindow, uiArray, windowId);

            if (strcmp(window->name, name) != 0)
                continue;

            return BE_CloseUIWindowAt(windowId);
        }

        return 0;
    }

    int BE_CloseUIWindowAt(unsigned int index) {
        if ((int) index >= uiArray.used || (BE_GetUIWindows()[index]->flags & BE_UI_WINDOW_FLAG_CLOSED) != 0)
            return 0;

        if (BE_GetCurrentUIWindow() == currentWindow)
            currentWindow = NULL;

        BE_GetUIWindows()[index]->flags |= BE_UI_WINDOW_FLAG_CLOSED;
        return 1;
    }

    int BE_ToggleUIRendering(int enable) {
        return BE_ToggleLayer("UIManager", enable);
    }

    void UIWindowRendererDrawRectangle(BE_UIWindow* window, BE_Vector2I position, BE_Vector2U size, BE_Color4U borderColor, BE_Color4U fillColor, int borderPadding) {
        if ((window->flags & BE_UI_WINDOW_FLAG_NO_BORDER) != 0)
            BE_RendererFillRectangle((BE_Vector2I) {position.x - 1, position.y - 1},
                                     (BE_Vector2U) {size.x + borderPadding, size.y + borderPadding}, fillColor);
        else
            BE_RendererDrawBorderedRectangle(position, size, borderColor, fillColor, borderPadding);
    }

    BE_UIWindow** BE_GetUIWindows(void) {
        return (BE_UIWindow**) uiArray.internalArray;
    }

    int BE_GetUIWindowAmount(void) {
        return uiArray.used;
    }

    int BE_GetAllocatedUIWindowAmount(void) {
        return (int) uiArray.size;
    }

    int BE_GetUIWindowReallocationAmount(void) {
        return uiArray.calledRealloc;
    }

    void BE_SetCurrentUIWindow(BE_UIWindow* window) {
        currentWindow = window;
    }

    BE_UIWindow* BE_GetCurrentUIWindow(void) {
        return currentWindow;
    }

    TTF_Font* BE_GetUIWindowFont(void) {
        return windowFont;
    }
CPP_GUARD_END()
