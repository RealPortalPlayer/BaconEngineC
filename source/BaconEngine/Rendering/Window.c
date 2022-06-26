#include <SharedEngineCode/Logger.h>
#include <BaconEngine/Debugging/Assert.h>

#include "BaconEngine/Rendering/Window.h"

#ifdef __cplusplus
extern "C" {
#endif

    static SDL_Window* window = NULL;
    static SDL_Renderer* renderer = NULL;
    static char* title = NULL;
    static Vector2U size = {};
    static Vector2U position = {};

    void InitializeWindow(const char* windowTitle, Vector2U windowSize) {
        if (window != NULL)
            return;

        ASSERT(SDL_Init(SDL_INIT_VIDEO) == 0, "Failed to initialize SDL: %s", SDL_GetError());
        LOG_INFO("Creating window\nTitle: %s\nSize: (%u, %u)", windowTitle, windowSize.x, windowSize.y);
        ASSERT((window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int) windowSize.x,
                                         (int) windowSize.y, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN)) != NULL, "Failed to create window: %s", SDL_GetError());
        ASSERT((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) != NULL, "Failed to create renderer: %s", SDL_GetError());

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

    SDL_Window* GetSDLWindow(void) {
        return window;
    }

    SDL_Renderer* GetSDLRenderer(void) {
        return renderer;
    }

    const char* GetTitle(void) {
        return title;
    }

    Vector2U GetSize(void) {
        return size;
    }

    Vector2U GetPosition(void) {
        return position;
    }

    int IsWindowVisible(void) {
        return (SDL_GetWindowFlags(window) & SDL_WINDOW_SHOWN) != 0;
    }

    int IsWindowStillOpened(void) {
        return window != NULL;
    }

    void SetTitle(const char* newTitle) {
        if (window == NULL || title == newTitle)
            return;

        if (newTitle == NULL)
            newTitle = "";

        LOG_DEBUG("Changing title\nOld Title: %s\nNew Title: %s.", title, newTitle);

        title = (char*) newTitle;

        SDL_SetWindowTitle(window, newTitle);
    }

    void SetSize(Vector2U newSize) {
        if (window == NULL || size.x == newSize.x && size.y == newSize.y)
            return;

        LOG_DEBUG("Changing size\nWindow: %s\nOld Size: (%u, %u)\nNew Size: (%u, %u)", title, size.x, size.y, newSize.x, newSize.y);

        size = newSize;

        SDL_SetWindowSize(window, (int) newSize.x, (int) newSize.y);
    }

    void SetWidth(unsigned int newWidth) {
        SetSize((Vector2U) {newWidth, size.y});
    }

    void SetHeight(unsigned int newHeight) {
        SetSize((Vector2U) {size.x, newHeight});
    }

    void SetPosition(Vector2U newPosition) {
        if (window == NULL || position.x == newPosition.x && position.y == newPosition.y)
            return;

        LOG_DEBUG("Changing position\nWindow: %s\nOld Position: (%u, %u)\nNew Position: (%u, %u)", title, position.x, position.y, newPosition.x, newPosition.y);

        position = newPosition;

        SDL_SetWindowSize(window, (int) newPosition.x, (int) newPosition.y);
    }

    void SetX(unsigned int newX) {
        SetPosition((Vector2U) {newX, position.y});
    }

    void SetY(unsigned int newY) {
        SetPosition((Vector2U) {position.x, newY});
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

#ifdef __cplusplus
};
#endif