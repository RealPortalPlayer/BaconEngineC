project(Interface)

add_library(Interface STATIC EntryPoint.c)

target_include_directories(Interface PUBLIC ../../include/Interface)

target_link_libraries(Interface LINK_PUBLIC SharedEngineCode)

target_compile_definitions(Interface PUBLIC BE_ENGINE_VERSION="${BE_ENGINE_VERSION}")