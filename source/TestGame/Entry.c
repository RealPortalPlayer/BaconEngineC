#include <BaconEngine/EntryPoint.h>

void TestCommand(Command this, CommandContext context) {
    LOG_INFO("%s", GetArgumentString(context.arguments, "testing", ""));
}

int ClientStart(int argc, char** argv) {
    RegisterCommand("test", "Tests the command handler.", CREATE_ARGUMENTS("testing", 1), &TestCommand);
    return 0;
}

int ClientShutdown(void) {
    return 0;
}

int ClientSupportsServer(void) {
    return 1;
}

const char* GetClientName(void) {
    return "Test";
}