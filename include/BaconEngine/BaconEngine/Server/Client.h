// Purpose: The connected client.
// Created on: 10/22/22 @ 5:03 AM

#pragma once

#include <netinet/in.h>

typedef struct {
    int clientId;
    struct sockaddr_in* clientDescriptor;
} BE_Client_Connected;
