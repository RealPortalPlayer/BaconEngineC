#include <string.h>

#include "SharedEngineCode/Storage/DynamicDictionary.h"
#include "SharedEngineCode/Debugging/StrictMode.h"
#include "SharedEngineCode/Debugging/Assert.h"
#include "SharedEngineCode/Internal/CppHeader.h"

SEC_CPP_GUARD_START()
    int UpdateFrozenState(SEC_DynamicDictionary* dictionary) {
        dictionary->frozen = dictionary->frozen || dictionary->keys.frozen || dictionary->values.frozen;
        dictionary->keys.frozen = 0;
        dictionary->values.frozen = 0;
        return dictionary->frozen;
    }

    int SEC_DynamicDictionary_Create(SEC_DynamicDictionary* dictionary, size_t size) {
        SEC_STRICTMODE_CHECK(size != 0, 0, "Invalid size");
        SEC_DynamicArray_Create(&dictionary->keys, size);
        SEC_DynamicArray_Create(&dictionary->values, size);
        return 1;
    }

    int SEC_DynamicDictionary_AddElementToStart(SEC_DynamicDictionary* dictionary, void* key, void* value) {
        if (UpdateFrozenState(dictionary))
            return 0;

        return SEC_DynamicArray_AddElementToStart(&dictionary->keys, key) &&
               SEC_DynamicArray_AddElementToStart(&dictionary->values, value);
    }

    int SEC_DynamicDictionary_AddElementToLast(SEC_DynamicDictionary* dictionary, void* key, void* value) {
        if (UpdateFrozenState(dictionary))
            return 0;

        return SEC_DynamicArray_AddElementToLast(&dictionary->keys, key) &&
               SEC_DynamicArray_AddElementToLast(&dictionary->keys, value);
    }

    int SEC_DynamicDictionary_RemoveFirstElement(SEC_DynamicDictionary* dictionary, int shift) {
        if (UpdateFrozenState(dictionary))
            return 0;

        return SEC_DynamicArray_RemoveFirstElement(&dictionary->keys, shift) &&
               SEC_DynamicArray_RemoveFirstElement(&dictionary->values, shift);
    }

    int SEC_DictionaryRemoveLastElement(SEC_DynamicDictionary* dictionary) {
        if (UpdateFrozenState(dictionary))
            return 0;

        return SEC_DynamicArray_RemoveLastElement(&dictionary->keys) &&
               SEC_DynamicArray_RemoveLastElement(&dictionary->values);
    }

    int SEC_DynamicDictionary_RemoveElementAt(SEC_DynamicDictionary* dictionary, unsigned int index) {
        if (UpdateFrozenState(dictionary))
            return 0;

        return SEC_DynamicArray_RemoveElementAt(&dictionary->keys, index) &&
               SEC_DynamicArray_RemoveElementAt(&dictionary->values, index);
    }

    int SEC_DynamicDictionary_RemoveElementViaKey(SEC_DynamicDictionary* dictionary, void* key, size_t elementSize) {
        if (UpdateFrozenState(dictionary))
            return 0;

        int index = SEC_DynamicDictionary_GetElementIndexFromKey(*dictionary, key, elementSize);

        return index != -1 ? SEC_DynamicDictionary_RemoveElementAt(dictionary, index) : 0;
    }

    int SEC_DynamicDictionary_RemoveElementViaValue(SEC_DynamicDictionary* dictionary, void* value, size_t elementSize) {
        if (UpdateFrozenState(dictionary))
            return 0;

        int index = SEC_DynamicDictionary_GetElementIndexFromValue(*dictionary, value, elementSize);

        return index != -1 ? SEC_DynamicDictionary_RemoveElementAt(dictionary, index) : 0;
    }

    int SEC_DynamicDictionary_GetElementIndexFromKey(SEC_DynamicDictionary dictionary, void* key, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], key, elementSize) != 0)
                continue;

            return index;
        }

        return -1;
    }

    int SEC_DynamicDictionary_GetElementIndexFromValue(SEC_DynamicDictionary dictionary, void* value, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], value, elementSize) != 0)
                continue;

            return index;
        }

        return -1;
    }

    void* SEC_DynamicDictionary_GetElementViaValue(SEC_DynamicDictionary dictionary, void* key, size_t elementSize) {
        int index = SEC_DynamicDictionary_GetElementIndexFromKey(dictionary, key, elementSize);

        return index != -1 ? dictionary.values.internalArray[index] : NULL;
    }

    void* SEC_DynamicDictionary_GetElementViaKey(SEC_DynamicDictionary dictionary, void* value, size_t elementSize) {
        int index = SEC_DynamicDictionary_GetElementIndexFromValue(dictionary, value, elementSize);

        return index != -1 ? dictionary.keys.internalArray[index] : NULL;
    }

    void SEC_DynamicDictionary_GetElementsViaKey(SEC_DynamicDictionary dictionary, SEC_DynamicDictionary* results, void* key, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], key, elementSize) != 0)
                continue;

            SEC_DynamicDictionary_AddElementToLast(results, key, dictionary.values.internalArray[index]);
        }
    }

    void SEC_DynamicDictionary_GetElementsViaValue(SEC_DynamicDictionary dictionary, SEC_DynamicDictionary* results, void* value, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], value, elementSize) != 0)
                continue;

            SEC_DynamicDictionary_AddElementToLast(results, dictionary.keys.internalArray[index], value);
        }
    }

    void SEC_DynamicDictionary_Shrink(SEC_DynamicDictionary* dictionary) {
        if (UpdateFrozenState(dictionary))
            return;

        SEC_LOGGER_TRACE("Shrinking dictionary, this is expensive");

        SEC_DynamicArray_Shrink(&dictionary->keys);
        SEC_DynamicArray_Shrink(&dictionary->values);
    }
SEC_CPP_GUARD_END()
