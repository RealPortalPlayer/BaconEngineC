project(BaconEngine)

add_library(BaconEngine STATIC
        Rendering/Window.c
        Rendering/Renderer.c
        ClientInformation.c
        Console/ArgumentManager.c
        Console/Console.c
        Console/EngineCommands.c
        Console/EngineCommands.h
        SceneLogic/Scene.c
        Rendering/Layer.c
        Storage/DynamicArray.c
        Rendering/UI.c
        Rendering/EngineLayers.c
        Rendering/EngineUIs.c
        Rendering/UIWindows.c
        Rendering/UIElements.c)

target_include_directories(BaconEngine PUBLIC ../../include/BaconEngine)
target_link_libraries(BaconEngine LINK_PUBLIC SharedEngineCode SDL2 SDL2_ttf)

if(MSVC)
    target_compile_options(BaconEngine PRIVATE /W4 /WX)
else()
    target_compile_options(BaconEngine PRIVATE -Wall -Wpedantic -Werror)
endif()