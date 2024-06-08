// Purpose: All the official packets
// Created on: 5/18/24 @ 6:59 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "Client.h"

void BE_Packets_SendPing(BE_Client client);
void BE_Packets_SendConnect(void);
void BE_Packets_SendDisconnect(void);
