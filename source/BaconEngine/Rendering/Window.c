#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/Debugging/Assert.h>
#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/ArgumentHandler.h>

#include "BaconEngine/Rendering/Window.h"
#include "BaconEngine/Rendering/Renderer.h"

SEC_CPP_GUARD_START()
    static SDL_Window* window = NULL;
    static SDL_Renderer* renderer = NULL;
    static char* title = NULL;
    static BE_Vector_2U size = {0, 0};
    static BE_Vector_2U position = {0, 0};

    void BE_Window_Initialize(const char* windowTitle, BE_Vector_2U windowSize) {
        (void) windowTitle;
        (void) windowSize;

        if (window != NULL)
            return;

#ifndef BACON_ENGINE_DISABLE_SDL
        SDL_WindowFlags windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;

        switch (BE_Renderer_GetCurrentType()) {
            case BE_RENDERER_TYPE_AUTO:
                break;

            case BE_RENDERER_TYPE_OPENGL:
                windowFlags |= SDL_WINDOW_OPENGL;
                break;

            case BE_RENDERER_TYPE_VULKAN:
                windowFlags |= SDL_WINDOW_VULKAN;
                break;

            case BE_RENDERER_TYPE_TEXT:
                return;
        }

        int monitorNumber = 0;
        {
            const char* preParsedMonitor = SEC_ArgumentHandler_GetValue("--monitor");

            if (preParsedMonitor != NULL) {
                char* error;
                int parsedMonitor = (int) strtol(preParsedMonitor, &error, 0);

                if (error != NULL && strlen(error) != 0) {
                    SEC_LOGGER_ERROR("Invalid display was supplied, ignoring...");

                    parsedMonitor = 0;
                }

                monitorNumber = parsedMonitor;
            }
        }

        if (!SDL_WasInit(SDL_INIT_VIDEO))
            SEC_ASSERT(SDL_Init(SDL_INIT_VIDEO) == 0, "Failed to initialize SDL: %s", SDL_GetError());

        SEC_LOGGER_INFO("Creating window\nTitle: %s\nSize: (%u, %u)\nMonitor: %i", windowTitle, windowSize.x, windowSize.y, monitorNumber);
        SEC_ASSERT((window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED_DISPLAY(monitorNumber), SDL_WINDOWPOS_CENTERED_DISPLAY(monitorNumber), (int) windowSize.x,
                                              (int) windowSize.y, windowFlags)) != NULL, "Failed to create window: %s", SDL_GetError());
        SEC_ASSERT((renderer = SDL_CreateRenderer(window, -1, !BE_Renderer_IsSoftwareMode() ? SDL_RENDERER_ACCELERATED : SDL_RENDERER_SOFTWARE)) != NULL, "Failed to create renderer: %s", SDL_GetError());

        title = (char*) windowTitle;
        size = windowSize;
        position.x = SDL_WINDOWPOS_CENTERED;
        position.y = SDL_WINDOWPOS_CENTERED;
#else
        SEC_LOG_INFO("SDL has been disabled, skipping window creation.");
#endif
    }

    void BE_Window_Destroy(void) {
        if (window == NULL)
            return;

#ifndef BACON_ENGINE_DISABLE_SDL
        SEC_LOGGER_INFO("Destroying window");
        SDL_DestroyWindow(window);
#endif
    }

    SDL_Window* BE_Window_GetInternalSDLWindow(void) {
        return window;
    }

    SDL_Renderer* BE_Window_GetInternalSDLRenderer(void) {
        return renderer;
    }

    const char* BE_Window_GetTitle(void) {
        return title;
    }

    BE_Vector_2U BE_Window_GetSize(void) {
        int x = 0;
        int y = 0;

#ifndef BACON_ENGINE_DISABLE_SDL
        SDL_GetWindowSize(window, &x, &y);
#endif

        return (BE_Vector_2U) {(unsigned) x, (unsigned) y};
    }

    BE_Vector_2U BE_Window_GetPosition(void) {
        return position;
    }

    int BE_Window_IsVisible(void) {
#ifndef BACON_ENGINE_DISABLE_SDL
        return (SDL_GetWindowFlags(window) & SDL_WINDOW_SHOWN) != 0;
#else
        return 0;
#endif
    }

    int BE_Window_IsStillOpened(void) {
        return window != NULL;
    }

    void BE_Window_SetTitle(const char* newTitle) {
        if (window == NULL || title == newTitle)
            return;

#ifndef BACON_ENGINE_DISABLE_SDL
        if (newTitle == NULL)
            newTitle = "";

        SEC_LOGGER_DEBUG("Changing title\nOld Title: %s\nNew Title: %s.", title, newTitle);

        title = (char*) newTitle;

        SDL_SetWindowTitle(window, newTitle);
#endif
    }

    void BE_Window_SetSize(BE_Vector_2U newSize) {
        if (window == NULL || (size.x == newSize.x && size.y == newSize.y))
            return;

#ifndef BACON_ENGINE_DISABLE_SDL
        SEC_LOGGER_DEBUG("Changing size\nWindow: %s\nOld Size: (%u, %u)\nNew Size: (%u, %u)", title, size.x, size.y, newSize.x, newSize.y);

        size = newSize;

        SDL_SetWindowSize(window, (int) newSize.x, (int) newSize.y);
#endif
    }

    void BE_Window_SetWidth(unsigned newWidth) {
        BE_Window_SetSize((BE_Vector_2U) {newWidth, size.y});
    }

    void BE_Window_SetHeight(unsigned newHeight) {
        BE_Window_SetSize((BE_Vector_2U) {size.x, newHeight});
    }

    void BE_Window_SetPosition(BE_Vector_2U newPosition) {
        if (window == NULL || (position.x == newPosition.x && position.y == newPosition.y))
            return;

#ifndef BACON_ENGINE_DISABLE_SDL
        SEC_LOGGER_DEBUG("Changing position\nWindow: %s\nOld Position: (%u, %u)\nNew Position: (%u, %u)", title, position.x, position.y, newPosition.x, newPosition.y);

        position = newPosition;

        SDL_SetWindowSize(window, (int) newPosition.x, (int) newPosition.y);
#endif
    }

    void BE_Window_SetXPosition(unsigned int newX) {
        BE_Window_SetPosition((BE_Vector_2U) {newX, position.y});
    }

    void BE_Window_SetYPosition(unsigned int newY) {
        BE_Window_SetPosition((BE_Vector_2U) {position.x, newY});
    }

    void BE_Window_SetVisibility(int visible) {
        if (window == NULL || BE_Window_IsVisible() == visible)
            return;

#ifndef BACON_ENGINE_DISABLE_SDL
        SEC_LOGGER_DEBUG("Setting window to %s", visible ? "visible" : "invisible");

        if (visible)
            SDL_ShowWindow(window);
        else
            SDL_HideWindow(window);
#endif
    }
SEC_CPP_GUARD_END()
