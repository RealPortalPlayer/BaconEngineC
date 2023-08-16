project(BaconLauncher C)

add_executable(BaconLauncher Main.c)

if(NOT BE_DISABLE_PEDANTIC_MODE)
    if(MSVC)
        target_compile_options(BaconLauncher PRIVATE /W4 /WX)
    else()
        target_compile_options(BaconLauncher PRIVATE -Wall -Wpedantic -Werror)
    endif()
endif()

target_link_libraries(BaconLauncher LINK_PRIVATE SharedEngineCode)

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(BaconLauncher PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
    set_target_properties(BaconLauncher PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
endif()

target_compile_definitions(BaconLauncher PUBLIC BE_ENGINE_VERSION="${BE_ENGINE_VERSION}" BE_LAUNCHER_VERSION="${BE_LAUNCHER_VERSION}")

if(WIN32)
    include(BaconLauncherWindows.cmake)
endif()