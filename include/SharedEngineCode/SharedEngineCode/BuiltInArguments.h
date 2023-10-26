// Purpose: Defines for all the built-in arguments.
// Created on: 9/11/22 @ 12:40 AM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#define SEC_BUILTINARGUMENTS_HELP "--help"
#define SEC_BUILTINARGUMENTS_HELP_SHORT "-h"
#define SEC_BUILTINARGUMENTS_CLIENT "--client"
#define SEC_BUILTINARGUMENTS_CLIENT_SHORT "-c"
#define SEC_BUILTINARGUMENTS_SERVER "--server"
#define SEC_BUILTINARGUMENTS_SERVER_SHORT "-s"
#define SEC_BUILTINARGUMENTS_NO_STRICT "--no-strict"
#define SEC_BUILTINARGUMENTS_NO_STRICT_SHORT "-ns"
#define SEC_BUILTINARGUMENTS_WIDTH "--width"
#define SEC_BUILTINARGUMENTS_HEIGHT "--height"
#define SEC_BUILTINARGUMENTS_RENDERER "--renderer"
#define SEC_BUILTINARGUMENTS_MONITOR "--monitor"
#define SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING "--disable-ui-rendering"
#define SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING_SHORT "-dur"
#define SEC_BUILTINARGUMENTS_EXIT "--exit"
#define SEC_BUILTINARGUMENTS_CONSOLE "--console"
#define SEC_BUILTINARGUMENTS_VERSION "--version"
#define SEC_BUILTINARGUMENTS_VERSION_SHORT "-v"
#ifdef BA_ALLOW_DEBUG_LOGS
#   define SEC_BUILTINARGUMENTS_DONT_PRINT_STRICT_CHECKS "--dont-print-strict-checks"
#   define SEC_BUILTINARGUMENTS_DONT_PRINT_STRICT_CHECKS_SHORT "-dpsc"
#   define SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOCATION "--dont-print-engine-memory-allocation"
#   define SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOCATION_SHORT "-dpema"
#   define SEC_BUILTINARGUMENTS_DONT_PRINT_COMMAND_REGISTER "--dont-print-command-register"
#   define SEC_BUILTINARGUMENTS_DONT_PRINT_COMMAND_REGISTER_SHORT "-dpcr"
#endif
#define SEC_BUILTINARGUMENTS_ENGINE "--engine"
#define SEC_BUILTINARGUMENTS_ENGINE_SHORT "-e"
