#include <SDL.h>
#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/Window.h"
#include "BaconEngine/Debugging/StrictMode.h"

CPP_GUARD_START()
    Color3U clearColor = {0, 0, 0};
    RendererTypes currentType;

    void SetClearColor(Color3U color) {
        clearColor = color;
    }

    void InitializeRenderer(void) {
        static int initialized = 0;

        STRICT_CHECK_NO_RETURN_VALUE(!initialized, "Already initialized renderer");

        char* value = (char*) GetArgumentValue("--renderer");

        if (value != NULL) {
            {
                int length = 0;

                while (value[length] != '\0') {
                    value[length] = (char) tolower(value[length]);
                    length++;
                }
            }

            if (strcmp(value, "opengl") == 0) {
                currentType = RENDERER_TYPE_OPENGL;
                return;
            }

            if (strcmp(value, "vulkan") == 0) {
                currentType = RENDERER_TYPE_VULKAN;
                return;
            }

            if (strcmp(value, "text") == 0) {
                currentType = RENDERER_TYPE_TEXT;
                return;
            }
        }

        currentType = RENDERER_TYPE_AUTO;
    }

    void ClearScreen(void) {
        if (GetInternalSDLRenderer() == NULL)
            return;

        SDL_SetRenderDrawColor(GetInternalSDLRenderer(), clearColor.r, clearColor.g, clearColor.b, 255);
        SDL_RenderClear(GetInternalSDLRenderer());
    }

    RendererTypes GetCurrentRenderer(void) {
        return currentType;
    }

    int IsSoftwareRendering(void) {
        static int software = -1;

        if (software == -1)
            software = GetArgumentIndex("--software") > -1;

        return software;
    }

    Color3U GetClearColor(void) {
        return clearColor;
    }
CPP_GUARD_END()
