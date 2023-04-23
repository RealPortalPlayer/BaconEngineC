// Purpose: Prevent a header being included in a client binary context
// Created on: 4/22/2023 @ 4:51 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#ifdef BE_CLIENT_BINARY
#   error This file is intended to be used by the engine only
#endif
