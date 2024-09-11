// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <SharedEngineCode/Debugging/StrictMode.h>
#include <BaconAPI/Debugging/Assert.h>

#include "BaconEngine/Rendering/Layer.h"
#include "../InterfaceFunctions.h"

#ifndef BE_CLIENT_BINARY
#   include "PrivateLayer.h"
#   include "BaconEngine/Rendering/Renderer.h"
#   include "EngineLayers.h"
#   include "../EngineMemory.h"
#   include "../Storage/PrivateDynamicArray.h"
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
typedef struct {
    const char* name;
    BE_Layer_Functions functions;
    BA_Boolean calledStart;
    BA_Boolean enabled;
} BE_Layer_Internal;

static BA_DynamicArray beLayerArray;
static BA_Boolean beLayerInitialized = BA_BOOLEAN_FALSE;

static int BE_Layer_NoOperation(void) {
    return 0;
}

static BE_Layer_Internal* BE_Layer_InternalGet(const char* name) {
    for (int i = 0; i < (int) beLayerArray.used; i++) {
        if (!BA_String_Equals(BA_DYNAMICARRAY_GET_ELEMENT(BE_Layer_Internal, beLayerArray, i)->name, name, BA_BOOLEAN_FALSE))
            continue;

        return beLayerArray.internalArray[i];
    }

    return NULL;
}

void BE_PrivateLayer_InitializeLayers(void) {
    if (BE_Renderer_GetCurrentType() == BE_RENDERER_TYPE_TEXT)
        return;

    BA_ASSERT(!beLayerInitialized, "Already initialized the layer stack\n");
    BA_LOGGER_INFO("Initializing layer stack\n");

    beLayerInitialized = BA_BOOLEAN_TRUE;

    BE_PrivateDynamicArray_Create(&beLayerArray, 100);
    BA_LOGGER_INFO("Registering engine layers\n");
    BE_EngineLayers_Initialize();
}
#endif


void BE_Layer_Register(const char* name, BA_Boolean enabled, BE_Layer_Functions functions) {
#ifndef BE_CLIENT_BINARY
    if (BE_Renderer_GetCurrentType() == BE_RENDERER_TYPE_TEXT)
        return;

    for (int i = 0; i < (int) beLayerArray.used; i++)
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(!BA_String_Equals(BA_DYNAMICARRAY_GET_ELEMENT(BE_Layer_Internal, beLayerArray, i)->name, name, BA_BOOLEAN_FALSE), "The layer '%s' is already registered\n", name);

    BE_Layer_Internal* layer = (BE_Layer_Internal*) BA_Memory_Allocate(sizeof(BE_Layer_Internal), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_LAYER);

    layer->name = name;
    layer->enabled = enabled;
    layer->calledStart = BA_BOOLEAN_FALSE;
    layer->functions.OnStart = functions.OnStart != NULL ? functions.OnStart : (void (*)(void)) &BE_Layer_NoOperation;
    layer->functions.OnUpdate = functions.OnUpdate != NULL ? functions.OnUpdate : (void (*)(BE_Layer_UpdateTypes)) &BE_Layer_NoOperation;
    layer->functions.OnToggle = functions.OnToggle != NULL ? functions.OnToggle : (void (*)(int)) &BE_Layer_NoOperation;
    layer->functions.OnEvent = functions.OnEvent != NULL ? functions.OnEvent : (int (*)(BE_Event)) &BE_Layer_NoOperation;
    layer->functions.OnStop = functions.OnStop != NULL ? functions.OnStop : (void (*)(void)) &BE_Layer_NoOperation;

    BA_DynamicArray_AddElementToLast(&beLayerArray, (void*) layer);
#else
    BE_INTERFACEFUNCTION(void, const char*, BA_Boolean, BE_Layer_Functions)(name, enabled, functions);
#endif
}

int BE_Layer_GetAmount(void) {
#ifndef BE_CLIENT_BINARY
    return beLayerInitialized ? beLayerArray.used : 0;
#else
    BE_INTERFACEFUNCTION(int, void);
    return function();
#endif
}

size_t BE_Layer_GetAllocatedLayersAmount(void) {
#ifndef BE_CLIENT_BINARY
    return beLayerInitialized ? beLayerArray.size : 0;
#else
    BE_INTERFACEFUNCTION(size_t, void);
    return function();
#endif
}

