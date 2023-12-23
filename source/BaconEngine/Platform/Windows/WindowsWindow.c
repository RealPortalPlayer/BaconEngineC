// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <Windows.h>
#include <BaconAPI/Debugging/Assert.h>

#include "WindowsWindow.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
HWND beWindowsWindowHandle;
const char beWindowsWindowClassName[] = "BaconEngineSoftwareWindow";

LRESULT CALLBACK BE_WindowsWindow_WndProc(HWND handle, UINT message, WPARAM wordParameter, LPARAM longParameter) {
    // TODO: Parse events.

    switch (message) {
        case WM_CLOSE:
            DestroyWindow(handle);

            handle = NULL;
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            break;
    }

    return DefWindowProc(handle, message, wordParameter, longParameter);
}

void* BE_WindowsWindow_GetInternal(void) {
    return beWindowsWindowHandle;
}

void BE_WindowsWindow_Create(const char* title, BE_Vector2_Unsigned size, int monitor) {
    HINSTANCE instance = GetModuleHandle(0);

    (void) monitor;

    WNDCLASSEX windowClass = {
        sizeof(WNDCLASSEX),
        0,
        &BE_WindowsWindow_WndProc,
        0,
        0,
        instance,
        LoadIcon(NULL, IDI_APPLICATION), // TODO: Custom icon.
        LoadCursor(NULL, IDC_ARROW),
        GetStockObject(BLACK_BRUSH),
        NULL,
        beWindowsWindowClassName,
        LoadIcon(NULL, IDI_APPLICATION)
    };

    BA_ASSERT(RegisterClassEx(&windowClass), "Failed to register window class: %u\n", GetLastError());

    beWindowsWindowHandle = CreateWindowEx(WS_EX_CLIENTEDGE, beWindowsWindowClassName, title, WS_OVERLAPPEDWINDOW,
                                           CW_USEDEFAULT, CW_USEDEFAULT, (int) size.x, (int) size.y, NULL, NULL, instance, NULL);

    BA_ASSERT(beWindowsWindowHandle != NULL, "Failed to create window: %u\n", GetLastError());
    ShowWindow(beWindowsWindowHandle, SW_SHOW);
    UpdateWindow(beWindowsWindowHandle);
}

const char* BE_WindowsWindow_GetTitle(void) {
    static char* title = NULL;
    int length = GetWindowTextLength(beWindowsWindowHandle);

    {
        char* newTitle = VirtualAlloc(NULL, length + 1, MEM_COMMIT, PAGE_READWRITE);

        GetWindowText(beWindowsWindowHandle, newTitle, length + 1);

        if (title == NULL || strcmp(title, newTitle) != 0)
            title = newTitle;
        else
            VirtualFree(newTitle, 0, MEM_RELEASE);
    }
    

    return title;
}

BE_Vector2_Unsigned BE_WindowsWindow_GetSize(void) {
    RECT rectangle = BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(RECT, 0);

    GetWindowRect(beWindowsWindowHandle, &rectangle);

    return BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, rectangle.right - rectangle.left,
                                                       rectangle.bottom - rectangle.top);
}

BE_Vector2_Integer BE_WindowsWindow_GetPosition(void) {
    RECT rectangle = BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(RECT, 0);

    GetWindowRect(beWindowsWindowHandle, &rectangle);

    return BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Integer, rectangle.left, rectangle.top);
}

BA_Boolean BE_WindowsWindow_IsVisible(void) {
    return IsWindowVisible(beWindowsWindowHandle);
}

BA_Boolean BE_WindowsWindow_IsStillOpened(void) {
    return beWindowsWindowHandle != NULL;
}

void BE_WindowsWindow_SetTitle(const char* title) {
    SetWindowText(beWindowsWindowHandle, title);
}

void BE_WindowsWindow_SetSize(BE_Vector2_Unsigned newSize) {
    BE_Vector2_Integer position = BE_WindowsWindow_GetPosition();

    SetWindowPos(beWindowsWindowHandle, HWND_TOP, position.x, position.y, (int) newSize.x, (int) newSize.y, SWP_NOOWNERZORDER);
}

void BE_WindowsWindow_SetPosition(BE_Vector2_Integer newPosition) {
    BE_Vector2_Unsigned size = BE_WindowsWindow_GetSize();

    SetWindowPos(beWindowsWindowHandle, HWND_TOP, newPosition.x, newPosition.y, (int) size.x, (int) size.y, SWP_NOOWNERZORDER);
}

void BE_WindowsWindow_SetVisibility(BA_Boolean visible) {
    ShowWindow(beWindowsWindowHandle, visible ? SW_SHOW : SW_HIDE);
}

void BE_WindowsWindow_Close(void) {
    PostQuitMessage(0);
}

void BE_WindowsWindow_UpdateEvents(void) {
    MSG message;

    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
