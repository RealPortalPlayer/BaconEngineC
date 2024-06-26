cmake_minimum_required(VERSION 3.2)
project(BaconEngine C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(BE_ENGINE_VERSION 1.0.0-incomplete)
set(BE_LAUNCHER_VERSION 5.0.0-alpha)
set(BE_ENGINE_NAME "BaconEngine")

option(BE_DISABLE_OFFICIAL_PROJECTS "Don't compile any of the official projects" OFF)
option(BE_DISABLE_TEST_PROJECTS "Don't compile any of the test projects" OFF)
option(BE_DISABLE_OPENGL "Don't compile with OpenGL" OFF)
option(BE_DISABLE_METAL "Don't compile with Metal, does nothing for non-Apple operating systems" OFF)
option(BE_DONT_STRIP "Don't remove debug information (no stripping)" OFF)
option(BE_DONT_SET_BINARY_OUTPUT_PATH "Don't change the binary output path" OFF)
option(BE_NO_ENGINE_COMMANDS "Disable every engine command" OFF)
option(BE_NO_IDE_FOLDERS "Don't make any IDE folders" ON)
option(BE_DISABLE_PEDANTIC_MODE "Don't enable pedantic checks" OFF)
option(BE_DISABLE_NETWORK "Disable networking code" OFF)
set(BE_GLFW_PATH "" CACHE STRING "Use GLFW from the specified path instead of trying to find it")
set(BE_ASAN_SANITIZER "" CACHE STRING "Set what ASAN sanitizer to use")

macro(engine_project)
    cmake_parse_arguments(PARSED "STANDALONE" "NAME" "SOURCES" ${ARGN})

    if(NOT PARSED_NAME)
        message(FATAL_ERROR "Your client requires a name")
    endif()

    project("${PARSED_NAME}")

    if(NOT PARSED_STANDALONE)
        message(STATUS "Creating binary client: ${PARSED_NAME}")
        add_library("${PARSED_NAME}" SHARED ${PARSED_SOURCES})
        target_link_libraries("${PARSED_NAME}" LINK_PRIVATE BaconEngineClient)
        set_target_properties("${PARSED_NAME}" PROPERTIES OUTPUT_NAME "Binary")

        if(EMSCRIPTEN)
            set_target_properties("${PARSED_NAME}" PROPERTIES LINK_FLAGS "-sSIDE_MODULE=1")
        endif()
    else()
        message(STATUS "Creating standalone client: ${PARSED_NAME}")
        add_executable("${PARSED_NAME}" ${PARSED_SOURCES})
        target_link_libraries("${PARSED_NAME}" LINK_PRIVATE BaconEngineLauncher)

        if(EMSCRIPTEN)
            set_target_properties("${PARSED_NAME}" PROPERTIES LINK_FLAGS "-sMAIN_MODULE=1")
        endif()
    endif()

    ba_strip("${PARSED_NAME}")
endmacro()

if(NOT EMSCRIPTEN)
    get_cmake_property(BE_SUPPORTS_SHARED_LIBS TARGET_SUPPORTS_SHARED_LIBS)

    if(NOT BE_SUPPORTS_SHARED_LIBS)
        message(FATAL_ERROR "Your platform doesn't support shared libraries, cannot continue")
    endif()
else()
    # No clue why Emscripten says it doesn't support shared libraries
    set_property(GLOBAL PROPERTY LINK_FLAGS "-sLINKABLE=1")
    set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".wasm")
endif()

message(STATUS "Adding dependencies")

if(NOT BE_NO_IDE_FOLDERS)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set_property(GLOBAL PROPERTY FOLDER "Dependencies")
endif()

if(NOT BE_DISABLE_OPENGL)
    message(STATUS "Finding OpenGL")
    find_package(OpenGL)

    if(NOT OPENGL_FOUND)
        message(WARNING "Failed to find OpenGL, disabling")
        set(BE_DISABLE_OPENGL ON)
    else()
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

        if("${BE_GLFW_PATH}" STREQUAL "")
            message(STATUS "Finding GLFW")
            find_package(glfw3)
        else()
            message(STATUS "Adding GLFW")
            add_subdirectory("${BE_GLFW_PATH}" glfw)
            set(glfw3_FOUND ON)
        endif()

        if(NOT glfw3_FOUND)
            message(WARNING "Failed to find GLFW, disabling OpenGL")
            set(BE_DISABLE_OPENGL ON)
        else()
            add_subdirectory(dependencies/glad)
        endif()
    endif()
endif()

if(NOT BE_DISABLE_METAL AND APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -x objective-c -fobjc-arc")
endif()

if(NOT BE_NO_IDE_FOLDERS)
    set_property(GLOBAL PROPERTY FOLDER "Official")
endif()

add_subdirectory(dependencies/zip)
add_subdirectory(dependencies/BaconAPI)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_definitions(DEBUG BA_ALLOW_DEBUG_LOGS)
elseif(BA_ALLOW_DEBUG_LOGS)
    add_compile_definitions(BA_ALLOW_DEBUG_LOGS)
endif()

message(STATUS "Adding meta projects")

add_subdirectory(source/SharedEngineCode)
add_subdirectory(source/BaconLauncher)
add_subdirectory(source/Interface)
add_subdirectory(source/BaconEngine)
add_subdirectory(source/BaconEngineLauncher)

if(NOT BE_DISABLE_OFFICIAL_PROJECTS)
    message(STATUS "Adding official engine clients")

    add_subdirectory(source/BaconEditor)
    add_subdirectory(source/BaconLauncherUI)
endif()

if(NOT BE_DISABLE_TEST_PROJECTS)
    if(NOT BE_NO_IDE_FOLDERS)
        set_property(GLOBAL PROPERTY FOLDER "Tests")
    endif()

    message(STATUS "Adding test projects")

    add_subdirectory(test/TestGame)
    add_subdirectory(test/TestClientCPP)
    add_subdirectory(test/StandaloneClientBinary)
endif()