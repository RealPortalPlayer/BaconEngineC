cmake_minimum_required(VERSION 3.21)
project(BaconEngine C)

set(CMAKE_C_STANDARD 17)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

message(STATUS "Adding dependencies.")

add_subdirectory(dependencies/SDL)

message(STATUS "Adding meta projects.")

add_subdirectory(source/SharedEngineCode)
add_subdirectory(source/BaconLauncher)
add_subdirectory(source/BaconEngine)

message(STATUS "Adding test projects.")

add_subdirectory(source/TestGame)