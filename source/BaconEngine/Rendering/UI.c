#include <SharedEngineCode/Internal/CppHeader.h>
#include <string.h>

#include "BaconEngine/Rendering/UI.h"
#include "BaconEngine/Storage/DynamicArray.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"

CPP_GUARD_START()
    DynamicArray uiArray;

    void InitializeUISystem(void) {
        static int initialized = 0;

        STRICT_CHECK_NO_RETURN_VALUE(!initialized, "Already initialized UI system.");

        initialized = 1;

        CreateDynamicArray(&uiArray, 50);
    }

    void RegisterUIWindow(const char* name, Vector2I position, Vector2I size, UIWindowFlags flags) {
        for (int i = 0; i < uiArray.used; i++)
            STRICT_CHECK_NO_RETURN_VALUE(strcmp(GET_ELEMENT(UIWindow, uiArray, i)->name, name) != 0, "The window '%s' is already registered, to have more than window with the same name, append one with \"###\" with anything else");

        UIWindow* window = malloc(sizeof(UIWindow));

        ASSERT(window != NULL, "Failed to allocate %lu bytes of data for a UI window", sizeof(UIWindow));

        window->name = name;
        window->position = position;
        window->size = size;
        window->flags = flags;

        ArrayPushElement(&uiArray, (void*) window);
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
CPP_GUARD_END()
