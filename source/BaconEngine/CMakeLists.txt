project(BaconEngine)

add_library(BaconEngine STATIC
        Rendering/Window.c
        Rendering/Renderer.c
        ClientInformation.c
        Console/ArgumentManager.c
        Console/Console.c
        Console/EngineCommands.c
        Console/EngineCommands.h
        Rendering/Layer.c
        Rendering/UI.c
        Rendering/EngineLayers.c
        Rendering/EngineUIs.c
        Rendering/UIWindow.c
        Rendering/UIElement.c
        Input/Keyboard.c
        Event.c)

if(NOT DISABLE_SDL)
    set(BACON_ENGINE_LIBRARIES SDL2)
else()
    target_compile_definitions(BaconEngine PUBLIC BACON_ENGINE_DISABLE_SDL)
endif()

if (NOT DISABLE_SDL AND NOT DISABLE_SDL_TTF)
    set(BACON_ENGINE_LIBRARIES ${BACON_ENGINE_LIBRARIES} SDL2_ttf)
else()
    target_compile_definitions(BaconEngine PUBLIC BACON_ENGINE_DISABLE_SDL_TTF)
endif()

target_include_directories(BaconEngine PUBLIC ../../include/BaconEngine)
target_link_libraries(BaconEngine LINK_PUBLIC SharedEngineCode ${BACON_ENGINE_LIBRARIES})

if(MSVC)
    target_compile_options(BaconEngine PRIVATE /W4 /WX)
else()
    target_compile_options(BaconEngine PRIVATE -Wall -Wpedantic -Werror)
endif()