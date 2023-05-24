// Purpose: Stores a variable amount of keys and values.
// Created on: 8/15/22 @ 8:18 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <stddef.h>

#include "SharedEngineCode/Internal/CPlusPlusSupport.h"
#include "DynamicArray.h"
#include "BaconEngine/BinaryExport.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    BE_DynamicArray keys;
    BE_DynamicArray values;
    SEC_Boolean frozen;
} BE_DynamicDictionary;

BE_BINARYEXPORT SEC_Boolean BE_DynamicDictionary_Create(BE_DynamicDictionary* dictionary, size_t size);
BE_BINARYEXPORT SEC_Boolean BE_DynamicDictionary_AddElementToStart(BE_DynamicDictionary* dictionary, void* key, void* value);
BE_BINARYEXPORT SEC_Boolean BE_DynamicDictionary_AddElementToLast(BE_DynamicDictionary* dictionary, void* key, void* value);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BE_BINARYEXPORT SEC_Boolean BE_DynamicDictionary_RemoveFirstElement(BE_DynamicDictionary* dictionary);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BE_BINARYEXPORT SEC_Boolean BE_DynamicDictionary_RemoveLastElement(BE_DynamicDictionary* dictionary);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BE_BINARYEXPORT SEC_Boolean BE_DynamicDictionary_RemoveElementAt(BE_DynamicDictionary* dictionary, unsigned int index);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BE_BINARYEXPORT SEC_Boolean BE_DynamicDictionary_RemoveElementViaKey(BE_DynamicDictionary* dictionary, void* key, size_t elementSize);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BE_BINARYEXPORT SEC_Boolean BE_DynamicDictionary_RemoveElementViaValue(BE_DynamicDictionary* dictionary, void* value, size_t elementSize);
BE_BINARYEXPORT int BE_DynamicDictionary_GetElementIndexFromKey(BE_DynamicDictionary dictionary, void* key, size_t elementSize);
BE_BINARYEXPORT int BE_DynamicDictionary_GetElementIndexFromValue(BE_DynamicDictionary dictionary, void* value, size_t elementSize);
BE_BINARYEXPORT void* BE_DynamicDictionary_GetElementKeyViaValue(BE_DynamicDictionary dictionary, void* value, size_t elementSize);
BE_BINARYEXPORT void* BE_DynamicDictionary_GetElementValueViaKey(BE_DynamicDictionary dictionary, void* key, size_t elementSize);
BE_BINARYEXPORT void BE_DynamicDictionary_GetElementsKeyViaValue(BE_DynamicDictionary dictionary, BE_DynamicDictionary* results, void* value, size_t elementSize);
BE_BINARYEXPORT void BE_DynamicDictionary_GetElementsValueViaKey(BE_DynamicDictionary dictionary, BE_DynamicDictionary* results, void* key, size_t elementSize);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()

#define BE_DYNAMICDICTIONARY_GET_KEY(type, dictionary, value, elementSize) ((type*) BE_DynamicDictionary_GetElementIndexFromValue(&(dictionary), (value), (elementSize)))
#define BE_DYNAMICDICTIONARY_GET_KEY_PTR(type, dictionary, value, elementSize) ((type*) BE_DynamicDictionary_GetElementIndexFromValue((dictionary), (value), (elementSize)))
#define BE_DYNAMICDICTIONARY_GET_VALUE(type, dictionary, key, elementSize) ((type*) BE_DynamicDictionary_GetElementIndexFromKey(&(dictionary), (key), (elementSize)))
#define BE_DYNAMICDICTIONARY_GET_VALUE_PTR(type, dictionary, key, elementSize) ((type*) BE_DynamicDictionary_GetElementIndexFromKey((dictionary), (key), (elementSize)))
