cmake_minimum_required(VERSION 3.15)
project(c_hashtable C)

# Use C11 or later (Clang-compatible)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compile the hashtable library (static)
# Assuming your source file will be named hashtable.c
add_library(hashtable STATIC hash_table.c)

# Optional: expose headers for other projects
target_include_directories(hashtable PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Test executable
# Assuming your test source file will be named test_hashtable.c
add_executable(test_hash_table test_hash_table.c hash_table.c hash_node.c)
target_link_libraries(test_hash_table PRIVATE hashtable)

# Enable CTest + register tests
enable_testing()
# add_test(NAME HashtableTests COMMAND test_hashtable)
add_test(NAME HashtableTests COMMAND $<TARGET_FILE:test_hash_table>)

# add_custom_target(run_all_tests ALL
#     COMMAND ${CMAKE_CTEST_COMMAND} -C Debug --output-on-failure
#     DEPENDS test_hashtablecmake --build build --config Debug
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
# )