#include <SharedEngineCode/Internal/CppHeader.h>
#include <string.h>

#include "BaconEngine/Rendering/UI.h"
#include "BaconEngine/Storage/DynamicArray.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"

CPP_GUARD_START()
    DynamicArray uiArray;
    UIWindow* currentWindow = NULL;

    void InitializeUISystem(void) {
        static int initialized = 0;

        STRICT_CHECK_NO_RETURN_VALUE(!initialized, "Already initialized UI system.");

        initialized = 1;

        CreateDynamicArray(&uiArray, 50);
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

        if ((flags & UI_WINDOW_FLAG_MAXIMIZED) != 0)
            currentWindow = window;

        ArrayPushElement(&uiArray, (void*) window);
    }

    int DeleteWindow(const char* name) {
        for (unsigned int windowId = 0; windowId < uiArray.used; windowId++) {
            UIWindow* window = GET_ELEMENT(UIWindow, uiArray, windowId);

            if (strcmp(window->name, name) != 0)
                continue;

            return DeleteWindowAt(windowId);
        }

        return 0;
    }

    int DeleteWindowAt(unsigned int index) {
        if (index >= uiArray.used)
            return 0;

        free(uiArray.internalArray[index]);

        return ArrayRemoveElementAt(&uiArray, index);
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
CPP_GUARD_END()
