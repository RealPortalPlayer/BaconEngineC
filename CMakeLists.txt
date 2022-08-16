cmake_minimum_required(VERSION 3.21)
project(BaconEngine C)

option(DISABLE_SDL "Don't include SDL." OFF)
option(DISABLE_SDL_TTF "Don't include SDL TTF." OFF)

set(CMAKE_C_STANDARD 17)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

message(STATUS "Adding dependencies.")

if(NOT DISABLE_SDL)
    add_subdirectory(dependencies/SDL)
else()
    message(STATUS "Ignoring SDL.")
endif()

if(NOT DISABLE_SDL AND NOT DISABLE_SDL_TTF)
    add_subdirectory(dependencies/SDL_ttf)
else()
    message(STATUS "Ignoring SDL TTF.")
endif()

add_subdirectory(dependencies/zlib)

message(STATUS "Adding meta projects.")

add_subdirectory(source/SharedEngineCode)
add_subdirectory(source/BaconLauncher)
add_subdirectory(source/BaconEngine)
add_subdirectory(source/BaconEngineLauncher)

message(STATUS "Adding official engine clients.")

add_subdirectory(source/BaconEditor)
add_subdirectory(source/BaconLauncherUI)

message(STATUS "Adding test projects.")

add_subdirectory(test/TestGame)
add_subdirectory(test/TestClientCPP)
add_subdirectory(test/StandaloneClientBinary)