int BE_Layer_GetLayersReallocationAmount(void) {
#ifndef BE_CLIENT_BINARY
    return beLayerInitialized ? beLayerArray.calledReallocate : 0;
#else
    BE_INTERFACEFUNCTION(int, void);
    return function();
#endif
}

BA_Boolean BE_Layer_Toggle(const char* name, BA_Boolean enable) {
#ifndef BE_CLIENT_BINARY
    BE_Layer_Internal* layer = BE_Layer_InternalGet(name);

    if (layer == NULL)
        return BA_BOOLEAN_FALSE;

    if (!layer->calledStart) {
        layer->functions.OnStart();

        layer->calledStart = BA_BOOLEAN_TRUE;
    }

    if (layer->enabled == enable)
        return BA_BOOLEAN_FALSE;

    BA_LOGGER_DEBUG("%s %s\n", enable ? "Enabling" : "Disabling", name);

    layer->enabled = enable;

    layer->functions.OnToggle(enable); // FIXME: The layer could still be in the middle of an update/event.
    return BA_BOOLEAN_TRUE;
#else
    BE_INTERFACEFUNCTION(BA_Boolean, const char*, BA_Boolean);
    return function(name, enable);
#endif
}

#ifndef BE_CLIENT_BINARY
void BE_PrivateLayer_OnUpdate(BE_Layer_UpdateTypes updateTypes) {
    if (BE_Renderer_GetCurrentType() == BE_RENDERER_TYPE_TEXT)
        return;

    for (int i = 0; i < beLayerArray.used; i++) {
        BE_Layer_Internal* layer = BA_DYNAMICARRAY_GET_ELEMENT(BE_Layer_Internal, beLayerArray, i);

        if (!layer->enabled)
            continue;

        if (!layer->calledStart) {
            layer->functions.OnStart();

            layer->calledStart = BA_BOOLEAN_TRUE;
        }

        layer->functions.OnUpdate(updateTypes);
    }
}

int BE_PrivateLayer_OnEvent(BE_Event event) {
    for (int i = 0; i < beLayerArray.used; i++) {
        BE_Layer_Internal* layer = BA_DYNAMICARRAY_GET_ELEMENT(BE_Layer_Internal, beLayerArray, i);

        if (!layer->enabled)
            continue;

        if (!layer->calledStart) {
            layer->functions.OnStart();

            layer->calledStart = BA_BOOLEAN_TRUE;
        }

        if (!layer->functions.OnEvent(event))
            continue;

        BA_LOGGER_TRACE("%s layer stopped the event line\n", layer->name);
        return BA_BOOLEAN_TRUE;
    }

    return BA_BOOLEAN_FALSE;
}
#endif

BA_Boolean BE_Layer_IsToggled(const char* name) {
#ifndef BE_CLIENT_BINARY
    BE_Layer_Internal* layer = BE_Layer_InternalGet(name);

    return layer != NULL && layer->enabled;
#else
    BE_INTERFACEFUNCTION(BA_Boolean, const char*);
    return function(name);
#endif
}

BA_Boolean BE_Layer_Exists(const char* name) {
#ifndef BE_CLIENT_BINARY
    return BE_Layer_InternalGet(name) != NULL;
#else
    BE_INTERFACEFUNCTION(BA_Boolean, const char*);
    return function(name);
#endif
}

#ifndef BE_CLIENT_BINARY
void BE_PrivateLayer_DestroyLayers(void) {
    if (BE_Renderer_GetCurrentType() == BE_RENDERER_TYPE_TEXT)
        return;

    BA_ASSERT(beLayerInitialized, "Layers are already destroyed\n");
    BA_LOGGER_INFO("Destroying layer stack\n");

    beLayerInitialized = BA_BOOLEAN_FALSE;

    for (int i = 0; i < beLayerArray.used; i++) {
        BE_Layer_Internal* layer = BA_DYNAMICARRAY_GET_ELEMENT(BE_Layer_Internal, beLayerArray, i);

        if (!layer->calledStart)
            continue;

        layer->functions.OnStop();
    }

    for (int i = 0; i < beLayerArray.used; i++)
        BA_Memory_Deallocate(BA_DYNAMICARRAY_GET_ELEMENT(BE_Layer_Internal, beLayerArray, i), sizeof(BE_Layer_Internal), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_LAYER);

    BA_Memory_Deallocate(beLayerArray.internalArray, sizeof(void*) * beLayerArray.size, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY);
}
#endif
BA_CPLUSPLUS_SUPPORT_GUARD_END()
