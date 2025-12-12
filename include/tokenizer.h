#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stddef.h>
enum LexerTokenType {
    TokenTypeOperator,
    TokenTypeUnaryOperator,
    TokenTypeNumber,
    TokenTypeParenthesisOpen,
    TokenTypeParenthesisClosed,
    TokenTypeVariable,
    TokenTypeFunction,
};

typedef struct {
    enum LexerTokenType type;
    char *value;
} LexerToken;

typedef struct {
    LexerToken *tokens;
    size_t token_count;
} LexerResult;

LexerResult lex(char *input);

#endif
