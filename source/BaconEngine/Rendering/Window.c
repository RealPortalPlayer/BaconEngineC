#include <SharedEngineCode/Logger.h>

#include "BaconEngine/Rendering/Window.h"

#ifdef __cplusplus
extern "C" {
#endif

static SDL_Window* window = NULL;
static char* title = NULL;
static Vector2UI size = {};
static Vector2UI position = {};

void InitializeWindow(const char* windowTitle, Vector2UI windowSize) {
    if (window != NULL)
        return;

    LOG_INFO("Creating window!\nTitle: %s\nSize: (%u, %u)", windowTitle, windowSize.x, windowSize.y);

    window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int) windowSize.x, (int) windowSize.y, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
    title = (char*) windowTitle;
    size = windowSize;
    position.x = SDL_WINDOWPOS_CENTERED;
    position.y = SDL_WINDOWPOS_CENTERED;
}

void DestroyWindow() {
    if (window == NULL)
        return;

    LOG_INFO("Destroying window.");

    SDL_DestroyWindow(window);
}

SDL_Window* GetSDLWindow() {
    return window;
}

const char* GetTitle() {
    return title;
}

Vector2UI GetSize() {
    return size;
}

Vector2UI GetPosition() {
    return position;
}

void SetTitle(const char* newTitle) {
    if (window == NULL)
        return;

    LOG_DEBUG("Setting the title from '%s' to '%s'.", title, newTitle);

    title = (char*) newTitle;

    SDL_SetWindowTitle(window, newTitle);
}

void SetSize(Vector2UI newSize) {
    if (window == NULL)
        return;

    LOG_DEBUG("Setting the size from (%u, %u) to (%u, %u).", size.x, size.y, newSize.x, newSize.y);

    size = newSize;

    SDL_SetWindowSize(window, (int) newSize.x, (int) newSize.y);
}

void SetWidth(unsigned int newWidth) {
    SetSize((Vector2UI) {newWidth, size.y});
}

void SetHeight(unsigned int newHeight) {
    SetSize((Vector2UI) {size.x, newHeight});
}

void SetPosition(Vector2UI newPosition) {
    if (window == NULL)
        return;

    LOG_DEBUG("Setting the position from (%u, %u) to (%u, %u).", position.x, position.y, newPosition.x, newPosition.y);

    position = newPosition;

    SDL_SetWindowSize(window, (int) newPosition.x, (int) newPosition.y);
}

void SetX(unsigned int newX) {
    SetPosition((Vector2UI) {newX, position.y});
}

void SetY(unsigned int newY) {
    SetPosition((Vector2UI) {position.x, newY});
}

#ifdef __cplusplus
};
#endif