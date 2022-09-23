cmake_minimum_required(VERSION 3.21)
project(BaconEngine C)

option(DISABLE_SDL "Don't include SDL." OFF)
option(DISABLE_SDL_TTF "Don't include SDL TTF." OFF)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

option(BE_DISABLE_OFFICIAL_PROJECTS "Don't compile any of the official projects" OFF)
option(BE_DISABLE_TEST_PROJECTS "Don't compile any of the test projects" OFF)
option(BE_DISABLE_OPENGL "Don't compile with OpenGL" OFF)
option(BE_DISABLE_METAL "Don't compile with Metal, does nothing for non-Apple operating systems" OFF)

message(STATUS "Adding dependencies")

if(NOT BE_DISABLE_OPENGL)
    find_package(OpenGL REQUIRED)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(dependencies/glfw)
    add_subdirectory(dependencies/glad)
endif()

if(NOT BE_DISABLE_METAL AND APPLE) # TODO: Metal
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -x objective-c")
endif()

message(STATUS "Adding meta projects.")

add_subdirectory(source/SharedEngineCode)
add_subdirectory(source/BaconLauncher)
add_subdirectory(source/BaconEngine)
add_subdirectory(source/BaconEngineLauncher)

if(NOT BE_DISABLE_OFFICIAL_PROJECTS)
    message(STATUS "Adding official engine clients.")

    add_subdirectory(source/BaconEditor)
    add_subdirectory(source/BaconLauncherUI)
endif()

if(NOT BE_DISABLE_TEST_PROJECTS)
    message(STATUS "Adding test projects.")

    add_subdirectory(test/TestGame)
    add_subdirectory(test/TestClientCPP)
    add_subdirectory(test/StandaloneClientBinary)
    add_subdirectory(test/CustomStandaloneClientBinary)
endif()