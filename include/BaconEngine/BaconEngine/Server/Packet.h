// Purpose: Main packet implementation
// Created on: 1/8/24 @ 3:20 PM

// Copyright (c) 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>
#include <BaconAPI/Internal/Compiler.h>
#include <stdint.h>
#include <string.h>
#include <BaconAPI/Debugging/Assert.h>

#include "Client.h"
#include "BaconEngine/BinaryExport.h"

#define BE_PACKET_MAXIMUM_DATA 1024

#if BA_COMPILER_GCC || BA_COMPILER_CLANG
#   define BE_PACKET_PACKED(structure) structure __attribute__((packed))
#elif BA_COMPILER_MSVC
#   define BE_PACKET_PACKED(structure) __pragma(pack(push, 1)) structure __pragma(pack(pop))
#else
#   define BE_PACKET_PACKED(structure) structure
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef void (*BE_Packet_Run)(BE_Client client, char data[BE_PACKET_MAXIMUM_DATA]);

// TODO: Flags

typedef enum {
    BE_PACKET_FLAG_ALLOW_CLIENT_TO_SERVER = 1 << 0,
    BE_PACKET_FLAG_ALLOW_SERVER_TO_CLIENT = 1 << 1,
    BE_PACKET_FLAG_ALLOW_UNCONNECTED = 1 << 2,

    BE_PACKET_FLAG_ALLOW_BOTH = BE_PACKET_FLAG_ALLOW_CLIENT_TO_SERVER | BE_PACKET_FLAG_ALLOW_SERVER_TO_CLIENT
} BE_Packet_Flags;

typedef enum {
    BE_PACKET_DATA_FIELD_TYPE_NULL,
    BE_PACKET_DATA_FIELD_TYPE_INTEGER,
    BE_PACKET_DATA_FIELD_TYPE_STRING
} BE_Packet_DataFieldTypes;

BE_BINARYEXPORT void BE_Packet_Register(uint64_t operationCode, BE_Packet_Flags flags, BE_Packet_Run Run);

BE_BINARYEXPORT void BE_Packet_Send(BE_Client client, uint64_t operationCode, char data[BE_PACKET_MAXIMUM_DATA]);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#define BE_PACKET_CREATOR_HEADER(code) \
char data[BE_PACKET_MAXIMUM_DATA] = {0}; \
char* dataWriter = data;               \
uint64_t operationCode = code

#define BE_PACKET_CREATOR_INTERNAL_CHECK(size) (dataWriter + (size) < data + BE_PACKET_MAXIMUM_DATA)

#define BE_PACKET_CREATOR_INTERNAL_SET(value, size) \
do {                                                \
    size_t calculatedSize = size;                   \
    if (BE_PACKET_CREATOR_INTERNAL_CHECK(calculatedSize)) \
        memset(dataWriter, value, calculatedSize);  \
    dataWriter += calculatedSize;                   \
} while (BA_BOOLEAN_FALSE)

#define BE_PACKET_CREATOR_INTERNAL_COPY(value, size) \
do {                                                 \
    size_t calculatedSize = size;                    \
    if (BE_PACKET_CREATOR_INTERNAL_CHECK(calculatedSize)) \
        memcpy(dataWriter, value, calculatedSize);   \
    dataWriter += calculatedSize;                    \
} while (BA_BOOLEAN_FALSE)

#define BE_PACKET_CREATOR_INTERNAL_SET_FIELD_TYPE(field) BE_PACKET_CREATOR_INTERNAL_SET(field, sizeof(BE_Packet_DataFieldTypes))

#define BE_PACKET_CREATOR_SET_INTEGER(value) \
BE_PACKET_CREATOR_INTERNAL_SET_FIELD_TYPE(BE_PACKET_DATA_FIELD_TYPE_INTEGER); \
BE_PACKET_CREATOR_INTERNAL_SET(value, sizeof(int))

#define BE_PACKET_CREATOR_SET_STRING(value) \
BE_PACKET_CREATOR_INTERNAL_SET_FIELD_TYPE(BE_PACKET_DATA_FIELD_TYPE_STRING); \
BE_PACKET_CREATOR_INTERNAL_COPY(value, sizeof(char) * (strlen(value) + 1))

#define BE_PACKET_CREATOR_FOOTER(client) \
if (!BE_PACKET_CREATOR_INTERNAL_CHECK(0)) \
    BA_LOGGER_ERROR("Packet data attempted overflow of %zu bytes\n", (dataWriter - data) - BE_PACKET_MAXIMUM_DATA); \
BE_Packet_Send(client, operationCode, data)

#define BE_PACKET_PARSER_HEADER(dataVariable) \
char* dataReader = dataVariable; \
char* unmovedDataVariable = dataVariable;

#define BE_PACKET_PARSER_INTERNAL_CHECK_SIZE(size) BA_ASSERT((dataReader + (size) < unmovedDataVariable + BE_PACKET_MAXIMUM_DATA), "Attempted to over-read the data buffer\n");

#define BE_PACKET_PARSER_INTERNAL_GET(variable, size, type) \
do {                                                        \
    BE_PACKET_PARSER_INTERNAL_CHECK_SIZE(sizeof(type));     \
    variable = (type) dataReader[0];                        \
    dataReader += sizeof(type);                             \
} while (BA_BOOLEAN_FALSE)

#define BE_PACKET_PARSER_INTERNAL_CHECK_CODE(expectedCode) \
do {                                                       \
    BE_Packet_DataFieldTypes fieldType;                    \
    BE_PACKET_PARSER_INTERNAL_GET(fieldType, sizeof(BE_Packet_DataFieldTypes), BE_Packet_DataFieldTypes); \
    if (expectedCode != fieldType) {                       \
        BA_LOGGER_ERROR("Unexpected field type: %u != %u", expectedCode, fieldType); \
        /* TODO: Kick/disconnect */                        \
        return;                                            \
    }                                                      \
} while (BA_BOOLEAN_FALSE)

#define BE_PACKET_PARSER_GET_INTEGER(variable) \
BE_PACKET_PARSER_INTERNAL_CHECK_CODE(BE_PACKET_DATA_FIELD_TYPE_INTEGER); \
BE_PACKET_PARSER_INTERNAL_GET(variable, int)

// TODO: Make sure the string only contains valid characters
#define BE_PACKET_PARSER_GET_STRING(variable) \
BE_PACKET_PARSER_INTERNAL_CHECK_CODE(BE_PACKET_DATA_FIELD_TYPE_STRING); \
variable = BA_String_CreateEmpty();           \
for (; dataReader < unmovedDataVariable + BE_PACKET_MAXIMUM_DATA; dataReader++) { \
    if (dataReader[0] == '\0') {              \
        dataReader++;                         \
        break;                                \
    }                                         \
    BA_String_AppendCharacter(&variable, dataReader[0]); \
}
