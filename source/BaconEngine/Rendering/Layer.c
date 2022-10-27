// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconEngine/Storage/DynamicArray.h>
#include <string.h>

#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Rendering/Layer.h"
#include "EngineLayers.h"
#include "../EngineMemory.h"
#include "PrivateLayer.h"
#include "BaconEngine/Rendering/Renderer.h"

SEC_CPP_SUPPORT_GUARD_START()
typedef struct {
    BE_Layer publicLayer;
    BE_Layer_Functions functions;
    SEC_Boolean calledStart;
    SEC_Boolean enabled;
} BE_Layer_Internal;

BE_DynamicArray beLayerArray;
SEC_Boolean beLayerInitialized = SEC_FALSE;

int BE_Layer_NoOperation(void) {
    return 0;
}

BE_Layer_Internal* BE_Layer_InternalGet(const char* name) {
    for (int i = 0; i < (int) beLayerArray.used; i++) {
        if (strcmp(BE_DYNAMICARRAY_GET_ELEMENT(BE_Layer_Internal, beLayerArray, i)->publicLayer.name, name) != 0)
            continue;

        return beLayerArray.internalArray[i];
    }

    return NULL;
}

void BE_PrivateLayer_InitializeLayers(void) {
    if (BE_Renderer_GetCurrentType() == BE_RENDERER_TYPE_TEXT)
        return;

    BE_ASSERT(!beLayerInitialized, "Already initialized the layer stack\n");
    SEC_LOGGER_INFO("Initializing layer stack\n");

    beLayerInitialized = SEC_TRUE;

    BE_DynamicArray_Create(&beLayerArray, 100);
    SEC_LOGGER_INFO("Registering engine layers\n");
    BE_EngineLayers_Initialize();
}

void BE_Layer_Register(const char* name, SEC_Boolean enabled, BE_Layer_Functions functions) {
    if (BE_Renderer_GetCurrentType() == BE_RENDERER_TYPE_TEXT)
        return;

    for (int i = 0; i < (int) beLayerArray.used; i++)
        BE_STRICTMODE_CHECK_NO_RETURN_VALUE(strcmp(BE_DYNAMICARRAY_GET_ELEMENT(BE_Layer_Internal, beLayerArray, i)->publicLayer.name, name) != 0, "The layer '%s' is already registered\n", name);

    BE_Layer_Internal* layer = (BE_Layer_Internal*) BE_EngineMemory_AllocateMemory(sizeof(BE_Layer_Internal), BE_ENGINEMEMORY_MEMORY_TYPE_LAYER);

    layer->publicLayer = (BE_Layer) {
        SEC_TRUE,
        name
    };
    layer->enabled = enabled;
    layer->calledStart = SEC_FALSE;
    layer->functions.OnStart = functions.OnStart != NULL ? functions.OnStart : (void (*)(void)) &BE_Layer_NoOperation;
    layer->functions.OnUpdate = functions.OnUpdate != NULL ? functions.OnUpdate : (void (*)(BE_Layer_UpdateTypes)) &BE_Layer_NoOperation;
    layer->functions.OnToggle = functions.OnToggle != NULL ? functions.OnToggle : (void (*)(int)) &BE_Layer_NoOperation;
    layer->functions.OnEvent = functions.OnEvent != NULL ? functions.OnEvent : (int (*)(BE_Event)) &BE_Layer_NoOperation;
    layer->functions.OnStop = functions.OnStop != NULL ? functions.OnStop : (void (*)(void)) &BE_Layer_NoOperation;

    BE_DynamicArray_AddElementToLast(&beLayerArray, (void *) layer);
}

BE_Layer BE_Layer_Get(const char* name) {
    BE_Layer_Internal* foundLayer = BE_Layer_InternalGet(name);

    return foundLayer != NULL ? foundLayer->publicLayer : SEC_CPP_SUPPORT_CREATE_STRUCT(BE_Layer, .valid = 0);
}

