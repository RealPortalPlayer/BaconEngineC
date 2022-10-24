engine_project(NAME StandaloneClientBinary SOURCES Entry.c STANDALONE)

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(StandaloneClientBinary PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
endif()