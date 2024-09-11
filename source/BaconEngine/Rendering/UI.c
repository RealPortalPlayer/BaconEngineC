// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/Debugging/StrictMode.h>
#include <BaconAPI/Debugging/Assert.h>
#include <BaconAPI/Math/Bitwise.h>

#include "BaconEngine/Rendering/UI.h"
#include "../InterfaceFunctions.h"
#include "BaconEngine/Client/Information.h"

#ifndef BE_CLIENT_BINARY
#   include "../EngineMemory.h"
#   include "PrivateUI.h"
#   include "EngineUIs.h"
#   include "../Storage/PrivateDynamicArray.h"
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static BA_DynamicArray beUIWindows;
static BA_Boolean beUIInitialized;
static BA_DynamicArray beUIRenderOrder;

static BE_PrivateUI_Window* BE_UI_GetWindowFromId(int windowId) {
    SEC_STRICTMODE_CHECK(windowId >= 0 && windowId < beUIWindows.used, NULL, "Invalid UI window ID\n");
    return BA_DYNAMICARRAY_GET_ELEMENT(BE_PrivateUI_Window, beUIWindows, windowId);
}

const BA_DynamicArray* BE_PrivateUI_GetWindows(void) {
    return &beUIWindows;
}

const BA_DynamicArray* BE_PrivateUI_GetRenderWindows(void) {
    return &beUIRenderOrder;
}

void BE_PrivateUI_Initialize(void) {
    BA_ASSERT(!beUIInitialized, "The UI system was already initialized\n");

    beUIInitialized = BA_BOOLEAN_TRUE;

    if (!BE_PrivateDynamicArray_Create(&beUIWindows, 100) || !BE_PrivateDynamicArray_Create(&beUIRenderOrder, 100)) {
        beUIInitialized = BA_BOOLEAN_FALSE;
        return;
    }

    BE_EngineUIs_Initialize();
}
#endif

int BE_UI_RegisterWindow(const char* name, BE_UI_WindowFlags flags, BE_Vector2_Integer position, BE_Vector2_Unsigned size) {
#ifndef BE_CLIENT_BINARY
    BA_ASSERT(beUIInitialized, "UI system is not initialized\n");

    beUIInitialized = BA_BOOLEAN_TRUE;

    BE_PrivateUI_Window* uiWindow = (BE_PrivateUI_Window*) BA_Memory_Allocate(sizeof(BE_PrivateUI_Window), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_UI);

    if (flags != BE_UI_WINDOW_FLAG_NULL) {
        SEC_STRICTMODE_CHECK(!BA_BITWISE_IS_BIT_SET(flags, BE_UI_WINDOW_FLAG_NO_TITLE_BAR) || !BA_BITWISE_IS_BIT_SET(flags, BE_UI_WINDOW_FLAG_MINIMIZED), -1,
                            "Invalid window flags, cannot both be missing a title-bar and also be minimized");
        SEC_STRICTMODE_CHECK(!BA_BITWISE_IS_BIT_SET(flags, BE_UI_WINDOW_FLAG_MINIMIZED) || !BA_BITWISE_IS_BIT_SET(flags, BE_EVENT_TYPE_WINDOW_MAXIMIZED), -1,
                            "Invalid window flags, cannot both be minimized and maximized");
    }

    uiWindow->name = name;
    uiWindow->flags = flags;
    uiWindow->position = position;
    uiWindow->size = size;
    uiWindow->currentRenderPosition = beUIRenderOrder.used;

    BE_PrivateDynamicArray_Create(&uiWindow->elements, 100);
    BA_DynamicArray_AddElementToLast(&beUIWindows, uiWindow);
    BA_DynamicArray_AddElementToLast(&beUIRenderOrder, uiWindow);
    return (uiWindow->windowId = beUIWindows.used - 1);
#else
    BE_INTERFACEFUNCTION(int, const char*, BE_UI_WindowFlags, BE_Vector2_Integer, BE_Vector2_Unsigned);
    return function(name, flags, position, size);
#endif
}

BA_Boolean BE_UI_RegisterElement(int windowId, BE_UI_Element* element) {
#ifndef BE_CLIENT_BINARY
    BA_ASSERT(beUIInitialized, "UI system is not initialized\n");

    BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

    return uiWindow != NULL && BA_DynamicArray_AddElementToLast(&uiWindow->elements, element);
#else
    BE_INTERFACEFUNCTION(BA_Boolean, int, BE_UI_Element*);
    return function(windowId, element);
#endif
}

