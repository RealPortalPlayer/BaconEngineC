project(BaconLauncherWindows)

add_executable(BaconLauncherWindows WIN32 Main.c)

target_compile_definitions(BaconLauncherWindows PRIVATE BE_WINDOWS_LAUNCHER)

if(NOT BE_DISABLE_PEDANTIC_MODE)
    if(MSVC)
        target_compile_options(BaconLauncherWindows PRIVATE /W4 /WX)
    else()
        target_compile_options(BaconLauncherWindows PRIVATE -Wall -Wpedantic -Werror)
    endif()
endif()

target_link_libraries(BaconLauncherWindows LINK_PRIVATE SharedEngineCode)

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(BaconLauncherWindows PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
    set_target_properties(BaconLauncherWindows PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
endif()

target_compile_definitions(BaconLauncherWindows PUBLIC BE_ENGINE_VERSION="${BE_ENGINE_VERSION}" BE_LAUNCHER_VERSION="${BE_LAUNCHER_VERSION}" BE_ENGINE_NAME="${BE_ENGINE_NAME}")

ba_strip(BaconLauncherWindows)