// Copyright (c) 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/Logger.h>
#include <BaconAPI/Debugging/Assert.h>

#include "PrivateDynamicArray.h"
#include "../EngineMemory.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Boolean BE_PrivateDynamicArray_Create(BA_DynamicArray* array, size_t size) {
    if (size <= 0)
        return BA_BOOLEAN_FALSE;

    array->internalArray = (void**) BA_Memory_Allocate(sizeof(void*) * size, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY);
    array->used = 0;
    array->size = size;
    array->frozen = BA_BOOLEAN_FALSE;
    return BA_BOOLEAN_TRUE;
}

BA_Boolean BE_PrivateDynamicArray_CheckResize(BA_DynamicArray* array) {
    if (array->size != (size_t) array->used)
        return BA_BOOLEAN_TRUE;

    BA_LOGGER_TRACE("Ran out of free space, expanding array\nThis is expensive, so you should try avoiding it\n");
    BA_ASSERT((int) array->size >= array->used, "Invalid array state\n");

    array->size *= 2;
    array->calledReallocate++;
    
    void** newArray = (void**) BA_Memory_Reallocate(array->internalArray, sizeof(void*) * (array->size / 2), sizeof(void*) * array->size, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY);

    if (newArray == NULL)
        return BA_BOOLEAN_FALSE;
    
    array->internalArray = newArray;
    return BA_BOOLEAN_TRUE;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
