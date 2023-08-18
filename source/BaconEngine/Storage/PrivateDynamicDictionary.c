// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "PrivateDynamicDictionary.h"
#include "PrivateDynamicArray.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_Boolean BE_PrivateDynamicDictionary_Create(SEC_DynamicDictionary* dictionary, size_t size) {
    SEC_Boolean returnValue = BE_PrivateDynamicArray_Create(&dictionary->keys, size) && BE_PrivateDynamicArray_Create(&dictionary->values, size);

    dictionary->frozen = SEC_BOOLEAN_FALSE;
    return returnValue;
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
