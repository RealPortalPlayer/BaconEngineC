// Purpose: Represent a connected client.
// Created on: 7/29/22 @ 4:52 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#define MAX_CLIENTS 1000

CPP_GUARD_START()
    typedef struct {
        unsigned int id;
        unsigned long long joinedAt;
        int packetErrors; // How many times they've been kicked for packet related issues.
        unsigned long long lastStillAlivePacket;
    } ConnectedClient;

    int GetClientsAmount(void); // Amount of clients connected.
    ConnectedClient** GetClients(void);
    /**
     * @return Null if not connected.
     */
    ConnectedClient* GetClient(unsigned int id);
    void KickClient(void/*ConnectedClient* client, const char* reason*/); // Do not run on client side.
CPP_GUARD_END()
