engine_project(NAME TestGame SOURCES Entry.c Layers.c)

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(TestGame PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/TestGame")
    set_target_properties(TestGame PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/TestGame")
endif()