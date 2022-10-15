#include "BaconEngine/Debugging/Assert.h"
#include "EngineMemory.h"

SEC_CPP_SUPPORT_GUARD_START()
BE_EngineMemory_MemoryInformation beEngineMemoryInformation;

BE_EngineMemory_MemoryInformation BE_EngineMemory_GetMemoryInformation(void) {
    return beEngineMemoryInformation;
}

size_t BE_EngineMemory_GetAllocatedBytes(void) {
    return beEngineMemoryInformation.command.allocatedBytes + beEngineMemoryInformation.ui.allocatedBytes +
           beEngineMemoryInformation.dynamicArray.allocatedBytes + beEngineMemoryInformation.layer.allocatedBytes;
}

BE_EngineMemory_MemoryTypeInformation* BE_EngineMemory_GetMemoryTypeInformation(BE_EngineMemory_MemoryType memoryType) {
    switch (memoryType) {
        case BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND:
            return &beEngineMemoryInformation.command;

        case BE_ENGINEMEMORY_MEMORY_TYPE_UI:
            return &beEngineMemoryInformation.ui;

        case BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY:
            return &beEngineMemoryInformation.dynamicArray;

        case BE_ENGINEMEMORY_MEMORY_TYPE_LAYER:
            return &beEngineMemoryInformation.layer;

        default:
            BE_ASSERT_ALWAYS("This shouldn't be reached");
    }
}

void* BE_EngineMemory_AllocateMemory(size_t size, BE_EngineMemory_MemoryType memoryType) {
    void* pointer;
    BE_EngineMemory_MemoryTypeInformation* memoryTypeInformation = BE_EngineMemory_GetMemoryTypeInformation(
            memoryType);

    SEC_LOGGER_TRACE("Allocating memory\n"
                     "Size: %lu\n"
                     "Type: %i", size, memoryType);
    BE_ASSERT(size > 0, "Size cannot be zero or below");
    BE_ASSERT((pointer = malloc(size)) != NULL, "Failed to allocate %lu bytes of data", size);

    memoryTypeInformation->allocatedBytes += size;
    memoryTypeInformation->allocatedAmount++;

    return pointer;
}

void BE_EngineMemory_ReallocateMemory(void* pointer, size_t oldSize, size_t newSize, BE_EngineMemory_MemoryType memoryType) {
    BE_EngineMemory_MemoryTypeInformation* memoryTypeInformation = BE_EngineMemory_GetMemoryTypeInformation(
            memoryType);

    SEC_LOGGER_TRACE("Reallocating memory\n"
                     "Old Size: %lu\n"
                     "New Size: %lu\n"
                     "Type: %i", oldSize, newSize, memoryType);
    BE_ASSERT(pointer != NULL, "Pointer cannot be null");
    BE_ASSERT(oldSize > 0 && newSize > 0, "Size cannot be zero or below");
    BE_ASSERT((pointer = realloc(pointer, newSize)) != NULL, "Failed to reallocate %lu bytes of data", newSize);

    memoryTypeInformation->allocatedBytes = memoryTypeInformation->allocatedBytes - oldSize + newSize;
}

void BE_EngineMemory_DeallocateMemory(void* pointer, size_t oldSize, BE_EngineMemory_MemoryType memoryType) {
    BE_EngineMemory_MemoryTypeInformation* memoryTypeInformation = BE_EngineMemory_GetMemoryTypeInformation(
            memoryType);

    SEC_LOGGER_TRACE("Deallocating memory\n"
                     "Size: %lu\n"
                     "Type: %i", oldSize, memoryType);
    BE_ASSERT(pointer != NULL, "Pointer cannot be null");
    BE_ASSERT(oldSize > 0, "Size cannot be zero or below");
    free(pointer);

    memoryTypeInformation->allocatedBytes -= oldSize;
    memoryTypeInformation->allocatedAmount--;
}
SEC_CPP_SUPPORT_GUARD_END()
