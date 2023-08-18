// Purpose: Stores a variable amount of keys and values.
// Created on: 8/15/22 @ 8:18 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <stddef.h>

#include "SharedEngineCode/Internal/CPlusPlusSupport.h"
#include "DynamicArray.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    SEC_DynamicArray keys;
    SEC_DynamicArray values;
    SEC_Boolean frozen;
} SEC_DynamicDictionary;

SEC_Boolean SEC_DynamicDictionary_Create(SEC_DynamicDictionary* dictionary, size_t size);
SEC_Boolean SEC_DynamicDictionary_AddElementToStart(SEC_DynamicDictionary* dictionary, void* key, void* value);
SEC_Boolean SEC_DynamicDictionary_AddElementToLast(SEC_DynamicDictionary* dictionary, void* key, void* value);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
SEC_Boolean SEC_DynamicDictionary_RemoveFirstElement(SEC_DynamicDictionary* dictionary);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
SEC_Boolean SEC_DynamicDictionary_RemoveLastElement(SEC_DynamicDictionary* dictionary);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
SEC_Boolean SEC_DynamicDictionary_RemoveElementAt(SEC_DynamicDictionary* dictionary, unsigned int index);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
SEC_Boolean SEC_DynamicDictionary_RemoveElementViaKey(SEC_DynamicDictionary* dictionary, void* key, size_t elementSize);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
SEC_Boolean SEC_DynamicDictionary_RemoveElementViaValue(SEC_DynamicDictionary* dictionary, void* value, size_t elementSize);
int SEC_DynamicDictionary_GetElementIndexFromKey(SEC_DynamicDictionary dictionary, void* key, size_t elementSize);
int SEC_DynamicDictionary_GetElementIndexFromValue(SEC_DynamicDictionary dictionary, void* value, size_t elementSize);
void* SEC_DynamicDictionary_GetElementKeyViaValue(SEC_DynamicDictionary dictionary, void* value, size_t elementSize);
void* SEC_DynamicDictionary_GetElementValueViaKey(SEC_DynamicDictionary dictionary, void* key, size_t elementSize);
void SEC_DynamicDictionary_GetElementsKeyViaValue(SEC_DynamicDictionary dictionary, SEC_DynamicDictionary* results, void* value, size_t elementSize);
void SEC_DynamicDictionary_GetElementsValueViaKey(SEC_DynamicDictionary dictionary, SEC_DynamicDictionary* results, void* key, size_t elementSize);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()

#define SEC_DYNAMICDICTIONARY_GET_KEY(type, dictionary, value, elementSize) ((type*) SEC_DynamicDictionary_GetElementIndexFromValue(&(dictionary), (value), (elementSize)))
#define SEC_DYNAMICDICTIONARY_GET_KEY_PTR(type, dictionary, value, elementSize) ((type*) SEC_DynamicDictionary_GetElementIndexFromValue((dictionary), (value), (elementSize)))
#define SEC_DYNAMICDICTIONARY_GET_VALUE(type, dictionary, key, elementSize) ((type*) SEC_DynamicDictionary_GetElementIndexFromKey(&(dictionary), (key), (elementSize)))
#define SEC_DYNAMICDICTIONARY_GET_VALUE_PTR(type, dictionary, key, elementSize) ((type*) SEC_DynamicDictionary_GetElementIndexFromKey((dictionary), (key), (elementSize)))
