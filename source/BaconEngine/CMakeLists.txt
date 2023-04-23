project(BaconEngine)

if(NOT BE_DISABLE_OPENGL)
    set(BE_PRIVATE_SOURCE_FILES
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
    set(BE_PRIVATE_SOURCE_FILES ${BE_PRIVATE_SOURCE_FILES}
            # Header files
            Platform/Windows/Windows.h
            Platform/Windows/WindowsWindow.h
            Platform/Windows/WindowsRenderer.h

            # Source files
            Platform/Windows/Windows.c
            Platform/Windows/WindowsWindow.c
            Platform/Windows/WindowsRenderer.c)
endif()

if(NOT BE_DISABLE_METAL AND APPLE)
    set(BE_PRIVATE_SOURCE_FILES ${BE_PRIVATE_SOURCE_FILES}
            # Header files
            Platform/Metal/Metal.h
            Platform/Metal/MetalWindow.h

            # Source files
            Platform/Metal/Metal.c
            Platform/Metal/MetalWindow.m)
endif()

set(BE_SOURCE_FILES
        # Header files
        InterfaceFunctions.h

        # Source files
        Rendering/Window.c
        Rendering/Renderer.c
        ClientInformation.c
        Console/ArgumentManager.c
        Console/Console.c
        Rendering/Layer.c
        Input/Keyboard.c
        Storage/DynamicArray.c
        Storage/DynamicDictionary.c
        I18N.c
        Input/Mouse.c
        Rendering/UI.c
        DeltaTime.c
        Debugging/Assert.c
        Debugging/StrictMode.c)

add_library(BaconEngine SHARED ${BE_SOURCE_FILES} ${BE_PRIVATE_SOURCE_FILES}
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
        AntiClientGuard.h

        # Source files
        EntryPoint.c
        Console/EngineCommands.c
        EngineMemory.c
        Platform/SpecificPlatformFunctions.c
        Platform/TextMode/TextMode.c
        Rendering/EngineUIs.c
        Rendering/EngineLayers.c)

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

if(MSVC) # I hate MSVC
    target_compile_definitions(BaconEngine PUBLIC WIN32_LEAN_AND_MEAN -D_CRT_SECURE_NO_WARNINGS)
    target_compile_options(BaconEngine PUBLIC /wd4204)
endif()

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(BaconEngine PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
endif()

# --- PROJECT SEPARATOR ---

project(BaconEngineClient)

add_library(BaconEngineClient STATIC ${BE_SOURCE_FILES})

target_include_directories(BaconEngineClient PUBLIC ../../include/BaconEngine)

target_compile_definitions(BaconEngineClient PUBLIC BE_CLIENT_BINARY)

# TODO: Should we worry about stuff like pedantic mode?

target_link_libraries(BaconEngineClient LINK_PUBLIC Interface)