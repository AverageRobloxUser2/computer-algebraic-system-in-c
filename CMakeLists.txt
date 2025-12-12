cmake_minimum_required(VERSION 4.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

project(
    "reverse_polish_notation"
    VERSION 0.0.1
)

include_directories("include")

add_executable("rpn" "src/main.c")
add_executable("rpn-testing" "src/testing.c")

add_library(
    "simplifier"

    "src/simplifyer/simplifier.c"
    "src/simplifyer/node_to_string.c"
    "src/simplifyer/simplify_add.c"
    "src/simplifyer/simplify_sub.c"
    "src/simplifyer/simplify_multiply.c"
)
add_library(
    "converter"
    "src/converter/evaluator.c"
    "src/converter/parser.c"
    "src/converter/tokenizer.c"
)
add_library("hash_table" "src/hash_table/hash_table.c")
add_library("vector" "src/vector/vector.c")

target_link_libraries("rpn" "simplifier")
target_link_libraries("rpn" "converter")
target_link_libraries("rpn" "hash_table")
target_link_libraries("rpn" "vector")
target_link_libraries("rpn" "m")

target_link_libraries("rpn-testing" "simplifier")
target_link_libraries("rpn-testing" "converter")
target_link_libraries("rpn-testing" "hash_table")
target_link_libraries("rpn-testing" "vector")
target_link_libraries("rpn-testing" "m")



add_compile_options(-Wall -Wextra -Wpedantic -ggdb -03)


