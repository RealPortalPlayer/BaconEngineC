project(BaconEngine)

add_library(BaconEngine STATIC Rendering/Window.c Rendering/Renderer.c ClientInformation.c Console/ArgumentManager.c Console/Console.c Console/EngineCommands.c Console/EngineCommands.h SceneLogic/Scene.c Rendering/Layer.c Storage/DynamicArray.c)

target_include_directories(BaconEngine PUBLIC ../../include/BaconEngine)
target_link_libraries(BaconEngine LINK_PUBLIC SharedEngineCode SDL2)

if(MSVC)
    target_compile_options(BaconEngine PRIVATE /W4 /WX)
else()
    target_compile_options(BaconEngine PRIVATE -Wall -Wpedantic -Werror)
endif()