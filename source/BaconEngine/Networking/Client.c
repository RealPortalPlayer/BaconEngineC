#include <SharedEngineCode/Internal/CppHeader.h>
#include <stddef.h>

#include "BaconEngine/Networking/Client.h"
#include "BaconEngine/Debugging/StrictMode.h"

CPP_GUARD_START()
    int clientAmount = 0;
    ConnectedClient* connectedClients[MAX_CLIENTS];

    int GetClientsAmount(void) {
        return clientAmount;
    }

    ConnectedClient** GetClients(void) {
        return connectedClients;
    }

    ConnectedClient* GetClient(unsigned int id) {
        static ConnectedClient* lastClient = NULL;
        STRICT_CHECK(id < MAX_CLIENTS, NULL, "ID out of bounds");

        if (lastClient != NULL && lastClient->id == id)
            return lastClient;

        for (int i = 0; i < clientAmount; ++i) {
            if (connectedClients[i] == NULL)
                break;

            if (connectedClients[i]->id != id)
                continue;

            return connectedClients[i];
        }

        return NULL;
    }

    void KickClient(void/*ConnectedClient* client, const char* reason*/) {
        STRICT_CHECK_NO_RETURN_VALUE(IsServerModeEnabled(), "This function only works on the server");
        // TODO: Remove client.
    }
CPP_GUARD_END()
