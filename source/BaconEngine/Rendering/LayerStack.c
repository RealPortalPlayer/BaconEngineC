#include <stddef.h>
#include <stdlib.h>

#include "BaconEngine/Rendering/LayerStack.h"
#include "BaconEngine/Debugging/Assert.h"

#ifdef __cplusplus
extern "C" {
#endif

    static Layer** layers = NULL;
    static int layerIndex = 0;

    void OnToggleDummy(struct Layer* this, int enabled) {}
    void OnStartShutdownDummy(struct Layer* this) {}
    void OnEventDummy(struct Layer* this, SDL_Event event) {}
    void OnRenderDummy(struct Layer* this, double deltaTime) {}

    void InitializeLayers(void) {
        if (layers != NULL)
            return;

        LOG_DEBUG("Initializing layer stack");

        layers = malloc(sizeof(Layer) * MAX_LAYERS);

        ASSERT(layers != NULL, "Failed to allocate %lu bytes of memory for the layer stack", sizeof(Layer) * MAX_LAYERS);
    }

    void DestroyLayers(void) {
        if (layers == NULL)
            return;

        LOG_DEBUG("Destroying layer stack");
        free(layers);

        layers = NULL;
        layerIndex = 0;
    }

    Layer* CreateLayer(const char* name, void (*OnToggle)(struct Layer* this, int enabled),
                       void (*OnStart)(struct Layer* this), void (*OnEvent)(struct Layer* this, SDL_Event event),
                       void (*OnBeforeRender)(struct Layer* this, double deltaTime),
                       void (*OnAfterRender)(struct Layer* this, double deltaTime), void (*OnShutdown)(struct Layer* this)) {
        if (layers == NULL)
            return NULL;

        ASSERT(layerIndex + 1 < sizeof(Layer) * MAX_LAYERS, "Ran out of free layers (%i)", MAX_LAYERS);

        Layer* layer = malloc(sizeof(Layer));

        ASSERT(layer != NULL, "Failed to allocate %lu bytes of memory", sizeof(Layer));

        layer->name = name;
        layer->OnToggle = OnToggle;
        layer->OnStart = OnStart;
        layer->OnEvent = OnEvent;
        layer->OnBeforeRender = OnBeforeRender;
        layer->OnAfterRender = OnAfterRender;
        layer->OnShutdown = OnShutdown;
        layers[layerIndex] = layer;
        layerIndex++;

        return layer;
    }

    int GetLayerCount(void) {
        return layerIndex;
    }

    Layer** GetLayers(void) {
        return layers;
    }

#ifdef __cplusplus
};
#endif