// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/String.h>
#include <BaconAPI/Logger.h>
#include <BaconAPI/Storage/DynamicArray.h>
#include <string.h>
#include <BaconAPI/Debugging/Assert.h>

#include "BaconEngine/Configuration.h"
#include "InterfaceFunctions.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static BA_Boolean BE_Configuration_AddLine(BA_DynamicDictionary* results, char* line, ssize_t length) {
    if (length == -2) {
        failed:
        BA_LOGGER_TRACE("Failed to allocate memory for configuration buffer\n");

        for (int j = 0; j < results->keys.used; j++) {
            free(results->keys.internalArray[j]);
            free(results->values.internalArray[j]);
        }

        free(results->keys.internalArray);
        free(results->values.internalArray);
        free(results);
        return BA_BOOLEAN_FALSE;
    }
    
    if (length < 2 || (line[0] == '/' && line[1] == '/'))
        return BA_BOOLEAN_TRUE;

    BA_DynamicArray* configurationInformation = BA_String_SplitCharacter(line, '=');

    if (configurationInformation == NULL)
        goto failed;
    
    if (configurationInformation->used == 0)
        return BA_BOOLEAN_TRUE;

    char* value = BA_String_CreateEmpty();

    for (int i = 1; i < configurationInformation->used; i++) {
        BA_String_Append(&value, BA_DYNAMICARRAY_GET_ELEMENT_POINTER(char, configurationInformation, i));
        free(configurationInformation->internalArray[i]);
    }

    BA_DynamicDictionary_AddElementToLast(results, BA_DYNAMICARRAY_GET_ELEMENT_POINTER(char, configurationInformation, 0), value);
    
    free(configurationInformation->internalArray);
    free(configurationInformation);
    return BA_BOOLEAN_TRUE;
}
#endif

BE_BINARYEXPORT BA_DynamicDictionary* BE_Configuration_ParseFromFile(FILE* configurationFile) {
#ifndef BE_CLIENT_BINARY
    BA_DynamicDictionary* results = malloc(sizeof(BA_DynamicDictionary));
    char* line;
    ssize_t length;
    
    BA_ASSERT(results != NULL, "Failed to allocate memory for configuration results\n");
    BA_DynamicDictionary_Create(results, 100);
    
    while ((length = BA_String_GetLine(configurationFile, &line, NULL)) != -1) {
        if (BE_Configuration_AddLine(results, line, length))
            continue;
        
        return NULL;
    }
    
    return results;
#else
    BE_INTERFACEFUNCTION(BA_DynamicDictionary*, FILE*);
    return function(configurationFile);
#endif
}

BE_BINARYEXPORT BA_DynamicDictionary* BE_Configuration_Parse(const char* configurationData) {
#ifndef BE_CLIENT_BINARY
    BA_DynamicDictionary* results = malloc(sizeof(BA_DynamicDictionary));
    BA_DynamicArray* unparsedKeysValues = BA_String_SplitCharacter(configurationData, '\n');

    BA_ASSERT(results != NULL, "Failed to allocate memory for configuration results\n");
    BA_DynamicDictionary_Create(results, 100);

    for (int i = 0; i < unparsedKeysValues->used; i++) {
        char* line = BA_DYNAMICARRAY_GET_ELEMENT_POINTER(char, unparsedKeysValues, i);

        if (BE_Configuration_AddLine(results, line, (ssize_t) strlen(line)))
            continue;

        return NULL;
    }
    
    return results;
#else
    BE_INTERFACEFUNCTION(BA_DynamicDictionary*, const char*);
    return function(configurationData);
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
