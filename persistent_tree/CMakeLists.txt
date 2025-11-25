add_executable(persistent_tree)

target_include_directories(persistent_tree PRIVATE ${PROJECT_SOURCE_DIR})

target_sources(persistent_tree PRIVATE main.cpp)