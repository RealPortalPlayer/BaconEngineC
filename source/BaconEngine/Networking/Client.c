#include <SharedEngineCode/Internal/CppHeader.h>
#include <stddef.h>

#include "BaconEngine/Networking/Client.h"
#include "BaconEngine/Debugging/StrictMode.h"

CPP_GUARD_START()
    int isCountDirty;
    int currentSlot;
    int lastValidSlot = -1;
    int checkForEmptySlots;
    int slotClearedRecently;
    ConnectedClient* connectedClients[MAX_CLIENTS];

    // TODO: currentSlot will be the current empty slot.
    //       lastValidSlot would be the last slot that is not null.
    //       checkForEmptySlots would be set when someone disconnects while slotClearedRecently is true.
    //       slotClearedRecently would be set if someone disconnects.
    //       currentSlot will be set, unless slotClearedRecently is true.
    //       checkForEmptySlots is only set when the disconnected ID is lower than lastValidSlot.
    //       When checkForEmptySlots is true, check for null up until we hit lastValidSlot.
    //       If lastValidSlot is -1, then currentSlot would be 0.

    int GetClientsSize(void) {
        static int cachedSize;

        if (isCountDirty) {
            cachedSize = 0;

            for (int i = 0; i < MAX_CLIENTS; ++i) {
                if (connectedClients[i] == NULL)
                    continue;

                cachedSize++;
            }
        }

        return cachedSize;
    }

    ConnectedClient** GetClients(void) {
        return connectedClients;
    }

    ConnectedClient* GetClient(int id) {
        STRICT_CHECK(id >= 0 && id < MAX_CLIENTS, NULL, "ID out of bounds");
        return connectedClients[id];
    }

    void KickClient(ConnectedClient* client, const char* reason) {
        STRICT_CHECK_NO_RETURN_VALUE(IsServerModeEnabled(), "This function only works on the server");
        // TODO: Remove client.
    }
CPP_GUARD_END()