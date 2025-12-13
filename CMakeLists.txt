cmake_minimum_required(VERSION 4.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Debug)

project(
    "reverse_polish_notation"
    VERSION 0.0.1
)

include_directories("include")

add_executable("rpn" "src/main.c")
add_executable("test_infix" "src/tests/infix.c")
add_executable("test_postfix" "src/tests/postfix.c")

add_library(
    "converter_to_rpn"
    "src/converter/lexer.c"
    "src/converter/lexed_to_infix.c"
    "src/converter/infix_to_postfix.c"
)

add_library(
    "math_equation"
    "src/math_equation/math_equation.c"
)

target_link_libraries("rpn" "converter_to_rpn")
target_link_libraries("rpn" "math_equation")
target_link_libraries("rpn" "m")

target_link_libraries("test_infix" "converter_to_rpn")
target_link_libraries("test_infix" "math_equation")
target_link_libraries("test_infix" "m")

target_link_libraries("test_postfix" "converter_to_rpn")
target_link_libraries("test_postfix" "math_equation")
target_link_libraries("test_postfix" "m")



add_compile_options(-Wall -Wextra -Wpedantic -ggdb -03)


