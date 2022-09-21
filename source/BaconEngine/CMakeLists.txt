project(BaconEngine)

if(NOT BE_DISABLE_OPENGL)
    set(BE_SOURCE_FILES
            Rendering/OpenGL/OpenGL.c
            Rendering/OpenGL/OpenGL.h
            Rendering/OpenGL/OpenGLWindow.h
            Rendering/OpenGL/OpenGLWindow.c
            Rendering/OpenGL/OpenGLRenderer.c
            Rendering/OpenGL/OpenGLRenderer.h)
endif()

if(NOT BE_DISABLE_METAL AND APPLE)
endif()

add_library(BaconEngine STATIC
        Rendering/Window.c
        Rendering/Renderer.c
        ClientInformation.c
        Console/ArgumentManager.c
        Console/Console.c
        Console/EngineCommands.c
        Console/EngineCommands.h
        Rendering/Layer.c
        Rendering/EngineLayers.c
        Rendering/EngineUIs.c
        Input/Keyboard.c
        EngineMemory.h
        EngineMemory.c
        Storage/DynamicArray.c
        Storage/DynamicDictionary.c
        I18N.c
        Input/Mouse.c
        Rendering/UI.c
        Rendering/PrivateUI.h
        EntryPoint.c
        Rendering/SpecificRendererFunctions.h
        Rendering/SpecificRendererFunctions.c
        ${BE_SOURCE_FILES}
        Rendering/TextMode/TextMode.c
        Rendering/TextMode/TextMode.h
        Rendering/TextMode/TextModeWindow.c
        Rendering/TextMode/TextModeWindow.h)

if(NOT BE_DISABLE_OPENGL)
    set(BE_LIBRARIES glfw OpenGL::GL)
else()
    target_compile_definitions(BaconEngine PUBLIC BE_DISABLE_OPENGL)
endif()

if(BE_DISABLE_METAL OR NOT APPLE)
    target_compile_definitions(BaconEngine PUBLIC BE_DISABLE_METAL)
endif()

target_include_directories(BaconEngine PUBLIC ../../include/BaconEngine)
target_link_libraries(BaconEngine LINK_PUBLIC SharedEngineCode ${BE_LIBRARIES} glad)

if(MSVC)
    target_compile_options(BaconEngine PRIVATE /W4 /WX)
else()
    target_compile_options(BaconEngine PRIVATE -Wall -Wpedantic -Werror)
endif()