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

target_link_libraries("rpn" "m")
target_sources("rpn" PRIVATE "src/tokenizer.c")
target_sources("rpn" PRIVATE "src/vector.c")
target_sources("rpn" PRIVATE "src/parser.c")
target_sources("rpn" PRIVATE "src/simplifier.c")
target_sources("rpn" PRIVATE "src/evaluator.c")
target_sources("rpn" PRIVATE "src/hash_table.c")

target_link_libraries("rpn-testing" "m")
target_sources("rpn-testing" PRIVATE "src/tokenizer.c")
target_sources("rpn-testing" PRIVATE "src/simplifier.c")
target_sources("rpn-testing" PRIVATE "src/vector.c")
target_sources("rpn-testing" PRIVATE "src/parser.c")
target_sources("rpn-testing" PRIVATE "src/evaluator.c")
target_sources("rpn-testing" PRIVATE "src/hash_table.c")


add_compile_options(-Wall -Wextra -Wpedantic -ggdb -03)


