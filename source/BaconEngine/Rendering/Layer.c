#include <SharedEngineCode/Internal/CppHeader.h>
#include <string.h>

#include "BaconEngine/Rendering/Layer.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Storage/DynamicArray.h"
#include "EngineLayers.h"

CPP_GUARD_START()
    typedef struct {
        BE_ClientLayer publicLayer;
        BE_ClientLayerFunctions functions;
        int calledStart;
    } InternalClientLayer;

    BE_DynamicArray layerArray;
    int initialized = 0;

    int LayerNoOperation(void) {
        return 0;
    }

    InternalClientLayer* InternalGetLayer(const char* name) {
        for (int i = 0; i < (int) layerArray.used; i++) {
            if (strcmp(BE_ARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i)->publicLayer.name, name) != 0)
                continue;

            return layerArray.internalArray[i];
        }

        return NULL;
    }

    void BE_InitializeLayers(void) {
        BE_STRICT_CHECK_NO_RETURN_VALUE(!initialized, "Already initialized the layer stack");
        SEC_LOG_INFO("Initializing layer stack");

        initialized = 1;

        BE_CreateDynamicArray(&layerArray, 100);
        SEC_LOG_INFO("Registering engine layers");
        InitializeEngineLayers();
    }

    void BE_RegisterLayer(const char* name, int enabled, BE_ClientLayerFunctions functions) {
        for (int i = 0; i < (int) layerArray.used; i++)
            BE_STRICT_CHECK_NO_RETURN_VALUE(strcmp(BE_ARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i)->publicLayer.name, name) != 0, "The layer '%s' is already registered", name);

        InternalClientLayer* layer;

        BE_ASSERT_MALLOC(layer, sizeof(InternalClientLayer), "a layer");

        layer->publicLayer = (BE_ClientLayer) {
            .valid = 1,
            .name = name,
            .enabled = enabled
        };
        layer->calledStart = 0;
        layer->functions.LayerOnStart = functions.LayerOnStart != NULL ? functions.LayerOnStart : (void (*)(void)) &LayerNoOperation;
        layer->functions.LayerOnUpdate = functions.LayerOnUpdate != NULL ? functions.LayerOnUpdate : (void (*)(BE_LayerUpdateTypes, double)) &LayerNoOperation;
        layer->functions.LayerOnToggle = functions.LayerOnToggle != NULL ? functions.LayerOnToggle : (void (*)(int)) &LayerNoOperation;
        layer->functions.LayerOnEvent = functions.LayerOnEvent != NULL ? functions.LayerOnEvent : (int (*)(BE_Event)) &LayerNoOperation;
        layer->functions.LayerOnStop = functions.LayerOnStop != NULL ? functions.LayerOnStop : (void (*)(void)) &LayerNoOperation;

        BE_ArrayAddElementToLast(&layerArray, (void *) layer);
    }

    BE_ClientLayer BE_GetLayer(const char* name) {
        BE_STRICT_CHECK(initialized, (BE_ClientLayer) {.valid=0}, "Layers are not initialized");

        InternalClientLayer* foundLayer = InternalGetLayer(name);

        if (foundLayer == NULL)
            return (BE_ClientLayer) {
                .valid = 0
            };

        return foundLayer->publicLayer;
    }

    int BE_GetLayerAmount(void) {
        return layerArray.used;
    }

    int BE_GetAllocatedLayersAmount(void) {
        return (int) layerArray.size;
    }

    int BE_GetLayerReallocationAmount(void) {
        return layerArray.calledRealloc;
    }

    int BE_ToggleLayer(const char* name, int enable) {
        InternalClientLayer* layer = InternalGetLayer(name);

        if (layer == NULL || layer->publicLayer.enabled == enable)
            return 0;

        SEC_LOG_DEBUG("%s %s", enable ? "Enabling" : "Disabling", name);

        layer->publicLayer.enabled = enable;

        layer->functions.LayerOnToggle(enable); // FIXME: The layer could still be in the middle of an update/event.

        return 1;
    }

    void BE_LayerOnUpdate(BE_LayerUpdateTypes layerUpdateType, double deltaTime) { // TODO: Make this a little bit more DRY.
        for (int i = 0; i < layerArray.used; i++) {
            InternalClientLayer* layer = BE_ARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i);

            if (!layer->publicLayer.enabled)
                continue;

            if (!layer->calledStart)
                layer->functions.LayerOnStart();

            layer->functions.LayerOnUpdate(layerUpdateType, deltaTime);

        }
    }

    int BE_LayerOnEvent(SDL_Event sdlEvent) {
        BE_Event event;

        if (BE_ConvertSDLToEngineEvent(&event, sdlEvent)) {
            for (int i = 0; i < layerArray.used; i++) {
                InternalClientLayer* layer = BE_ARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i);

                if (!layer->publicLayer.enabled)
                    continue;

                if (!layer->calledStart) {
                    layer->functions.LayerOnStart();

                    layer->calledStart = 1;
                }

                int result = layer->functions.LayerOnEvent(event);

                if (result)
                    SEC_LOG_TRACE("%s layer stopped the event line", layer->publicLayer.name);

                if (result)
                    return 1;
            }
        }

        return 0;
    }

    void BE_DestroyLayers(void) {
        BE_STRICT_CHECK_NO_RETURN_VALUE(initialized, "Layers are already destroyed");
        SEC_LOG_INFO("Destroying layer stack");

        initialized = 0;

        for (int i = 0; i < layerArray.used; i++) {
            InternalClientLayer* layer = BE_ARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i);

            layer->functions.LayerOnStop();
            free(layer);
        }

        free(layerArray.internalArray);
    }
CPP_GUARD_END()
