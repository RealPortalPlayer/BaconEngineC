#include "BaconEngine/Rendering/UI.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "../EngineMemory.h"
#include "PrivateUI.h"
#include "BaconEngine/Math/Bitwise.h"
#include "EngineUIs.h"

SEC_CPP_SUPPORT_GUARD_START()
BE_DynamicArray beUIWindows;
int beUIInitialized;
//BE_PrivateUI_Window* beUIFullscreenWindow;
BE_DynamicArray beUIRenderOrder;

BE_PrivateUI_Window* BE_UI_GetWindowFromId(unsigned windowId) {
    BE_STRICTMODE_CHECK(beUIWindows.used > (int) windowId, NULL, "Invalid UI window ID\n");
    return BE_DYNAMICARRAY_GET_ELEMENT(BE_PrivateUI_Window, beUIWindows, windowId);
}

const BE_DynamicArray* BE_PrivateUI_GetWindows(void) {
    return &beUIWindows;
}

const BE_DynamicArray* BE_PrivateUI_GetRenderWindows(void) {
    return &beUIRenderOrder;
}

// TODO: Make this private.
void BE_PrivateUI_Initialize(void) {
    BE_ASSERT(!beUIInitialized, "The UI system was already initialized\n");

    beUIInitialized = 1;

    if (!BE_DynamicArray_Create(&beUIWindows, 100) || !BE_DynamicArray_Create(&beUIRenderOrder, 100)) {
        beUIInitialized = 0;
        return;
    }

    BE_EngineUIs_Initialize();
}

unsigned BE_UI_RegisterWindow(const char* name, BE_UI_WindowFlags flags, BE_Vector_2I position, BE_Vector_2U size) {
    BE_ASSERT(beUIInitialized, "UI system is not initialized\n");

    beUIInitialized = 1;

    BE_PrivateUI_Window* uiWindow = (BE_PrivateUI_Window*) BE_EngineMemory_AllocateMemory(sizeof(BE_PrivateUI_Window),
                                                                                          BE_ENGINEMEMORY_MEMORY_TYPE_UI);

    // TODO: Validate flags

    uiWindow->name = name;
    uiWindow->flags = flags;
    uiWindow->position = position;
    uiWindow->size = size;
    uiWindow->currentRenderPosition = beUIRenderOrder.used;

    BE_DynamicArray_Create(&uiWindow->elements, 100);
    BE_DynamicArray_AddElementToLast(&beUIWindows, uiWindow);
    BE_DynamicArray_AddElementToLast(&beUIRenderOrder, uiWindow);

    return (uiWindow->windowId = beUIWindows.used - 1);
}

int BE_UI_RegisterElement(unsigned windowId, BE_UI_Element* element) {
    BE_ASSERT(beUIInitialized, "UI system is not initialized\n");

    BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

    return uiWindow != NULL && BE_DynamicArray_AddElementToLast(&uiWindow->elements, element);
}

int BE_UI_ToggleWindowFlag(unsigned windowId, BE_UI_WindowFlags flag, int toggle) {
    BE_ASSERT(beUIInitialized, "UI system is not initialized\n");

    BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

    if (uiWindow == NULL || BE_BITWISE_IS_BIT_SET(uiWindow->flags, flag) == toggle)
        return 0;

    BE_BITWISE_TOGGLE_BIT(uiWindow->flags, flag);
    return 1;
}

const char* BE_UI_GetWindowName(unsigned windowId) {
    BE_ASSERT(beUIInitialized, "UI system is not initialized\n");

    BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

    return uiWindow != NULL ? uiWindow->name : "";
}

int BE_UI_IsWindowStillOpen(unsigned windowId) {
    BE_ASSERT(beUIInitialized, "UI system is not initialized\n");

    BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

    return uiWindow != NULL && !BE_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_CLOSED);
}

BE_DynamicArray* BE_UI_GetWindowElements(unsigned windowId) {
    BE_ASSERT(beUIInitialized, "UI system is not initialized\n");

    BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

    return uiWindow != NULL ? &uiWindow->elements : NULL;
}

int BE_UI_SetActiveWindow(unsigned windowId) {
    BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

    if (uiWindow == NULL || beUIRenderOrder.used <= 1 || uiWindow->currentRenderPosition == 0)
        return 0;

    BE_PrivateUI_Window* activeWindow = BE_DYNAMICARRAY_GET_ELEMENT(BE_PrivateUI_Window, beUIRenderOrder, 0);

    beUIRenderOrder.internalArray[0] = uiWindow;
    beUIRenderOrder.internalArray[uiWindow->currentRenderPosition] = activeWindow;
    activeWindow->currentRenderPosition = uiWindow->currentRenderPosition;
    uiWindow->currentRenderPosition = 0;

    return 1;
}

int BE_UI_GetWindowAmount(void) {
    return beUIInitialized ? beUIWindows.used : 0;
}

int BE_UI_GetAllocatedWindowsAmount(void) {
    return beUIInitialized ? (int) beUIWindows.size : 0;
}

int BE_UI_GetWindowReallocationAmount(void) {
    return beUIInitialized ? beUIWindows.calledRealloc : 0;
}

// TODO: Make this private.
void BE_PrivateUI_Destroy(void) {
    BE_ASSERT(beUIInitialized, "UI system has already been destroyed\n");
    BE_ASSERT(!BE_ClientInformation_IsRunning(), "Cannot destroy UI system while still running\n");

    beUIInitialized = 0;

    for (int windowId = 0; windowId < beUIWindows.used; windowId++) {
        BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

        for (int elementId = 0; elementId < uiWindow->elements.used; elementId++)
            BE_EngineMemory_DeallocateMemory(uiWindow->elements.internalArray[elementId], sizeof(BE_UI_Element), BE_ENGINEMEMORY_MEMORY_TYPE_UI);

        BE_EngineMemory_DeallocateMemory(uiWindow->elements.internalArray, sizeof(void*) * uiWindow->elements.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
        BE_EngineMemory_DeallocateMemory(uiWindow, sizeof(BE_PrivateUI_Window), BE_ENGINEMEMORY_MEMORY_TYPE_UI);
    }

    BE_EngineMemory_DeallocateMemory(beUIRenderOrder.internalArray, sizeof(void*) * beUIRenderOrder.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
    BE_EngineMemory_DeallocateMemory(beUIWindows.internalArray, sizeof(void*) * beUIWindows.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
}
SEC_CPP_SUPPORT_GUARD_END()
