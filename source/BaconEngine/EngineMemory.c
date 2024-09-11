// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <BaconAPI/Debugging/Assert.h>
#include <BaconAPI/Debugging/StaticAssert.h>
#include <BaconAPI/String.h>

#ifndef BE_CLIENT_BINARY
#   include "EngineMemory.h"
#endif

#include "InterfaceFunctions.h"
#include "BaconEngine/EngineMemoryInformation.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
BA_Memory_TypeData baMemoryLookupTable[] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0}
};

BA_STATIC_ASSERT_LOOKUP_TABLE_CHECK(baMemoryLookupTable, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SIZE);

BA_Boolean BE_EngineMemory_AllocateDeallocateLogsEnabled(void) {
#   ifdef BA_ALLOW_DEBUG_LOGS
    static int enabled = -1;

    if (enabled == -1)
        enabled = !BA_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOCATION, SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOCATION_SHORT, 0);

    return enabled;
#   else
    return BA_BOOLEAN_FALSE;
#   endif
}
#endif

size_t BE_EngineMemoryInformation_GetAllocatedBytes(void) {
#ifndef BE_CLIENT_BINARY
    return BA_Memory_GetAllocatedBytes();
#else
    BE_INTERFACEFUNCTION(size_t, void);
    return function();
#endif
}

#ifndef BE_CLIENT_BINARY
char* BA_Memory_GetAllocatedInformation(const char* prefix) {
    char* finalString;

    BA_MEMORY_CREATE_INFORMATION_STRING(finalString, BA_MEMORY_DEFINE_INFORMATION_STRING_TEMPLATE("Command") "\n"
                                                     BA_MEMORY_DEFINE_INFORMATION_STRING_TEMPLATE("UI") "\n"
                                                     BA_MEMORY_DEFINE_INFORMATION_STRING_TEMPLATE("DynamicArray") "\n"
                                                     BA_MEMORY_DEFINE_INFORMATION_STRING_TEMPLATE("Layer") "\n"
                                                     BA_MEMORY_DEFINE_INFORMATION_STRING_TEMPLATE("Server") "\n"
                                                     BA_MEMORY_DEFINE_INFORMATION_STRING_TEMPLATE("  Connected") "\n"
                                                     BA_MEMORY_DEFINE_INFORMATION_STRING_TEMPLATE("  Client") "\n"
                                                     BA_MEMORY_DEFINE_INFORMATION_STRING_TEMPLATE("  Client Socket") "\n"
                                                     BA_MEMORY_DEFINE_INFORMATION_STRING_TEMPLATE("ArgumentManager") "\n"
                                                     BA_MEMORY_DEFINE_INFORMATION_STRING_TEMPLATE("  Name") "\n"
                                                     BA_MEMORY_DEFINE_INFORMATION_STRING_TEMPLATE("  Argument") "\n"
                                                     BA_MEMORY_DEFINE_INFORMATION_STRING_TEMPLATE("  Temporary Argument"));
#   define BE_ENGINEMEMORY_ADD_INFORMATION(memoryType1, memoryType2, memoryType3) \
prefix, baMemoryLookupTable[memoryType1].allocatedAmount + baMemoryLookupTable[memoryType2].allocatedAmount + baMemoryLookupTable[memoryType3].allocatedAmount, baMemoryLookupTable[memoryType1].allocatedBytes + baMemoryLookupTable[memoryType2].allocatedBytes + baMemoryLookupTable[memoryType3].allocatedBytes, \
BA_MEMORY_GET_MEMORY_INFORMATION(memoryType1),                              \
BA_MEMORY_GET_MEMORY_INFORMATION(memoryType2),                              \
BA_MEMORY_GET_MEMORY_INFORMATION(memoryType3)
    
    BA_ASSERT(BA_String_Format(&finalString,
                               BA_MEMORY_GET_MEMORY_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_COMMAND),
                               BA_MEMORY_GET_MEMORY_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_UI),
                               BA_MEMORY_GET_MEMORY_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY),
                               BA_MEMORY_GET_MEMORY_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_LAYER),
                               BE_ENGINEMEMORY_ADD_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CONNECTED, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CLIENT, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CLIENT_SOCKET),
                               BE_ENGINEMEMORY_ADD_INFORMATION(BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_ARGUMENT_MANAGER_NAME, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_ARGUMENT_MANAGER_ARGUMENT, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_ARGUMENT_MANAGER_TEMPORARY_ARGUMENT)) != NULL, "Failed to format string\n");
    return finalString;
}

size_t BA_Memory_GetEnumeratorSize(void) {
    return BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SIZE;
}
#endif

char* BE_EngineMemoryInformation_GetAllocationInformation(const char* prefix) {
#ifndef BE_CLIENT_BINARY
    return BA_Memory_GetAllocatedInformation(prefix);
#else
    BE_INTERFACEFUNCTION(char*, const char*);
    return function(prefix);
#endif
}

size_t BE_EngineMemoryInformation_GetAllocatedAmount(void) {
#ifndef BE_CLIENT_BINARY
    return BA_Memory_GetAllocatedAmount();
#else
    BE_INTERFACEFUNCTION(size_t, void);
    return function();
#endif
}

BE_BINARYEXPORT size_t BE_EngineMemoryInformation_GetEnumeratorSize(void) {
#ifndef BE_CLIENT_BINARY
    return BA_Memory_GetEnumeratorSize();
#else
    BE_INTERFACEFUNCTION(size_t, void);
    return function();
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
