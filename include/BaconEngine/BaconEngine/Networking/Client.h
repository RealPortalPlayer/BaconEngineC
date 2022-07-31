// Purpose: Represent a connected client.
// Created on: 7/29/22 @ 4:52 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#define MAX_CLIENTS 1000

CPP_GUARD_START()
    typedef struct {
        int id;
        unsigned long long joinedAt;
        int packetErrors; // How many times they've been kicked for packet related issues.
        unsigned long long lastStillAlivePacket;
    } ConnectedClient;

    int GetClientsSize(void); // Amount of clients connected.
    ConnectedClient** GetClients(void);
    /**
     * @return Null if not connected.
     */
    ConnectedClient* GetClient(int id);
    void KickClient(ConnectedClient* client, const char* reason);
CPP_GUARD_END()