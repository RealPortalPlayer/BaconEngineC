#include <SharedEngineCode/Internal/CppHeader.h>
#include <string.h>

#include "BaconEngine/Rendering/UI.h"
#include "BaconEngine/Storage/DynamicArray.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "EngineUIs.h"
#include "BaconEngine/Rendering/Window.h"

CPP_GUARD_START()
    DynamicArray uiArray;
    UIWindow* currentWindow = NULL;
    TTF_Font* windowFont;

    void InitializeUISystem(void) {
        static int initialized = 0;

        if (GetInternalSDLRenderer() == NULL)
            return;

        STRICT_CHECK_NO_RETURN_VALUE(!initialized, "Already initialized UI system.");
        LOG_INFO("Initializing UI");

        initialized = 1;

        CreateDynamicArray(&uiArray, 50);
        LOG_INFO("Registering engine UIs");
        InitializeEngineUIs();

        STRICT_CHECK_NO_RETURN_VALUE((windowFont = TTF_OpenFont("arial.ttf", 15)) != NULL, "Failed to get window font: %s", SDL_GetError()); // FIXME: Find a royalty free font!
    }

    void RegisterUIWindow(const char* name, Vector2I position, Vector2U size, UIWindowFlags flags) {
        for (int i = 0; i < uiArray.used; i++)
            STRICT_CHECK_NO_RETURN_VALUE(strcmp(GET_ELEMENT(UIWindow, uiArray, i)->name, name) != 0, "The window '%s' is already registered, to have more than window with the same name, append one with \"###\" with anything else");

        UIWindow* window = malloc(sizeof(UIWindow));

        ASSERT(window != NULL, "Failed to allocate %lu bytes of data for a UI window", sizeof(UIWindow));

        STRICT_CHECK_NO_RETURN_VALUE((flags & UI_WINDOW_FLAG_NO_TITLE_BAR) == 0 || (flags & UI_WINDOW_FLAG_MINIMIZED) == 0,
                                     "Invalid flags, minimized only works on windows with a title bar");

        STRICT_CHECK_NO_RETURN_VALUE(currentWindow == NULL || (flags & UI_WINDOW_FLAG_MAXIMIZED) == 0,
                                     "Invalid flags, only one window can be maximized");

        window->name = name;
        window->position = position;
        window->size = size;
        window->flags = flags;

        if (currentWindow == NULL || (currentWindow->flags & UI_WINDOW_FLAG_MAXIMIZED) == 0)
            currentWindow = window;

        ArrayAddElementToLast(&uiArray, (void *) window);
    }

    int CloseUIWindow(const char* name) {
        for (unsigned int windowId = 0; (int) windowId < uiArray.used; windowId++) {
            UIWindow* window = GET_ELEMENT(UIWindow, uiArray, windowId);

            if (strcmp(window->name, name) != 0)
                continue;

            return CloseUIWindowAt(windowId);
        }

        return 0;
    }

    int CloseUIWindowAt(unsigned int index) {
        if ((int) index >= uiArray.used || (GetUIWindows()[index]->flags & UI_WINDOW_FLAG_CLOSED) != 0)
            return 0;

        if (GetCurrentUIWindow() == currentWindow)
            currentWindow = NULL;

        GetUIWindows()[index]->flags |= UI_WINDOW_FLAG_CLOSED;
        return 1;
    }

    UIWindow** GetUIWindows(void) {
        return (UIWindow**) uiArray.internalArray;
    }

    int GetUIWindowAmount(void) {
        return uiArray.used;
    }

    int GetAllocatedUIWindowAmount(void) {
        return (int) uiArray.size;
    }

    int GetUIWindowReallocationAmount(void) {
        return uiArray.calledRealloc;
    }

    void SetCurrentUIWindow(UIWindow* window) {
        currentWindow = window;
    }

    UIWindow* GetCurrentUIWindow(void) {
        return currentWindow;
    }

    TTF_Font* GetUIWindowFont(void) {
        return windowFont;
    }
CPP_GUARD_END()
