// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "PrivateDynamicDictionary.h"
#include "PrivateDynamicArray.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Boolean BE_PrivateDynamicDictionary_Create(BA_DynamicDictionary* dictionary, size_t size) {
    BA_Boolean returnValue = BE_PrivateDynamicArray_Create(&dictionary->keys, size) && BE_PrivateDynamicArray_Create(&dictionary->values, size);

    dictionary->frozen = BA_BOOLEAN_FALSE;
    return returnValue;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
