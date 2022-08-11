#include <SharedEngineCode/Logger.h>
#include <BaconEngine/Debugging/Assert.h>
#include <SharedEngineCode/Internal/CppHeader.h>
#include <SDL_ttf.h>

#include "BaconEngine/Rendering/Window.h"
#include "BaconEngine/Rendering/Renderer.h"

CPP_GUARD_START()
    static SDL_Window* window = NULL;
    static SDL_Renderer* renderer = NULL;
    static char* title = NULL;
    static Vector2U size = {0, 0};
    static Vector2U position = {0, 0};

    void InitializeWindow(const char* windowTitle, Vector2U windowSize) {
        if (window != NULL)
            return;

        SDL_WindowFlags windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;

        switch (GetCurrentRenderer()) {
            case RENDERER_TYPE_AUTO:
                break;

            case RENDERER_TYPE_OPENGL:
                windowFlags |= SDL_WINDOW_OPENGL;
                break;

            case RENDERER_TYPE_VULKAN:
                windowFlags |= SDL_WINDOW_VULKAN;
                break;

            case RENDERER_TYPE_TEXT:
                return;
        }

        if (!SDL_WasInit(SDL_INIT_VIDEO))
            ASSERT(SDL_Init(SDL_INIT_VIDEO) == 0, "Failed to initialize SDL: %s", SDL_GetError());

        LOG_INFO("Creating window\nTitle: %s\nSize: (%u, %u)", windowTitle, windowSize.x, windowSize.y);
        ASSERT((window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int) windowSize.x,
                                         (int) windowSize.y, windowFlags)) != NULL, "Failed to create window: %s", SDL_GetError());
        ASSERT((renderer = SDL_CreateRenderer(window, -1, !IsSoftwareRendering() ? SDL_RENDERER_ACCELERATED : SDL_RENDERER_SOFTWARE)) != NULL, "Failed to create renderer: %s", SDL_GetError());

        title = (char*) windowTitle;
        size = windowSize;
        position.x = SDL_WINDOWPOS_CENTERED;
        position.y = SDL_WINDOWPOS_CENTERED;
    }

    void DestroyWindow(void) {
        if (window == NULL)
            return;

        LOG_INFO("Destroying window");
        SDL_DestroyWindow(window);
    }

    SDL_Window* GetInternalSDLWindow(void) {
        return window;
    }

    SDL_Renderer* GetInternalSDLRenderer(void) {
        return renderer;
    }

    const char* GetWindowTitle(void) {
        return title;
    }

    Vector2U GetWindowSize(void) {
        int x;
        int y;

        SDL_GetWindowSize(window, &x, &y);

        return (Vector2U) {(unsigned) x, (unsigned) y};
    }

    Vector2U GetWindowPosition(void) {
        return position;
    }

    int IsWindowVisible(void) {
        return (SDL_GetWindowFlags(window) & SDL_WINDOW_SHOWN) != 0;
    }

    int IsWindowStillOpened(void) {
        return window != NULL;
    }

    void SetWindowTitle(const char* newTitle) {
        if (window == NULL || title == newTitle)
            return;

        if (newTitle == NULL)
            newTitle = "";

        LOG_DEBUG("Changing title\nOld Title: %s\nNew Title: %s.", title, newTitle);

        title = (char*) newTitle;

        SDL_SetWindowTitle(window, newTitle);
    }

    void SetWindowSize(Vector2U newSize) {
        if (window == NULL || (size.x == newSize.x && size.y == newSize.y))
            return;

        LOG_DEBUG("Changing size\nWindow: %s\nOld Size: (%u, %u)\nNew Size: (%u, %u)", title, size.x, size.y, newSize.x, newSize.y);

        size = newSize;

        SDL_SetWindowSize(window, (int) newSize.x, (int) newSize.y);
    }

    void SetWindowWidth(unsigned int newWidth) {
        SetWindowSize((Vector2U) {newWidth, size.y});
    }

    void SetWindowHeight(unsigned int newHeight) {
        SetWindowSize((Vector2U) {size.x, newHeight});
    }

    void SetWindowPosition(Vector2U newPosition) {
        if (window == NULL || (position.x == newPosition.x && position.y == newPosition.y))
            return;

        LOG_DEBUG("Changing position\nWindow: %s\nOld Position: (%u, %u)\nNew Position: (%u, %u)", title, position.x, position.y, newPosition.x, newPosition.y);

        position = newPosition;

        SDL_SetWindowSize(window, (int) newPosition.x, (int) newPosition.y);
    }

    void SetWindowXPosition(unsigned int newX) {
        SetWindowPosition((Vector2U) {newX, position.y});
    }

    void SetWindowYPosition(unsigned int newY) {
        SetWindowPosition((Vector2U) {position.x, newY});
    }

    void SetWindowVisibility(int visible) {
        if (window == NULL || IsWindowVisible() == visible)
            return;

        LOG_DEBUG("Setting window to %s", visible ? "visible" : "invisible");

        if (visible)
            SDL_ShowWindow(window);
        else
            SDL_HideWindow(window);
    }
CPP_GUARD_END()
