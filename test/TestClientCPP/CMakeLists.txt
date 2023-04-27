engine_project(NAME TestClientCPP SOURCES Entry.cpp)

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(TestClientCPP PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/TestClientCPP")
    set_target_properties(TestClientCPP PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/TestClientCPP")
endif()