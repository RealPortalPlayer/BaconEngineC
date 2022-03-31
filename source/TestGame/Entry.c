#include <BaconEngine/EntryPoint.h>
#include <BaconEngine/Rendering/Window.h>

int ClientStart(int argc, char** argv) {
    SetTitle("Title");
    return 0;
}

int ClientShutdown() {
    return 0;
}

int SupportsServer() {
    return 1;
}

const char* GetName() {
    return "Test";
}