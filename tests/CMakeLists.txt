find_package(GTest REQUIRED)

add_executable(tests
    test.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR} ../persistent_tree
)

target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main)

add_test(NAME unit-test COMMAND tests)