int BE_Layer_GetAmount(void) {
    return beLayerInitialized ? beLayerArray.used : 0;
}

int BE_Layer_GetAllocatedLayersAmount(void) {
    return beLayerInitialized ? (int) beLayerArray.size : 0;
}

int BE_Layer_GetLayersReallocationAmount(void) {
    return beLayerInitialized ? beLayerArray.calledRealloc : 0;
}

SEC_Boolean BE_Layer_Toggle(const char* name, SEC_Boolean enable) {
    BE_Layer_Internal* layer = BE_Layer_InternalGet(name);

    if (layer == NULL)
        return SEC_FALSE;

    if (!layer->calledStart) {
        layer->functions.OnStart();

        layer->calledStart = SEC_TRUE;
    }

    if (layer->enabled == enable)
        return SEC_FALSE;

    SEC_LOGGER_DEBUG("%s %s\n", enable ? "Enabling" : "Disabling", name);

    layer->enabled = enable;

    layer->functions.OnToggle(enable); // FIXME: The layer could still be in the middle of an update/event.
    return SEC_TRUE;
}

void BE_PrivateLayer_OnUpdate(BE_Layer_UpdateTypes updateTypes) {
    if (BE_Renderer_GetCurrentType() == BE_RENDERER_TYPE_TEXT)
        return;

    for (int i = 0; i < beLayerArray.used; i++) {
        BE_Layer_Internal* layer = BE_DYNAMICARRAY_GET_ELEMENT(BE_Layer_Internal, beLayerArray, i);

        if (!layer->enabled)
            continue;

        if (!layer->calledStart) {
            layer->functions.OnStart();

            layer->calledStart = SEC_TRUE;
        }

        layer->functions.OnUpdate(updateTypes);
    }
}

int BE_PrivateLayer_OnEvent(BE_Event event) {
    for (int i = 0; i < beLayerArray.used; i++) {
        BE_Layer_Internal* layer = BE_DYNAMICARRAY_GET_ELEMENT(BE_Layer_Internal, beLayerArray, i);

        if (!layer->enabled)
            continue;

        if (!layer->calledStart) {
            layer->functions.OnStart();

            layer->calledStart = SEC_TRUE;
        }

        if (!layer->functions.OnEvent(event))
            continue;

        SEC_LOGGER_TRACE("%s layer stopped the event line\n", layer->publicLayer.name);
        return SEC_TRUE;
    }

    return SEC_FALSE;
}

SEC_Boolean BE_Layer_IsToggled(const char* name) {
    BE_Layer_Internal* layer = BE_Layer_InternalGet(name);

    return layer != NULL && layer->enabled;
}

void BE_PrivateLayer_DestroyLayers(void) {
    if (BE_Renderer_GetCurrentType() == BE_RENDERER_TYPE_TEXT)
        return;

    BE_ASSERT(beLayerInitialized, "Layers are already destroyed\n");
    SEC_LOGGER_INFO("Destroying layer stack\n");

    beLayerInitialized = SEC_FALSE;

    for (int i = 0; i < beLayerArray.used; i++) {
        BE_Layer_Internal* layer = BE_DYNAMICARRAY_GET_ELEMENT(BE_Layer_Internal, beLayerArray, i);

        if (!layer->calledStart)
            continue;

        layer->functions.OnStop();
    }

    for (int i = 0; i < beLayerArray.used; i++)
        BE_EngineMemory_DeallocateMemory(BE_DYNAMICARRAY_GET_ELEMENT(BE_Layer_Internal, beLayerArray, i), sizeof(BE_Layer_Internal), BE_ENGINEMEMORY_MEMORY_TYPE_LAYER);

    BE_EngineMemory_DeallocateMemory(beLayerArray.internalArray, sizeof(void *) * beLayerArray.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
}
SEC_CPP_SUPPORT_GUARD_END()
