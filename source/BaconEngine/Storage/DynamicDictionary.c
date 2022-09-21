#include <string.h>
#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Storage/DynamicDictionary.h"

SEC_CPP_GUARD_START()
    int UpdateFrozenState(BE_DynamicDictionary* dictionary) {
        dictionary->frozen = dictionary->frozen || dictionary->keys.frozen || dictionary->values.frozen;
        dictionary->keys.frozen = 0;
        dictionary->values.frozen = 0;
        return dictionary->frozen;
    }

    int BE_DynamicDictionary_Create(BE_DynamicDictionary* dictionary, size_t size) {
        BE_STRICTMODE_CHECK(size != 0, 0, "Invalid size");
        BE_DynamicArray_Create(&dictionary->keys, size);
        BE_DynamicArray_Create(&dictionary->values, size);
        return 1;
    }

    int BE_DynamicDictionary_AddElementToStart(BE_DynamicDictionary* dictionary, void* key, void* value) {
        if (UpdateFrozenState(dictionary))
            return 0;

        return BE_DynamicArray_AddElementToStart(&dictionary->keys, key) &&
               BE_DynamicArray_AddElementToStart(&dictionary->values, value);
    }

    int BE_DynamicDictionary_AddElementToLast(BE_DynamicDictionary* dictionary, void* key, void* value) {
        if (UpdateFrozenState(dictionary))
            return 0;

        return BE_DynamicArray_AddElementToLast(&dictionary->keys, key) &&
               BE_DynamicArray_AddElementToLast(&dictionary->keys, value);
    }

    int BE_DynamicDictionary_RemoveFirstElement(BE_DynamicDictionary* dictionary, int shift) {
        if (UpdateFrozenState(dictionary))
            return 0;

        return BE_DynamicArray_RemoveFirstElement(&dictionary->keys, shift) &&
               BE_DynamicArray_RemoveFirstElement(&dictionary->values, shift);
    }

    int BE_DynamicDictionary_RemoveLastElement(BE_DynamicDictionary* dictionary) {
        if (UpdateFrozenState(dictionary))
            return 0;

        return BE_DynamicArray_RemoveLastElement(&dictionary->keys) &&
               BE_DynamicArray_RemoveLastElement(&dictionary->values);
    }

    int BE_DynamicDictionary_RemoveElementAt(BE_DynamicDictionary* dictionary, unsigned int index) {
        if (UpdateFrozenState(dictionary))
            return 0;

        return BE_DynamicArray_RemoveElementAt(&dictionary->keys, index) &&
               BE_DynamicArray_RemoveElementAt(&dictionary->values, index);
    }

    int BE_DynamicDictionary_RemoveElementViaKey(BE_DynamicDictionary* dictionary, void* key, size_t elementSize) {
        if (UpdateFrozenState(dictionary))
            return 0;

        int index = BE_DynamicDictionary_GetElementIndexFromKey(*dictionary, key, elementSize);

        return index != -1 ? BE_DynamicDictionary_RemoveElementAt(dictionary, index) : 0;
    }

    int BE_DynamicDictionary_RemoveElementViaValue(BE_DynamicDictionary* dictionary, void* value, size_t elementSize) {
        if (UpdateFrozenState(dictionary))
            return 0;

        int index = BE_DynamicDictionary_GetElementIndexFromValue(*dictionary, value, elementSize);

        return index != -1 ? BE_DynamicDictionary_RemoveElementAt(dictionary, index) : 0;
    }

    int BE_DynamicDictionary_GetElementIndexFromKey(BE_DynamicDictionary dictionary, void* key, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], key, elementSize) != 0)
                continue;

            return index;
        }

        return -1;
    }

    int BE_DynamicDictionary_GetElementIndexFromValue(BE_DynamicDictionary dictionary, void* value, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], value, elementSize) != 0)
                continue;

            return index;
        }

        return -1;
    }

    void* BE_DynamicDictionary_GetElementViaValue(BE_DynamicDictionary dictionary, void* key, size_t elementSize) {
        int index = BE_DynamicDictionary_GetElementIndexFromKey(dictionary, key, elementSize);

        return index != -1 ? dictionary.values.internalArray[index] : NULL;
    }

    void* BE_DynamicDictionary_GetElementViaKey(BE_DynamicDictionary dictionary, void* value, size_t elementSize) {
        int index = BE_DynamicDictionary_GetElementIndexFromValue(dictionary, value, elementSize);

        return index != -1 ? dictionary.keys.internalArray[index] : NULL;
    }

    void BE_DynamicDictionary_GetElementsViaKey(BE_DynamicDictionary dictionary, BE_DynamicDictionary* results, void* key, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], key, elementSize) != 0)
                continue;

            BE_DynamicDictionary_AddElementToLast(results, key, dictionary.values.internalArray[index]);
        }
    }

    void BE_DynamicDictionary_GetElementsViaValue(BE_DynamicDictionary dictionary, BE_DynamicDictionary* results, void* value, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], value, elementSize) != 0)
                continue;

            BE_DynamicDictionary_AddElementToLast(results, dictionary.keys.internalArray[index], value);
        }
    }

    void BE_DynamicDictionary_Shrink(BE_DynamicDictionary* dictionary) {
        if (UpdateFrozenState(dictionary))
            return;

        SEC_LOGGER_TRACE("Shrinking dictionary, this is expensive");

        BE_DynamicArray_Shrink(&dictionary->keys);
        BE_DynamicArray_Shrink(&dictionary->values);
    }
SEC_CPP_GUARD_END()
