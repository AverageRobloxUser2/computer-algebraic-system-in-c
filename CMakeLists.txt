cmake_minimum_required(VERSION 4.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_C_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_PARALLEL_LEVEL 16)
add_compile_options(-Wall -Wextra -Wpedantic -ggdb -O3)


project(
    "reverse_polish_notation"
    VERSION 0.0.1
)

include_directories("include")

add_executable("rpn" "src/main.c")
add_executable("test_infix" "src/tests/infix.c")
add_executable("test_postfix" "src/tests/postfix.c")
add_executable("test_rules" "src/tests/test_rules.c")

add_library(
    "cas"

    "src/infix-postfix/lexer.c"
    "src/infix-postfix/lexed_to_infix.c"
    "src/infix-postfix/infix_to_postfix.c"
    
    "src/rules/rules.c"
    "src/rules/math_variable_map.c"

    "src/math_equation/math_equation.c"

    "src/ast_tree/ast_tree.c"
    "src/ast_tree/printing.c"
    "src/ast_tree/postfix_to_ast_tree.c"
    "src/ast_tree/simplify/concat_operators.c"
)

target_link_libraries(
    "rpn"
    cas
)

#
target_link_libraries(
    "test_infix"
    cas
)
#
target_link_libraries(
    "test_postfix"
    cas
)

target_link_libraries(
    "test_rules"
    cas
)


