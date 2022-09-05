#include <SharedEngineCode/Internal/CppHeader.h>
#include <string.h>
#include <SharedEngineCode/Debugging/StrictMode.h>
#include <SharedEngineCode/Debugging/Assert.h>

#include "BaconEngine/Rendering/UI.h"
#include "EngineUIs.h"
#include "BaconEngine/Rendering/Window.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/Layer.h"

SEC_CPP_GUARD_START()
    int uiInitialized = 0;
    SEC_DynamicArray uiArray;
    BE_UI_Window* currentWindow = NULL;
    TTF_Font* windowFont;
    BE_UI_Window* registeringWindow = NULL;

    void BE_UI_InitializeUIs(void) {
        if (BE_Window_GetInternalSDLRenderer() == NULL)
            return;

        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(!uiInitialized, "Already initialized UI system.");
        SEC_LOGGER_INFO("Initializing UI");

        uiInitialized = 1;

        SEC_DynamicArray_Create(&uiArray, 50);
        SEC_LOGGER_INFO("Registering engine UIs");
        InitializeEngineUIs();

//        STRICT_CHECK_NO_RETURN_VALUE((fontMemory = SDL_RWFromConstMem(file->contents, strlen((const char*) file->contents))) != NULL, "Failed to get window font: %s", SDL_GetError());
//        STRICT_CHECK_NO_RETURN_VALUE((windowFont = TTF_OpenFontRW(fontMemory, 1, 15)) != NULL, "Failed to get window font: %s", SDL_GetError()); // FIXME: Find a royalty free font!
    }

    void BE_UI_RegisterWindow(const char* name, BE_Vector_2I position, BE_Vector_2U size, BE_UI_WindowFlags flags) {
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(registeringWindow == NULL, "You're already registering another window");

        for (int i = 0; i < uiArray.used; i++)
            SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(strcmp(SEC_DYNAMICARRAY_GET_ELEMENT(BE_UI_Window, uiArray, i)->name, name) != 0, "The window '%s' is already registered, to have more than window with the same name, append one with \"###\" with anything else");

        BE_UI_Window* window;

        SEC_ASSERT_MALLOC(window, sizeof(BE_UI_Window), "a UI window");
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE((flags & BE_UI_WINDOW_FLAG_NO_TITLE_BAR) == 0 || (flags & BE_UI_WINDOW_FLAG_MINIMIZED) == 0,
                                             "Invalid flags, minimized only works on windows with a title bar");
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(currentWindow == NULL || (flags & BE_UI_WINDOW_FLAG_MAXIMIZED) == 0,
                                             "Invalid flags, only one window can be maximized");

        char* parsedName;

        SEC_ASSERT_MALLOC(parsedName, sizeof(char) * strlen(name) + 1, "UI window name");
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

        SEC_DynamicArray_Create(&window->elements, 100);

        registeringWindow = window;
    }

    void BE_UI_RegisterElement(BE_UIElement* element) {
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(registeringWindow != NULL, "There is no window to finish");
        SEC_DynamicArray_AddElementToLast(&registeringWindow->elements, element);
    }

    void BE_UI_FinishRegisteringWindow(void) {
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(registeringWindow != NULL, "There is no window to finish");

        if (currentWindow == NULL || (currentWindow->flags & BE_UI_WINDOW_FLAG_MAXIMIZED) == 0)
            currentWindow = registeringWindow;

        SEC_DynamicArray_AddElementToLast(&uiArray, registeringWindow);

        registeringWindow = NULL;
    }

    int BE_UI_CloseWindow(const char* name) {
        for (unsigned int windowId = 0; (int) windowId < uiArray.used; windowId++) {
            BE_UI_Window* window = SEC_DYNAMICARRAY_GET_ELEMENT(BE_UI_Window, uiArray, windowId);

            if (strcmp(window->name, name) != 0)
                continue;

            return BE_UI_CloseWindowAt(windowId);
        }

        return 0;
    }

    int BE_UI_CloseWindowAt(unsigned int index) {
        if ((int) index >= uiArray.used || (BE_UI_GetWindows()[index]->flags & BE_UI_WINDOW_FLAG_CLOSED) != 0)
            return 0;

        if (BE_UI_GetCurrentWindow() == currentWindow)
            currentWindow = NULL;

        BE_UI_GetWindows()[index]->flags |= BE_UI_WINDOW_FLAG_CLOSED;
        return 1;
    }

    int BE_UI_ToggleWindowRendering(int enable) {
        return BE_Layer_Toggle("UIManager", enable);
    }

    void BE_UI_WindowRendererDrawRectangle(BE_UI_Window* window, BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U borderColor, BE_Color_4U fillColor, int borderPadding) {
        if ((window->flags & BE_UI_WINDOW_FLAG_NO_BORDER) != 0)
            BE_Renderer_FillRectangle((BE_Vector_2I) {position.x - 1, position.y - 1},
                                      (BE_Vector_2U) {size.x + borderPadding, size.y + borderPadding}, fillColor);
        else
            BE_Renderer_DrawBorderedRectangle(position, size, borderColor, fillColor, borderPadding);
    }

    BE_UI_Window** BE_UI_GetWindows(void) {
        return (BE_UI_Window**) uiArray.internalArray;
    }

    int BE_UI_GetWindowAmount(void) {
        return uiArray.used;
    }

    int BE_UI_GetAllocatedWindowAmount(void) {
        return (int) uiArray.size;
    }

    int BE_UI_GetWindowReallocationAmount(void) {
        return uiArray.calledRealloc;
    }

    void BE_UI_SetCurrentWindow(BE_UI_Window* window) {
        currentWindow = window;
    }

    BE_UI_Window* BE_UI_GetCurrentWindow(void) {
        return currentWindow;
    }

    TTF_Font* BE_UI_GetWindowFont(void) {
        return windowFont;
    }

    void BE_UI_DestroyWindows(void) {
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(uiInitialized, "UI Windows are already destroyed");
        SEC_LOGGER_INFO("Destroying UI Windows");

        uiInitialized = 0;

        for (int i = 0; i < uiArray.used; i++) {
            BE_UI_Window* layer = SEC_DYNAMICARRAY_GET_ELEMENT(BE_UI_Window, uiArray, i);

            SEC_EngineMemory_RemoveAllocated(sizeof(BE_UI_Window));
            free(layer);
        }

        SEC_EngineMemory_RemoveAllocated(sizeof(void *) * uiArray.size);
        free(uiArray.internalArray);
    }
SEC_CPP_GUARD_END()
