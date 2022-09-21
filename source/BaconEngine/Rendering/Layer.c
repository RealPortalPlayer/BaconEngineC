#include <SharedEngineCode/Internal/CppHeader.h>
#include <BaconEngine/Storage/DynamicArray.h>
#include <string.h>

#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Rendering/Layer.h"
#include "EngineLayers.h"
#include "../EngineMemory.h"
#include "BaconEngine/ClientInformation.h"
#include "BaconEngine/Rendering/Renderer.h"

SEC_CPP_GUARD_START()
    typedef struct {
        BE_Layer publicLayer;
        BE_Layer_Functions functions;
        int calledStart;
        int enabled;
    } InternalClientLayer;

    BE_DynamicArray layerArray;
    int layerInitialized = 0;

    int LayerNoOperation(void) {
        return 0;
    }

    InternalClientLayer* InternalGetLayer(const char* name) {
        if (layerInitialized) {
            for (int i = 0; i < (int) layerArray.used; i++) {
                if (strcmp(BE_DYNAMICARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i)->publicLayer.name, name) != 0)
                    continue;

                return layerArray.internalArray[i];
            }
        }

        return NULL;
    }

    void BE_Layer_InitializeLayers(void) {
        BE_STRICTMODE_CHECK_NO_RETURN_VALUE(!layerInitialized, "Already initialized the layer stack");
        SEC_LOGGER_INFO("Initializing layer stack");

        layerInitialized = 1;

        BE_DynamicArray_Create(&layerArray, 100);
        SEC_LOGGER_INFO("Registering engine layers");
        BE_EngineLayers_Initialize();
    }

    void BE_Layer_Register(const char* name, int enabled, BE_Layer_Functions functions) {
        if (!layerInitialized)
            return;

        for (int i = 0; i < (int) layerArray.used; i++)
            BE_STRICTMODE_CHECK_NO_RETURN_VALUE(strcmp(BE_DYNAMICARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i)->publicLayer.name, name) != 0, "The layer '%s' is already registered", name);

        InternalClientLayer* layer = BE_EngineMemory_AllocateMemory(sizeof(InternalClientLayer), BE_ENGINEMEMORY_MEMORY_TYPE_LAYER);

        layer->publicLayer = (BE_Layer) {
            1,
            name
        };
        layer->enabled = enabled;
        layer->calledStart = 0;
        layer->functions.OnStart = functions.OnStart != NULL ? functions.OnStart : (void (*)(void)) &LayerNoOperation;
        layer->functions.OnUpdate = functions.OnUpdate != NULL ? functions.OnUpdate : (void (*)(BE_Layer_UpdateTypes, double)) &LayerNoOperation;
        layer->functions.OnToggle = functions.OnToggle != NULL ? functions.OnToggle : (void (*)(int)) &LayerNoOperation;
        layer->functions.OnEvent = functions.OnEvent != NULL ? functions.OnEvent : (int (*)(BE_Event)) &LayerNoOperation;
        layer->functions.OnStop = functions.OnStop != NULL ? functions.OnStop : (void (*)(void)) &LayerNoOperation;

        BE_DynamicArray_AddElementToLast(&layerArray, (void *) layer);
    }

    BE_Layer BE_Layer_Get(const char* name) {
        BE_STRICTMODE_CHECK(layerInitialized, (BE_Layer) {0}, "Layers are not initialized");

        InternalClientLayer* foundLayer = InternalGetLayer(name);

        if (foundLayer == NULL)
            return (BE_Layer) {
                .valid = 0
            };

        return foundLayer->publicLayer;
    }

    int BE_Layer_GetAmount(void) {
        return layerInitialized ? layerArray.used : 0;
    }

    int BE_Layer_GetAllocatedLayersAmount(void) {
        return layerInitialized ? (int) layerArray.size : 0;
    }

    int BE_Layer_GetLayersReallocationAmount(void) {
        return layerInitialized ? layerArray.calledRealloc : 0;
    }

    int BE_Layer_Toggle(const char* name, int enable) {
        InternalClientLayer* layer = InternalGetLayer(name);

        if (layer == NULL)
            return 0;

        if (!layer->calledStart) {
            layer->functions.OnStart();

            layer->calledStart = 1;
        }

        if (layer->enabled == enable)
            return 0;

        SEC_LOGGER_DEBUG("%s %s", enable ? "Enabling" : "Disabling", name);

        layer->enabled = enable;

        layer->functions.OnToggle(enable); // FIXME: The layer could still be in the middle of an update/event.

        return 1;
    }

    void BE_Layer_OnUpdate(BE_Layer_UpdateTypes updateTypes, double deltaTime) {
        if (!layerInitialized)
            return;

        for (int i = 0; i < layerArray.used; i++) {
            InternalClientLayer* layer = BE_DYNAMICARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i);

            if (!layer->enabled)
                continue;

            if (!layer->calledStart) {
                layer->functions.OnStart();

                layer->calledStart = 1;
            }

            layer->functions.OnUpdate(updateTypes, deltaTime);
        }
    }

    int BE_Layer_OnEvent(BE_Event event) {
        if (!layerInitialized)
            return 0;

        for (int i = 0; i < layerArray.used; i++) {
            InternalClientLayer* layer = BE_DYNAMICARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i);

            if (!layer->enabled)
                continue;

            if (!layer->calledStart) {
                layer->functions.OnStart();

                layer->calledStart = 1;
            }

            if (!layer->functions.OnEvent(event))
                continue;

            SEC_LOGGER_TRACE("%s layer stopped the event line", layer->publicLayer.name);
            return 1;
        }

        return 0;
    }

    int BE_Layer_IsToggled(const char* name) {
        InternalClientLayer* layer = InternalGetLayer(name);

        return layer != NULL && layer->enabled;
    }

    void BE_Layer_DestroyLayers(void) {
        if (BE_ClientInformation_IsServerModeEnabled() || BE_Renderer_GetCurrentType() == BE_RENDERER_TYPE_TEXT)
            return;

        BE_STRICTMODE_CHECK_NO_RETURN_VALUE(layerInitialized, "Layers are already destroyed");
        SEC_LOGGER_INFO("Destroying layer stack");

        layerInitialized = 0;

        for (int i = 0; i < layerArray.used; i++) {
            InternalClientLayer* layer = BE_DYNAMICARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i);

            layer->functions.OnStop();
        }

        for (int i = 0; i < layerArray.used; i++)
            BE_EngineMemory_DeallocateMemory(BE_DYNAMICARRAY_GET_ELEMENT(InternalClientLayer, layerArray, i), sizeof(InternalClientLayer), BE_ENGINEMEMORY_MEMORY_TYPE_LAYER);

        BE_EngineMemory_DeallocateMemory(layerArray.internalArray, sizeof(void *) * layerArray.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
    }
SEC_CPP_GUARD_END()
