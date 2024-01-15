// Purpose: The connected client.
// Created on: 10/22/22 @ 5:03 AM

// Copyright (c) 2022, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <netinet/in.h>

typedef struct {
    unsigned clientId;
    struct sockaddr_in* socket;
} BE_Client_Connected;
