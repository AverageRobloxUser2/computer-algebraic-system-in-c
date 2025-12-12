#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

bool check_is_digit(char c) {
    return isdigit(c) != 0 || c == '.';
}

bool check_is_parenthasis(char c) {
    return c == '(' || c == ')';
}

bool check_is_operator(char c) {
    char *op_list = "+-*/^!";

    for(size_t i = 0; i < strlen(op_list); i++) {
        if (op_list[i] == c){
            return true;
        }
    }

    return false;
}

bool check_is_variable_or_function(char c) {
    // variables could be
    // v_1
    // v_2
    // v2
    return isalnum(c) || c == '_';
}

LexerToken create_lexer_token(
    enum LexerTokenType type,
    char *start_ptr,
    char *end_ptr
) {
    LexerToken result;

    result.type = type;

    char old_end_char = *end_ptr;
    *end_ptr = '\0';

    size_t value_length = strlen(start_ptr);
    char *value = calloc(value_length + 1, sizeof(char));
    strcpy(value, start_ptr);
    *end_ptr = old_end_char;

    result.value = value;

    return result;
}

LexerToken handle_digit(char **current_ptr_ptr) {
    char *current_ptr = *current_ptr_ptr;

    char *start_ptr = current_ptr;

    while(check_is_digit(*current_ptr)) {
        current_ptr++;
    }
    *current_ptr_ptr = current_ptr;

    return create_lexer_token(TokenTypeNumber, start_ptr, current_ptr);
}

LexerToken handle_operator(char **current_ptr_ptr) {
    char *current_ptr = *current_ptr_ptr;

    char *start_ptr = current_ptr;

    // operators are length of 1 so we just get the next char in string
    // and mark that as the end of the operator
    current_ptr++;
    *current_ptr_ptr = current_ptr;

    return create_lexer_token(TokenTypeOperator, start_ptr, current_ptr);
}

LexerToken handle_parenthasis(char **current_ptr_ptr) {
    char *current_ptr = *current_ptr_ptr;

    char *start_ptr = current_ptr;

    // operators are length of 1 so we just get the next char in string
    // and mark that as the end of the operator
    current_ptr++;
    *current_ptr_ptr = current_ptr;

    if (*start_ptr == '(') {
        return create_lexer_token(
            TokenTypeParenthesisOpen,
            start_ptr,
            current_ptr
        ); 
    }

    return create_lexer_token(
        TokenTypeParenthesisClosed,
        start_ptr,
        current_ptr
    ); 

}

LexerToken handle_variable_or_function(char **current_ptr_ptr) {
    char *current_ptr = *current_ptr_ptr;

    char *start_ptr = current_ptr;

    while (check_is_variable_or_function(*current_ptr)) {
        current_ptr++;
    }

    *current_ptr_ptr = current_ptr;

    // function is if it ends with a oepning parenthesis
    // "sin(" -> function
    // "pi" -> not function
    bool is_function = *current_ptr == '(';
    if (is_function) {
        return create_lexer_token(TokenTypeFunction, start_ptr, current_ptr);
    }
    return create_lexer_token(TokenTypeVariable, start_ptr, current_ptr);
}

LexerResult lex(char *input) {
    char *current_ptr = input;

    LexerResult lexer_result;

    lexer_result.token_count = 0;
    lexer_result.tokens = NULL;
    lexer_result.success = false;

    LexerToken token;
    bool add_token = false;

    while (true) {
        if (add_token) {
            // dont do insert a new token on first itteration
            lexer_result.token_count++;
            lexer_result.tokens = reallocarray(
                lexer_result.tokens,
                lexer_result.token_count,
                sizeof(LexerToken)
            );

            lexer_result.tokens[lexer_result.token_count-1] = token;
            add_token = false;
        }

        if (*current_ptr == '\0') {
            break;
        }

        if (iscntrl(*current_ptr) != 0) {
            break;
        }

        if (isblank(*current_ptr) != 0) {
            current_ptr++;
            continue;
        }

        bool is_digit = check_is_digit(*current_ptr); 
        if (is_digit) {
            token = handle_digit(&current_ptr);
            add_token = true;
            continue;
        }

        bool is_operator = check_is_operator(*current_ptr);
        if (is_operator) {
            token = handle_operator(&current_ptr);
            add_token = true;
            continue;
        }

        bool is_variable_or_function = check_is_variable_or_function(*current_ptr);

        if (is_variable_or_function) {
            token = handle_variable_or_function(&current_ptr);
            add_token = true;
            continue;
        }

        bool is_parenthasis = check_is_parenthasis(*current_ptr);
        if (is_parenthasis) {
            token = handle_parenthasis(&current_ptr);
            add_token = true;
            continue;
        }

        printf("Failed on %s", current_ptr);

        return lexer_result;
    }

    lexer_result.success = true;
    return lexer_result;
}
