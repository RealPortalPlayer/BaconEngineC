#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Rendering/UI.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "../EngineMemory.h"
#include "PrivateUI.h"
#include "BaconEngine/ClientInformation.h"
#include "BaconEngine/Math/Bitwise.h"
#include "EngineUIs.h"

SEC_CPP_GUARD_START()
    BE_DynamicArray uiWindows;
    int initialized;
    BE_PrivateUI_Window* fullscreenWindow;
    BE_DynamicArray renderOrder;

    BE_PrivateUI_Window* BE_UI_GetWindowFromId(unsigned windowId) {
        BE_STRICTMODE_CHECK(uiWindows.used > windowId, NULL, "Invalid UI window ID");
        return BE_DYNAMICARRAY_GET_ELEMENT(BE_PrivateUI_Window, uiWindows, windowId);
    }

    const BE_DynamicArray* BE_PrivateUI_GetWindows(void) {
        return &uiWindows;
    }

    const BE_DynamicArray* BE_PrivateUI_GetRenderWindows(void) {
        return &renderOrder;
    }

    void BE_UI_Initialize(void) {
        BE_STRICTMODE_CHECK_NO_RETURN_VALUE(!initialized, "The UI system was already initialized");

        initialized = 1;

        if (!BE_DynamicArray_Create(&uiWindows, 100) || !BE_DynamicArray_Create(&renderOrder, 100)) {
            initialized = 0;
            return;
        }

        BE_EngineUIs_Initialize();
    }

    unsigned BE_UI_RegisterWindow(const char* name, BE_UI_WindowFlags flags, BE_Vector_2I position, BE_Vector_2U size) {
        BE_ASSERT(initialized, "UI system is not initialized");

        initialized = 1;

        BE_PrivateUI_Window* uiWindow = BE_EngineMemory_AllocateMemory(sizeof(BE_PrivateUI_Window), BE_ENGINEMEMORY_MEMORY_TYPE_UI);

        // TODO: Validate flags

        uiWindow->name = name;
        uiWindow->flags = flags;
        uiWindow->position = position;
        uiWindow->size = size;
        uiWindow->currentRenderPosition = renderOrder.used;

        BE_DynamicArray_Create(&uiWindow->elements, 100);
        BE_DynamicArray_AddElementToLast(&uiWindows, uiWindow);
        BE_DynamicArray_AddElementToLast(&renderOrder, uiWindow);

        return (uiWindow->windowId = uiWindows.used - 1);
    }

    int BE_UI_RegisterElement(unsigned windowId, BE_UI_Element* element) {
        BE_ASSERT(initialized, "UI system is not initialized");

        BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

        return uiWindow != NULL && BE_DynamicArray_AddElementToLast(&uiWindow->elements, element);
    }

    int BE_UI_ToggleWindowFlag(unsigned windowId, BE_UI_WindowFlags flag, int toggle) {
        BE_ASSERT(initialized, "UI system is not initialized");

        BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

        if (uiWindow == NULL || BE_BITWISE_IS_BIT_SET(uiWindow->flags, flag) == toggle)
            return 0;

        BE_BITWISE_TOGGLE_BIT(uiWindow->flags, flag);
        return 1;
    }

    const char* BE_UI_GetWindowName(unsigned windowId) {
        BE_ASSERT(initialized, "UI system is not initialized");

        BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

        return uiWindow != NULL ? uiWindow->name : "";
    }

    int BE_UI_IsWindowStillOpen(unsigned windowId) {
        BE_ASSERT(initialized, "UI system is not initialized");

        BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

        return uiWindow != NULL && (uiWindow->flags & BE_UI_WINDOW_FLAG_CLOSED) == 0;
    }

    BE_DynamicArray* BE_UI_GetWindowElements(unsigned windowId) {
        BE_ASSERT(initialized, "UI system is not initialized");

        BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

        return uiWindow != NULL ? &uiWindow->elements : NULL;
    }

    int BE_UI_SetActiveWindow(unsigned windowId) {
        BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

        if (uiWindow == NULL || renderOrder.used <= 1 || uiWindow->currentRenderPosition == 0)
            return 0;

        BE_PrivateUI_Window* activeWindow = BE_DYNAMICARRAY_GET_ELEMENT(BE_PrivateUI_Window, renderOrder, 0);

        renderOrder.internalArray[0] = uiWindow;
        renderOrder.internalArray[uiWindow->currentRenderPosition] = activeWindow;
        activeWindow->currentRenderPosition = uiWindow->currentRenderPosition;
        uiWindow->currentRenderPosition = 0;

        return 1;
    }

    int BE_UI_GetWindowAmount(void) {
        return initialized ? uiWindows.used : 0;
    }

    int BE_UI_GetAllocatedWindowsAmount(void) {
        return initialized ? (int) uiWindows.size : 0;
    }

    int BE_UI_GetWindowReallocationAmount(void) {
        return initialized ? uiWindows.calledRealloc : 0;
    }

    void BE_UI_Destroy(void) {
        BE_STRICTMODE_CHECK_NO_RETURN_VALUE(initialized, "UI system has already been destroyed");
        BE_STRICTMODE_CHECK_NO_RETURN_VALUE(!BE_ClientInformation_IsRunning(), "Cannot destroy UI system while still running");

        initialized = 0;

        for (int windowId = 0; windowId < uiWindows.used; windowId++) {
            BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

            for (int elementId = 0; elementId < uiWindow->elements.used; elementId++)
                BE_EngineMemory_DeallocateMemory(uiWindow->elements.internalArray[elementId], sizeof(BE_UI_Element), BE_ENGINEMEMORY_MEMORY_TYPE_UI);

            BE_EngineMemory_DeallocateMemory(uiWindow->elements.internalArray, sizeof(void*) * uiWindow->elements.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
            BE_EngineMemory_DeallocateMemory(uiWindow, sizeof(BE_PrivateUI_Window), BE_ENGINEMEMORY_MEMORY_TYPE_UI);
        }

        BE_EngineMemory_DeallocateMemory(renderOrder.internalArray, sizeof(void*) * renderOrder.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
        BE_EngineMemory_DeallocateMemory(uiWindows.internalArray, sizeof(void*) * uiWindows.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
    }
SEC_CPP_GUARD_END()