BA_Boolean BE_UI_ToggleWindowFlag(int windowId, BE_UI_WindowFlags flag, BA_Boolean toggle) {
#ifndef BE_CLIENT_BINARY
    BA_ASSERT(beUIInitialized, "UI system is not initialized\n");

    BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

    if (uiWindow == NULL || BA_BITWISE_IS_BIT_SET(uiWindow->flags, flag) == toggle)
        return BA_BOOLEAN_FALSE;

    BA_BITWISE_TOGGLE_BIT(uiWindow->flags, flag);
    return BA_BOOLEAN_TRUE;
#else
    BE_INTERFACEFUNCTION(BA_Boolean, int, BE_UI_WindowFlags, BA_Boolean);
    return function(windowId, flag, toggle);
#endif
}

const char* BE_UI_GetWindowName(int windowId) {
#ifndef BE_CLIENT_BINARY
    BA_ASSERT(beUIInitialized, "UI system is not initialized\n");

    BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

    return uiWindow != NULL ? uiWindow->name : "";
#else
    BE_INTERFACEFUNCTION(const char*, int);
    return function(windowId);
#endif
}

BA_Boolean BE_UI_IsWindowStillOpen(int windowId) {
#ifndef BE_CLIENT_BINARY
    BA_ASSERT(beUIInitialized, "UI system is not initialized\n");

    BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

    return uiWindow != NULL && !BA_BITWISE_IS_BIT_SET(uiWindow->flags, BE_UI_WINDOW_FLAG_CLOSED);
#else
    BE_INTERFACEFUNCTION(BA_Boolean, int);
    return function(windowId);
#endif
}

BA_DynamicArray* BE_UI_GetWindowElements(int windowId) {
#ifndef BE_CLIENT_BINARY
    BA_ASSERT(beUIInitialized, "UI system is not initialized\n");

    BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

    return uiWindow != NULL ? &uiWindow->elements : NULL;
#else
    BE_INTERFACEFUNCTION(BA_DynamicArray*, int);
    return function(windowId);
#endif
}

BA_Boolean BE_UI_SetActiveWindow(int windowId) {
#ifndef BE_CLIENT_BINARY
    BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

    if (uiWindow == NULL || beUIRenderOrder.used <= 1 || uiWindow->currentRenderPosition == 0)
        return BA_BOOLEAN_FALSE;

    BE_PrivateUI_Window* activeWindow = BA_DYNAMICARRAY_GET_ELEMENT(BE_PrivateUI_Window, beUIRenderOrder, 0);

    beUIRenderOrder.internalArray[0] = uiWindow;
    beUIRenderOrder.internalArray[uiWindow->currentRenderPosition] = activeWindow;
    activeWindow->currentRenderPosition = uiWindow->currentRenderPosition;
    uiWindow->currentRenderPosition = 0;
    return BA_BOOLEAN_TRUE;
#else
    BE_INTERFACEFUNCTION(BA_Boolean, int);
    return function(windowId);
#endif
}

int BE_UI_GetWindowAmount(void) {
#ifndef BE_CLIENT_BINARY
    return beUIInitialized ? beUIWindows.used : 0;
#else
    BE_INTERFACEFUNCTION(int, void);
    return function();
#endif
}

int BE_UI_GetAllocatedWindowsAmount(void) {
#ifndef BE_CLIENT_BINARY
    return beUIInitialized ? (int) beUIWindows.size : 0;
#else
    BE_INTERFACEFUNCTION(int, void);
    return function();
#endif
}

int BE_UI_GetWindowReallocationAmount(void) {
#ifndef BE_CLIENT_BINARY
    return beUIInitialized ? beUIWindows.calledReallocate : 0;
#else
    BE_INTERFACEFUNCTION(int, void);
    return function();
#endif
}

#ifndef BE_CLIENT_BINARY
void BE_PrivateUI_Destroy(void) {
    BA_ASSERT(beUIInitialized, "UI system has already been destroyed\n");
    BA_ASSERT(!BE_ClientInformation_IsRunning(), "Cannot destroy UI system while still running\n");

    beUIInitialized = BA_BOOLEAN_FALSE;

    for (int windowId = 0; windowId < beUIWindows.used; windowId++) {
        BE_PrivateUI_Window* uiWindow = BE_UI_GetWindowFromId(windowId);

        for (int elementId = 0; elementId < uiWindow->elements.used; elementId++)
            BA_Memory_Deallocate(uiWindow->elements.internalArray[elementId], sizeof(BE_UI_Element), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_UI);

        BA_Memory_Deallocate(uiWindow->elements.internalArray, sizeof(void*) * uiWindow->elements.size, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY);
        BA_Memory_Deallocate(uiWindow, sizeof(BE_PrivateUI_Window), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_UI);
    }

    BA_Memory_Deallocate(beUIRenderOrder.internalArray, sizeof(void*) * beUIRenderOrder.size, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY);
    BA_Memory_Deallocate(beUIWindows.internalArray, sizeof(void*) * beUIWindows.size, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY);
}
#endif
BA_CPLUSPLUS_SUPPORT_GUARD_END()
