#include <SharedEngineCode/Internal/CppHeader.h>
#include <string.h>

#include "BaconEngine/Rendering/Layer.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Storage/DynamicArray.h"
#include "EngineLayers.h"

CPP_GUARD_START()
    typedef struct {
        ClientLayer publicLayer;
        ClientLayerFunctions functions;
        int calledStart;
    } InternalClientLayer;

    DynamicArray layerArray;
    int initialized = 0;

    int LayerNoOperation(void) {
        return 0;
    }

    InternalClientLayer* InternalGetLayer(const char* name) {
        for (int i = 0; i < (int) layerArray.used; i++) {
            if (strcmp(GET_ELEMENT(InternalClientLayer, layerArray, i)->publicLayer.name, name) != 0)
                continue;

            return layerArray.internalArray[i];
        }

        return NULL;
    }

    void InitializeLayers(void) {
        STRICT_CHECK_NO_RETURN_VALUE(!initialized, "Already initialized the layer stack");
        LOG_INFO("Initializing layer stack");

        initialized = 1;

        CreateDynamicArray(&layerArray, 100);
        LOG_INFO("Registering engine layers");
        InitializeEngineLayers();
    }

    void RegisterLayer(const char* name, int enabled, ClientLayerFunctions functions) {
        for (int i = 0; i < (int) layerArray.used; i++)
            STRICT_CHECK_NO_RETURN_VALUE(strcmp(GET_ELEMENT(InternalClientLayer, layerArray, i)->publicLayer.name, name) != 0, "The layer '%s' is already registered", name);

        InternalClientLayer* layer = malloc(sizeof(InternalClientLayer));

        ASSERT(layer != NULL, "Failed to allocate %lu bytes of data for a layer", sizeof(InternalClientLayer));

        layer->publicLayer = (ClientLayer) {
            .valid = 1,
            .name = name,
            .enabled = enabled
        };

        layer->calledStart = 0;
        layer->functions.LayerOnStart = functions.LayerOnStart != NULL ? functions.LayerOnStart : (void (*)(void)) &LayerNoOperation;
        layer->functions.LayerOnUpdate = functions.LayerOnUpdate != NULL ? functions.LayerOnUpdate : (void (*)(LayerUpdateTypes, double)) &LayerNoOperation;
        layer->functions.LayerOnToggle = functions.LayerOnToggle != NULL ? functions.LayerOnToggle : (void (*)(int)) &LayerNoOperation;
        layer->functions.LayerOnEvent = functions.LayerOnEvent != NULL ? functions.LayerOnEvent : (int (*)(SDL_Event)) &LayerNoOperation;
        layer->functions.LayerOnStop = functions.LayerOnStop != NULL ? functions.LayerOnStop : (void (*)(void)) &LayerNoOperation;

        ArrayPushElement(&layerArray, (void*) layer);
    }

    ClientLayer GetLayer(const char* name) {
        STRICT_CHECK(initialized, (ClientLayer) {.valid=0}, "Layers are not initialized");

        InternalClientLayer* foundLayer = InternalGetLayer(name);

        if (foundLayer == NULL)
            return (ClientLayer) {
                .valid = 0
            };

        return foundLayer->publicLayer;
    }

    int ToggleLayer(const char* name, int enable) {
        InternalClientLayer* layer = InternalGetLayer(name);

        if (layer == NULL || layer->publicLayer.enabled == enable)
            return 0;

        LOG_DEBUG("%s %s", enable ? "Enabling" : "Disabling", name);

        layer->publicLayer.enabled = enable;

        layer->functions.LayerOnToggle(enable);

        return 1;
    }

    void LayerOnUpdate(LayerUpdateTypes layerUpdateType, double deltaTime) { // TODO: Make this a little bit more DRY.
        for (int i = 0; i < layerArray.used; i++) {
            InternalClientLayer* layer = GET_ELEMENT(InternalClientLayer, layerArray, i);

            if (!layer->publicLayer.enabled)
                continue;

            if (!layer->calledStart) {
                layer->functions.LayerOnStart();

                layer->calledStart = 1;
            }

            layer->functions.LayerOnUpdate(layerUpdateType, deltaTime);
        }
    }

    int LayerOnEvent(SDL_Event event) {
        for (int i = 0; i < layerArray.used; i++) {
            InternalClientLayer* layer = GET_ELEMENT(InternalClientLayer, layerArray, i);

            if (!layer->publicLayer.enabled)
                continue;

            if (!layer->calledStart) {
                layer->functions.LayerOnStart();

                layer->calledStart = 1;
            }

            if (!layer->functions.LayerOnEvent(event))
                continue;

            LOG_TRACE("%s layer stopped the event line", layer->publicLayer.name);

            return 1;
        }

        return 0;
    }

    void DestroyLayers(void) {
        STRICT_CHECK_NO_RETURN_VALUE(initialized, "Layers are already destroyed");
        LOG_INFO("Destroying layer stack");

        initialized = 0;

        for (int i = 0; i < layerArray.used; i++) {
            InternalClientLayer* layer = GET_ELEMENT(InternalClientLayer, layerArray, i);

            layer->functions.LayerOnStop();
            free(layer);
        }

        free(layerArray.internalArray);
    }
CPP_GUARD_END()
