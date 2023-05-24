// Purpose: Simplify bitwise operations.
// Created on: 9/12/22 @ 11:36 AM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#define BE_BITWISE_IS_BIT_SET(mainBit, compareBit) ((mainBit & compareBit) == compareBit)
#define BE_BITWISE_SET_BIT(mainBit, bit) mainBit |= bit
#define BE_BITWISE_UNSET_BIT(mainBit, bit) mainBit &= ~bit
#define BE_BITWISE_TOGGLE_BIT(mainBit, bit) \
do {                                        \
    if (BE_BITWISE_IS_BIT_SET(mainBit, bit)) \
        BE_BITWISE_UNSET_BIT(mainBit, bit); \
    else                                    \
        BE_BITWISE_SET_BIT(mainBit, bit);   \
} while (SEC_BOOLEAN_FALSE)
