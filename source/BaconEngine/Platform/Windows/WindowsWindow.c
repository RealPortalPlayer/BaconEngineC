#include <Windows.h>

#include "WindowsWindow.h"
#include "BaconEngine/Debugging/Assert.h"

SEC_CPP_SUPPORT_GUARD_START()
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

void BE_WindowsWindow_Create(const char* title, BE_Vector_2U size, int monitor) {
    HINSTANCE instance = GetModuleHandle(0);

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

    BE_ASSERT(RegisterClassEx(&windowClass), "Failed to register window class: %u\n", GetLastError());

    beWindowsWindowHandle = CreateWindowEx(WS_EX_CLIENTEDGE, beWindowsWindowClassName, title, WS_OVERLAPPEDWINDOW,
                                           CW_USEDEFAULT, CW_USEDEFAULT, (int) size.x, (int) size.y, NULL, NULL, instance, NULL);

    BE_ASSERT(beWindowsWindowHandle != NULL, "Failed to create window: %u\n", GetLastError());
    ShowWindow(beWindowsWindowHandle, SW_SHOW);
    UpdateWindow(beWindowsWindowHandle);
}

const char* BE_WindowsWindow_GetTitle(void) {
    char* title;
    int length = GetWindowTextLength(beWindowsWindowHandle);

    // FIXME: Is this a memory leak?
    title = VirtualAlloc(NULL, length + 1, MEM_COMMIT, PAGE_READWRITE);

    GetWindowText(beWindowsWindowHandle, title, length + 1);

    return title;
}

BE_Vector_2U BE_WindowsWindow_GetSize(void) {
    RECT rectangle = SEC_CPP_SUPPORT_CREATE_STRUCT(RECT, 0);

    GetWindowRect(beWindowsWindowHandle, &rectangle);

    return SEC_CPP_SUPPORT_CREATE_STRUCT(BE_Vector_2U, rectangle.right - rectangle.left,
                                                       rectangle.bottom - rectangle.top);
}

BE_Vector_2I BE_WindowsWindow_GetPosition(void) {
    RECT rectangle = SEC_CPP_SUPPORT_CREATE_STRUCT(RECT, 0);

    GetWindowRect(beWindowsWindowHandle, &rectangle);

    return SEC_CPP_SUPPORT_CREATE_STRUCT(BE_Vector_2I, rectangle.left, rectangle.top);
}

int BE_WindowsWindow_IsVisible(void) {
    return IsWindowVisible(beWindowsWindowHandle);
}

int BE_WindowsWindow_IsStillOpened(void) {
    return beWindowsWindowHandle != NULL;
}

void BE_WindowsWindow_SetTitle(const char* title) {
    SetWindowText(beWindowsWindowHandle, title);
}

void BE_WindowsWindow_SetSize(BE_Vector_2U newSize) {
    BE_Vector_2I position = BE_WindowsWindow_GetPosition();

    SetWindowPos(beWindowsWindowHandle, HWND_TOP, position.x, position.y, (int) newSize.x, (int) newSize.y, SWP_NOOWNERZORDER);
}

void BE_WindowsWindow_SetPosition(BE_Vector_2I newPosition) {
    BE_Vector_2U size = BE_WindowsWindow_GetSize();

    SetWindowPos(beWindowsWindowHandle, HWND_TOP, newPosition.x, newPosition.y, (int) size.x, (int) size.y, SWP_NOOWNERZORDER);
}

void BE_WindowsWindow_SetVisibility(int visible) {
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
SEC_CPP_SUPPORT_GUARD_END()
