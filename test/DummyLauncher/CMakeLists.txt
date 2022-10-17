project(DummyLauncher C)

add_executable(DummyLauncher Main.c)

target_link_libraries(DummyLauncher LINK_PRIVATE SharedEngineCode)

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(DummyLauncher PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
endif()