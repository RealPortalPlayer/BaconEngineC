#include "SharedEngineCode/Internal/CppHeader.h"
#include <string.h>

#include "SharedEngineCode/Storage/DynamicDictionary.h"
#include "SharedEngineCode/Debugging/StrictMode.h"
#include "SharedEngineCode/Debugging/Assert.h"

CPP_GUARD_START()
    int SEC_CreateDynamicDictionary(SEC_DynamicDictionary* dictionary, size_t size) {
        SEC_STRICT_CHECK(size != 0, 0, "Invalid size");

        SEC_CreateDynamicArray(&dictionary->keys, size);
        SEC_CreateDynamicArray(&dictionary->values, size);
        return 1;
    }

    int SEC_DictionaryAddElementToFirst(SEC_DynamicDictionary* dictionary, void* key, void* value) {
        return SEC_ArrayAddElementToFirst(&dictionary->keys, key) &&
               SEC_ArrayAddElementToFirst(&dictionary->values, value);
    }

    int SEC_DictionaryAddElementToLast(SEC_DynamicDictionary* dictionary, void* key, void* value) {
        return SEC_ArrayAddElementToLast(&dictionary->keys, key) &&
               SEC_ArrayAddElementToLast(&dictionary->keys, value);
    }

    int SEC_DictionaryRemoveFirstElement(SEC_DynamicDictionary* dictionary, int shift) {
        return SEC_ArrayRemoveFirstElement(&dictionary->keys, shift) &&
               SEC_ArrayRemoveFirstElement(&dictionary->values, shift);
    }

    int SEC_DictionaryRemoveLastElement(SEC_DynamicDictionary* dictionary) {
        return SEC_ArrayRemoveLastElement(&dictionary->keys) &&
               SEC_ArrayRemoveLastElement(&dictionary->values);
    }

    int SEC_DictionaryRemoveElementAt(SEC_DynamicDictionary* dictionary, unsigned int index) {
        return SEC_ArrayRemoveElementAt(&dictionary->keys, index) &&
               SEC_ArrayRemoveElementAt(&dictionary->values, index);
    }

    int SEC_DictionaryRemoveElementViaKey(SEC_DynamicDictionary* dictionary, void* key, size_t elementSize) {
        int index = SEC_GetElementIndexFromKey(*dictionary, key, elementSize);

        return index != -1 ? SEC_DictionaryRemoveElementAt(dictionary, index) : 0;
    }

    int SEC_DictionaryRemoveElementViaValue(SEC_DynamicDictionary* dictionary, void* value, size_t elementSize) {
        int index = SEC_GetElementIndexFromValue(*dictionary, value, elementSize);

        return index != -1 ? SEC_DictionaryRemoveElementAt(dictionary, index) : 0;
    }

    int SEC_GetElementIndexFromKey(SEC_DynamicDictionary dictionary, void* key, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], key, elementSize) != 0)
                continue;

            return index;
        }

        return -1;
    }

    int SEC_GetElementIndexFromValue(SEC_DynamicDictionary dictionary, void* value, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], value, elementSize) != 0)
                continue;

            return index;
        }

        return -1;
    }


    void* SEC_GetElementViaValue(SEC_DynamicDictionary dictionary, void* key, size_t elementSize) {
        int index = SEC_GetElementIndexFromKey(dictionary, key, elementSize);

        return index != -1 ? dictionary.values.internalArray[index] : NULL;
    }

    void* SEC_GetElementViaKey(SEC_DynamicDictionary dictionary, void* value, size_t elementSize) {
        int index = SEC_GetElementIndexFromValue(dictionary, value, elementSize);

        return index != -1 ? dictionary.keys.internalArray[index] : NULL;
    }

    void SEC_GetElementsViaKey(SEC_DynamicDictionary dictionary, SEC_DynamicDictionary* results, void* key, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], key, elementSize) != 0)
                continue;

            SEC_DictionaryAddElementToLast(results, key, dictionary.values.internalArray[index]);
        }
    }

    void SEC_GetElementsViaValue(SEC_DynamicDictionary dictionary, SEC_DynamicDictionary* results, void* value, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], value, elementSize) != 0)
                continue;

            SEC_DictionaryAddElementToLast(results, dictionary.keys.internalArray[index], value);
        }
    }
CPP_GUARD_END()
