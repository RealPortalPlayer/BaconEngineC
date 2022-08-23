#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/Debugging/Assert.h>
#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/ArgumentHandler.h>

#include "BaconEngine/Rendering/Window.h"
#include "BaconEngine/Rendering/Renderer.h"

CPP_GUARD_START()
    static SDL_Window* window = NULL;
    static SDL_Renderer* renderer = NULL;
    static char* title = NULL;
    static BE_Vector2U size = {0, 0};
    static BE_Vector2U position = {0, 0};

    void BE_InitializeWindow(const char* windowTitle, BE_Vector2U windowSize) {
        (void) windowTitle;
        (void) windowSize;

        if (window != NULL)
            return;

#ifndef BACON_ENGINE_DISABLE_SDL
        SDL_WindowFlags windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;

        switch (BE_GetCurrentRenderer()) {
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
            const char* preParsedMonitor = SEC_GetArgumentValue("--monitor");

            if (preParsedMonitor != NULL) {
                char* error;
                int parsedMonitor = (int) strtol(preParsedMonitor, &error, 0);

                if (error != NULL && strlen(error) != 0) {
                    SEC_LOG_ERROR("Invalid display was supplied, ignoring...");

                    parsedMonitor = 0;
                }

                monitorNumber = parsedMonitor;
            }
        }

        if (!SDL_WasInit(SDL_INIT_VIDEO))
            SEC_ASSERT(SDL_Init(SDL_INIT_VIDEO) == 0, "Failed to initialize SDL: %s", SDL_GetError());

        SEC_LOG_INFO("Creating window\nTitle: %s\nSize: (%u, %u)\nMonitor: %i", windowTitle, windowSize.x, windowSize.y, monitorNumber);
        SEC_ASSERT((window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED_DISPLAY(monitorNumber), SDL_WINDOWPOS_CENTERED_DISPLAY(monitorNumber), (int) windowSize.x,
                                              (int) windowSize.y, windowFlags)) != NULL, "Failed to create window: %s", SDL_GetError());
        SEC_ASSERT((renderer = SDL_CreateRenderer(window, -1, !BE_IsSoftwareRendering() ? SDL_RENDERER_ACCELERATED : SDL_RENDERER_SOFTWARE)) != NULL, "Failed to create renderer: %s", SDL_GetError());

        title = (char*) windowTitle;
        size = windowSize;
        position.x = SDL_WINDOWPOS_CENTERED;
        position.y = SDL_WINDOWPOS_CENTERED;
#else
        SEC_LOG_INFO("SDL has been disabled, skipping window creation.");
#endif
    }

    void BE_DestroyWindow(void) {
        if (window == NULL)
            return;

#ifndef BACON_ENGINE_DISABLE_SDL
        SEC_LOG_INFO("Destroying window");
        SDL_DestroyWindow(window);
#endif
    }

    SDL_Window* BE_GetInternalSDLWindow(void) {
        return window;
    }

    SDL_Renderer* BE_GetInternalSDLRenderer(void) {
        return renderer;
    }

    const char* BE_GetWindowTitle(void) {
        return title;
    }

    BE_Vector2U BE_GetWindowSize(void) {
        int x = 0;
        int y = 0;

#ifndef BACON_ENGINE_DISABLE_SDL
        SDL_GetWindowSize(window, &x, &y);
#endif

        return (BE_Vector2U) {(unsigned) x, (unsigned) y};
    }

    BE_Vector2U BE_GetWindowPosition(void) {
        return position;
    }

    int BE_IsWindowVisible(void) {
#ifndef BACON_ENGINE_DISABLE_SDL
        return (SDL_GetWindowFlags(window) & SDL_WINDOW_SHOWN) != 0;
#else
        return 0;
#endif
    }

    int BE_IsWindowStillOpened(void) {
        return window != NULL;
    }

    void BE_SetWindowTitle(const char* newTitle) {
        if (window == NULL || title == newTitle)
            return;

#ifndef BACON_ENGINE_DISABLE_SDL
        if (newTitle == NULL)
            newTitle = "";

        SEC_LOG_DEBUG("Changing title\nOld Title: %s\nNew Title: %s.", title, newTitle);

        title = (char*) newTitle;

        SDL_SetWindowTitle(window, newTitle);
#endif
    }

    void BE_SetWindowSize(BE_Vector2U newSize) {
        if (window == NULL || (size.x == newSize.x && size.y == newSize.y))
            return;

#ifndef BACON_ENGINE_DISABLE_SDL
        SEC_LOG_DEBUG("Changing size\nWindow: %s\nOld Size: (%u, %u)\nNew Size: (%u, %u)", title, size.x, size.y, newSize.x, newSize.y);

        size = newSize;

        SDL_SetWindowSize(window, (int) newSize.x, (int) newSize.y);
#endif
    }

    void BE_SetWindowWidth(unsigned newWidth) {
        BE_SetWindowSize((BE_Vector2U) {newWidth, size.y});
    }

    void BE_SetWindowHeight(unsigned newHeight) {
        BE_SetWindowSize((BE_Vector2U) {size.x, newHeight});
    }

    void BE_SetWindowPosition(BE_Vector2U newPosition) {
        if (window == NULL || (position.x == newPosition.x && position.y == newPosition.y))
            return;

#ifndef BACON_ENGINE_DISABLE_SDL
        SEC_LOG_DEBUG("Changing position\nWindow: %s\nOld Position: (%u, %u)\nNew Position: (%u, %u)", title, position.x, position.y, newPosition.x, newPosition.y);

        position = newPosition;

        SDL_SetWindowSize(window, (int) newPosition.x, (int) newPosition.y);
#endif
    }

    void BE_SetWindowXPosition(unsigned int newX) {
        BE_SetWindowPosition((BE_Vector2U) {newX, position.y});
    }

    void BE_SetWindowYPosition(unsigned int newY) {
        BE_SetWindowPosition((BE_Vector2U) {position.x, newY});
    }

    void BE_SetWindowVisibility(int visible) {
        if (window == NULL || BE_IsWindowVisible() == visible)
            return;

#ifndef BACON_ENGINE_DISABLE_SDL
        SEC_LOG_DEBUG("Setting window to %s", visible ? "visible" : "invisible");

        if (visible)
            SDL_ShowWindow(window);
        else
            SDL_HideWindow(window);
#endif
    }
CPP_GUARD_END()
