#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/Debugging/StrictMode.h>
#include <SharedEngineCode/Debugging/Assert.h>
#include <SharedEngineCode/Storage/DynamicArray.h>
#include <SharedEngineCode/EngineMemory.h>

#include "BaconEngine/Rendering/Layer.h"
#include "EngineLayers.h"

SEC_CPP_GUARD_START()
    typedef struct { // TODO: BE_Layer_IsToggled
        BE_Layer publicLayer;
        BE_Layer_Functions functions;
        int calledStart;
    } InternalClientLayer;

    SEC_DynamicArray layerArray;
    int layerInitialized = 0;

    int LayerNoOperation(void) {
        return 0;
    }

    InternalClientLayer* InternalGetLayer(const char* name) {
        for (int i = 0; i < (int) layerArray.used; i++) {
            if (strcmp(SEC_DYNAMICARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i)->publicLayer.name, name) != 0)
                continue;

            return layerArray.internalArray[i];
        }

        return NULL;
    }

    void BE_Layer_InitializeLayers(void) {
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(!layerInitialized, "Already initialized the layer stack");
        SEC_LOGGER_INFO("Initializing layer stack");

        layerInitialized = 1;

        SEC_DynamicArray_Create(&layerArray, 100);
        SEC_LOGGER_INFO("Registering engine layers");
        InitializeEngineLayers();
    }

    void BE_Layer_Register(const char* name, int enabled, BE_Layer_Functions functions) {
        for (int i = 0; i < (int) layerArray.used; i++)
            SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(strcmp(SEC_DYNAMICARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i)->publicLayer.name, name) != 0, "The layer '%s' is already registered", name);

        InternalClientLayer* layer;

        SEC_ASSERT_MALLOC(layer, sizeof(InternalClientLayer), "a layer");

        layer->publicLayer = (BE_Layer) {
            .valid = 1,
            .name = name,
            .enabled = enabled
        };
        layer->calledStart = 0;
        layer->functions.OnStart = functions.OnStart != NULL ? functions.OnStart : (void (*)(void)) &LayerNoOperation;
        layer->functions.OnUpdate = functions.OnUpdate != NULL ? functions.OnUpdate : (void (*)(BE_Layer_UpdateTypes, double)) &LayerNoOperation;
        layer->functions.OnToggle = functions.OnToggle != NULL ? functions.OnToggle : (void (*)(int)) &LayerNoOperation;
        layer->functions.OnEvent = functions.OnEvent != NULL ? functions.OnEvent : (int (*)(BE_Event)) &LayerNoOperation;
        layer->functions.OnStop = functions.OnStop != NULL ? functions.OnStop : (void (*)(void)) &LayerNoOperation;

        SEC_DynamicArray_AddElementToLast(&layerArray, (void *) layer);
    }

    BE_Layer BE_Layer_Get(const char* name) {
        SEC_STRICTMODE_CHECK(layerInitialized, (BE_Layer) {.valid=0}, "Layers are not initialized");

        InternalClientLayer* foundLayer = InternalGetLayer(name);

        if (foundLayer == NULL)
            return (BE_Layer) {
                .valid = 0
            };

        return foundLayer->publicLayer;
    }

    int BE_Layer_GetAmount(void) {
        return layerArray.used;
    }

    int BE_Layer_GetAllocatedLayersAmount(void) {
        return (int) layerArray.size;
    }

    int BE_Layer_GetLayersReallocationAmount(void) {
        return layerArray.calledRealloc;
    }

    int BE_Layer_Toggle(const char* name, int enable) {
        InternalClientLayer* layer = InternalGetLayer(name);

        if (layer == NULL || layer->publicLayer.enabled == enable)
            return 0;

        SEC_LOGGER_DEBUG("%s %s", enable ? "Enabling" : "Disabling", name);

        layer->publicLayer.enabled = enable;

        layer->functions.OnToggle(enable); // FIXME: The layer could still be in the middle of an update/event.

        return 1;
    }

    void BE_Layer_OnUpdate(BE_Layer_UpdateTypes updateTypes, double deltaTime) { // TODO: Make this a little bit more DRY.
        for (int i = 0; i < layerArray.used; i++) {
            InternalClientLayer* layer = SEC_DYNAMICARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i);

            if (!layer->publicLayer.enabled)
                continue;

            if (!layer->calledStart)
                layer->functions.OnStart();

            layer->functions.OnUpdate(updateTypes, deltaTime);

        }
    }

    int BE_Layer_OnEvent(SDL_Event sdlEvent) {
        BE_Event event;

        if (BE_Event_ConvertSDLToEngineEvent(&event, sdlEvent)) {
            for (int i = 0; i < layerArray.used; i++) {
                InternalClientLayer* layer = SEC_DYNAMICARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i);

                if (!layer->publicLayer.enabled)
                    continue;

                if (!layer->calledStart) {
                    layer->functions.OnStart();

                    layer->calledStart = 1;
                }

                int result = layer->functions.OnEvent(event);

                if (result)
                    SEC_LOGGER_TRACE("%s layer stopped the event line", layer->publicLayer.name);

                if (result)
                    return 1;
            }
        }

        return 0;
    }

    void BE_Layer_DestroyLayers(void) {
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(layerInitialized, "Layers are already destroyed");
        SEC_LOGGER_INFO("Destroying layer stack");

        layerInitialized = 0;

        for (int i = 0; i < layerArray.used; i++) {
            InternalClientLayer* layer = SEC_DYNAMICARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i);

            layer->functions.OnStop();
            SEC_EngineMemory_RemoveAllocated(sizeof(BE_Layer));
            free(layer);
        }

        SEC_EngineMemory_RemoveAllocated(sizeof(void *) * layerArray.size);
        free(layerArray.internalArray);
    }
SEC_CPP_GUARD_END()
