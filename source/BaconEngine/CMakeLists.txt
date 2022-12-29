project(BaconEngine)

if(NOT BE_DISABLE_OPENGL)
    set(BE_SOURCE_FILES
            # Header files
            Platform/OpenGL/OpenGL.h
            Platform/OpenGL/OpenGLWindow.h
            Platform/OpenGL/OpenGLRenderer.h
            Platform/OpenGL/OpenGLInput.h

            # Source files
            Platform/OpenGL/OpenGL.c
            Platform/OpenGL/OpenGLWindow.c
            Platform/OpenGL/OpenGLRenderer.c
            Platform/OpenGL/OpenGLInput.c)
endif()

if(WIN32)
    set(BE_SOURCE_FILES ${BE_SOURCE_FILES}
            # Header files
            Platform/Windows/Windows.h
            Platform/Windows/WindowsWindow.h
            Platform/Windows/WindowsRenderer.h

            # Source files
            Platform/Windows/Windows.c
            Platform/Windows/WindowsWindow.c
            Platform/Windows/WindowsRenderer.c)
endif()

#if(NOT BE_DISABLE_METAL AND APPLE)
#endif()

add_library(BaconEngine STATIC
        # Header files
        Console/EngineCommands.h
        EngineMemory.h
        Rendering/PrivateUI.h
        Platform/SpecificPlatformFunctions.h
        Platform/TextMode/TextMode.h
        Rendering/PrivateLayer.h
        PrivateDeltaTime.h
        Console/PrivateConsole.h
        Rendering/PrivateRenderer.h
        Rendering/PrivateWindow.h

        # Source files
        Rendering/Window.c
        Rendering/Renderer.c
        ClientInformation.c
        Console/ArgumentManager.c
        Console/Console.c
        Console/EngineCommands.c
        Rendering/Layer.c
        Rendering/EngineLayers.c
        Rendering/EngineUIs.c
        Input/Keyboard.c
        EngineMemory.c
        Storage/DynamicArray.c
        Storage/DynamicDictionary.c
        I18N.c
        Input/Mouse.c
        Rendering/UI.c
        EntryPoint.c
        Platform/SpecificPlatformFunctions.c
        ${BE_SOURCE_FILES}
        Platform/TextMode/TextMode.c
        DeltaTime.c
        Debugging/Assert.c
        Debugging/StrictMode.c)

if(NOT BE_DISABLE_OPENGL)
    set(BE_LIBRARIES glfw OpenGL::GL glad)
else()
    target_compile_definitions(BaconEngine PUBLIC BE_DISABLE_OPENGL)
endif()

if(BE_DISABLE_METAL OR NOT APPLE)
    target_compile_definitions(BaconEngine PUBLIC BE_DISABLE_METAL)
endif()

if(BE_NO_ENGINE_COMMANDS)
    target_compile_definitions(BaconEngine PUBLIC BE_NO_ENGINE_COMMANDS)
endif()

target_include_directories(BaconEngine PUBLIC ../../include/BaconEngine)
target_link_libraries(BaconEngine LINK_PUBLIC SharedEngineCode ${BE_LIBRARIES})

if(NOT BE_DISABLE_PEDANTIC_MODE)
    if(MSVC)
        target_compile_options(BaconEngine PRIVATE /W4 /WX)
    else()
        target_compile_options(BaconEngine PRIVATE -Wall -Wpedantic -Werror)
    endif()
endif()

if(NOT "${BE_ASAN_SANITIZER}" STREQUAL "")
    message(STATUS "Enabling ASAN sanitizer")

    if(MSVC)
        target_compile_options(BaconEngine PUBLIC /fsanitize=${BE_ASAN_SANITIZER})
        target_link_options(BaconEngine PUBLIC /fsanitize=${BE_ASAN_SANITIZER})
        target_compile_definitions(BaconEngine PUBLIC BE_ASAN_ENABLED)
    else()
        target_compile_options(BaconEngine PUBLIC -fsanitize=${BE_ASAN_SANITIZER})
        target_link_options(BaconEngine PUBLIC -fsanitize=${BE_ASAN_SANITIZER})
        target_compile_definitions(BaconEngine PUBLIC BE_ASAN_ENABLED)
    endif()
endif()