#include "tokenizer.h"
#include "evaluator.h"
#include "vector.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>
#include <unistd.h>

LexerToken *create_token(enum LexerTokenType type, char *start_ptr, char *end_ptr) {
    LexerToken *result = malloc(sizeof(LexerToken));

    result->type = type;
    result->start_ptr = start_ptr;
    result->end_ptr = end_ptr;

    return result;
}


Vector *evaluator_tokenize(Evaluator *evaluator, char *input) {
    int input_length = strlen(input);
    Vector *tokens = vector_new();

    char *current_char_ptr = input;

    bool is_expecting_number = true;
    LexerToken *token = NULL;
    LexerToken *last_token = NULL;


    while(true) {

        if (token != NULL) {
            bool implicit_multiplication = false;

            if (last_token != NULL) {

                // if there are parentheses close to each other like ")("
                if (last_token->type == token->type && token->type == TokenTypeParenthesis) {
                    if (*last_token->start_ptr != *token->start_ptr) {
                        implicit_multiplication = true;
                    }
                // if there is a closing parenthesis and an expression after
                // like ..)2 or ..)sin(4), but not ..)! or ..)*
                } else if (
                        last_token->type == TokenTypeParenthesis &&
                        *last_token->start_ptr == ')' &&
                        token->type != TokenTypeOperator &&
                        token->type != TokenTypeUnaryOperator ) {
                    implicit_multiplication = true;
                // if theres a parenthesis after a number like
                // 2(1 + 2)
                } else if ( last_token->type == TokenTypeNumber &&
                        token->type == TokenTypeParenthesis &&
                        *token->start_ptr == '('
                        ) {
                    implicit_multiplication = true;
                } else if (
                        last_token->type == TokenTypeNumber &&
                        token->type != TokenTypeParenthesis &&
                        token->type != TokenTypeOperator &&
                        token->type != TokenTypeUnaryOperator
                        ) {
                    implicit_multiplication = true;
                // if theres an expression after an important unary operator
                // important means that it goes on the right of the expression
                // 2!(4) or 5!2 or 3!sin(4)
                } else if (
                        last_token->type == TokenTypeUnaryOperator &&
                        token->type != TokenTypeOperator &&
                        token->type != TokenTypeUnaryOperator ) {
                    MathOperator *unary_operator = evaluator_get_unary_operator(evaluator, last_token);
                    if (unary_operator->is_important) {
                        implicit_multiplication = true;
                    }
                }

                if (implicit_multiplication) {
                    vector_pushback(tokens, create_token(TokenTypeOperator, "*", "*"));
                }
            }

            vector_pushback(tokens, token);
            last_token = token;
            token = NULL;
        }

        if (*current_char_ptr == '\0') {
            break;
        }

        if (isspace(*current_char_ptr)) {
            current_char_ptr++;
            continue;
        }


        // current_char is 0-9 or .
        if (isdigit(*current_char_ptr) || *current_char_ptr == '.') {
            char *start_ptr = current_char_ptr;

            while(isdigit(*current_char_ptr) || *current_char_ptr == '.') {
                current_char_ptr++;
            }

            token = create_token(TokenTypeNumber, start_ptr, current_char_ptr);
            is_expecting_number = false;
            continue;
        } else if (isalpha(*current_char_ptr)) {
            // get the char after the last letter :)
            // for example string "sin()"
            //                     ^ current_char_ptr
            // this will get '('
            char *end_ptr = current_char_ptr;
            while (isalpha(*end_ptr)) {
                end_ptr++;
            }

            // function is considered if it has name and parenthesis without whitespace
            // for example "sin(" is considered a  function while "sin (" isnt
            bool is_function_name = (*end_ptr) == '(';

            if (is_function_name) {
                token = create_token(TokenTypeFunction, current_char_ptr, end_ptr);
                is_expecting_number = true;
            } else {
                token = create_token(TokenTypeVariable, current_char_ptr, end_ptr);
                is_expecting_number = false;
            }
            current_char_ptr = end_ptr;

            continue;
        } else if (strchr("()", *current_char_ptr)) {
            // character is a parenthesis
            token = create_token(TokenTypeParenthesis, current_char_ptr, current_char_ptr + 1);

            current_char_ptr++;
            continue;
        }

        // first check unary operators
        // rules are these
        // 1. expects number is true and current char is in unary operators
        // 2. or not expects number and current char is '!'

        bool to_be_continued = false;
        // check unary operators
        for(int i = 0; i < evaluator->operators->length; i++) {
            MathOperator *math_operator = vector_get(evaluator->operators, i);

            if (!math_operator->is_unary) {
                continue;
            }

            if (!math_operator->is_important && !is_expecting_number) {
                continue;
            }

            if (math_operator->operator != *current_char_ptr) {
                continue;
            }

            token = create_token(TokenTypeUnaryOperator, current_char_ptr, current_char_ptr+1);
            current_char_ptr++;
            to_be_continued = true;
            break;
        }

        if (to_be_continued) {
            continue;;
        }

        // check operators
        for(int i = 0; i < evaluator->operators->length; i++) {
            MathOperator *operation = vector_get(evaluator->operators, i);

            if (operation->operator != *current_char_ptr) {
                continue;
            }

            token = create_token(TokenTypeOperator, current_char_ptr, current_char_ptr+1);
            current_char_ptr++;
            to_be_continued = true;
            is_expecting_number = true;
            break;
        }

        if (to_be_continued) {
            continue;;
        }

        return NULL;
    }

    return tokens;
};
