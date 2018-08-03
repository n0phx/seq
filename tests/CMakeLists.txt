cmake_minimum_required (VERSION 3.10)

set(TEST_RUNNER seq_test)
add_executable(${TEST_RUNNER} seq_test.cpp)
target_link_libraries(${TEST_RUNNER} PRIVATE seq::seq)

if(MSVC)
    target_compile_options(${TEST_RUNNER} PRIVATE /permissive- /W4
        /w44061 /w44062 /w14242 /w14245 /w14254 /w14287 /we4289
        /w14296 /w14365 /w14388 /w14545 /w14546 /w14547 /w14549
        /w14555 /w14619 /w14640 /w14826 /w14905 /w14906 /w14928
        /w15026 /w15027)
else()
    target_compile_options(${TEST_RUNNER} PRIVATE -Wall -Wextra -Wpedantic
        -Wcast-align -Wconversion -Wduplicated-branches -Wduplicated-cond
        -Wold-style-cast -Wshadow -Wsign-conversion -Wunreachable-code
        -Wunused -Wuseless-cast -Wno-unknown-pragmas)
endif()

add_test(NAME ${TEST_RUNNER}
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../..
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TEST_RUNNER})
