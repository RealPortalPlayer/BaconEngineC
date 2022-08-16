#include <SharedEngineCode/Internal/CppHeader.h>
#include <string.h>

#include "BaconEngine/Storage/DynamicDictionary.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"

CPP_GUARD_START()
    int BE_CreateDynamicDictionary(BE_DynamicDictionary* dictionary, size_t size) {
        BE_STRICT_CHECK(size != 0, 0, "Invalid size");

        BE_CreateDynamicArray(&dictionary->keys, size);
        BE_CreateDynamicArray(&dictionary->values, size);
        return 1;
    }

    int BE_DictionaryAddElementToFirst(BE_DynamicDictionary* dictionary, void* key, void* value) {
        return BE_ArrayAddElementToFirst(&dictionary->keys, key) &&
               BE_ArrayAddElementToFirst(&dictionary->values, value);
    }

    int BE_DictionaryAddElementToLast(BE_DynamicDictionary* dictionary, void* key, void* value) {
        return BE_ArrayAddElementToLast(&dictionary->keys, key) &&
                BE_ArrayAddElementToLast(&dictionary->keys, value);
    }

    int BE_DictionaryRemoveFirstElement(BE_DynamicDictionary* dictionary, int shift) {
        return BE_ArrayRemoveFirstElement(&dictionary->keys, shift) &&
               BE_ArrayRemoveFirstElement(&dictionary->values, shift);
    }

    int BE_DictionaryRemoveLastElement(BE_DynamicDictionary* dictionary) {
        return BE_ArrayRemoveLastElement(&dictionary->keys) &&
               BE_ArrayRemoveLastElement(&dictionary->values);
    }

    int BE_DictionaryRemoveElementAt(BE_DynamicDictionary* dictionary, unsigned int index) {
        return BE_ArrayRemoveElementAt(&dictionary->keys, index) &&
               BE_ArrayRemoveElementAt(&dictionary->values, index);
    }

    int BE_DictionaryRemoveElementViaKey(BE_DynamicDictionary* dictionary, void* key, size_t elementSize) {
        int index = BE_GetElementIndexFromKey(*dictionary, key, elementSize);

        return index != -1 ? BE_DictionaryRemoveElementAt(dictionary, index) : 0;
    }

    int BE_DictionaryRemoveElementViaValue(BE_DynamicDictionary* dictionary, void* value, size_t elementSize) {
        int index = BE_GetElementIndexFromValue(*dictionary, value, elementSize);

        return index != -1 ? BE_DictionaryRemoveElementAt(dictionary, index) : 0;
    }

    int BE_GetElementIndexFromKey(BE_DynamicDictionary dictionary, void* key, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], key, elementSize) != 0)
                continue;

            return index;
        }

        return -1;
    }

    int BE_GetElementIndexFromValue(BE_DynamicDictionary dictionary, void* value, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], value, elementSize) != 0)
                continue;

            return index;
        }

        return -1;
    }


    void* BE_GetElementViaValue(BE_DynamicDictionary dictionary, void* key, size_t elementSize) {
        int index = BE_GetElementIndexFromKey(dictionary, key, elementSize);

        return index != -1 ? dictionary.values.internalArray[index] : NULL;
    }

    void* BE_GetElementViaKey(BE_DynamicDictionary dictionary, void* value, size_t elementSize) {
        int index = BE_GetElementIndexFromValue(dictionary, value, elementSize);

        return index != -1 ? dictionary.keys.internalArray[index] : NULL;
    }

    void BE_GetElementsViaKey(BE_DynamicDictionary dictionary, BE_DynamicDictionary* results, void* key, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], key, elementSize) != 0)
                continue;

            BE_DictionaryAddElementToLast(results, key, dictionary.values.internalArray[index]);
        }
    }

    void BE_GetElementsViaValue(BE_DynamicDictionary dictionary, BE_DynamicDictionary* results, void* value, size_t elementSize) {
        for (int index = 0; index < dictionary.keys.used; index++) {
            if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], value, elementSize) != 0)
                continue;

            BE_DictionaryAddElementToLast(results, dictionary.keys.internalArray[index], value);
        }
    }
CPP_GUARD_END()
