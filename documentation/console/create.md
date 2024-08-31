# Creating a client command

We'd recommend putting each command in their own file, but it's up to your decision.

Let's say you've created the files `MyCommand.h` and `MyCommand.c`

Put the following code inside the header file:
```c++
#pragma once

#include <BaconEngine/Console/Command.h>

void CommandNameHere(BE_Command_Context context);

```

Then, put the following code inside the source file:
```c++
#include "MyCommand.h"

void CommandNameHere(BE_Command_Context context) {
    // XXX: Insert code
}

```

Finally, inside your clients initialize function, put this code:
```c++
BE_Command_Register("command_name_here", "Description here", BE_COMMAND_FLAG_NULL, &CommandNameHere);
```

# Creating an engine command

Go to [BaconEngine/Console/EngineCommands.c](../../source/BaconEngine/Console/EngineCommands.c), and add a function header.

Think of the command name, and place `BE_EngineCommands_` as its prefix. An example would be the `help` command having
the function name `BE_EngineCommands_Help`.

Add this code to the bottom of the `BE_EngineCommands_Initialize` function:
```c++
BE_Command_Register("command_name_here", "Description here", BE_COMMAND_FLAG_NULL, &BE_EngineCommands_CommandNameHere);
```

Finally, add this just before `BA_CPLUSPLUS_SUPPORT_GUARD_END()`:
```c++
void BE_EngineCommands_CommandNameHere(BE_Command_Context context) {
    // XXX: Insert code
}
```