project(BaconEngine)

add_library(BaconEngine STATIC Rendering/Window.c Rendering/Renderer.c Rendering/LayerStack.c)

target_include_directories(BaconEngine PUBLIC ../../include/BaconEngine)
target_link_libraries(BaconEngine LINK_PUBLIC SharedEngineCode SDL2)