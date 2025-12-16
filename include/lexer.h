#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdbool.h>
#include <stddef.h>
enum LexerTokenType {
    TokenTypeOperator,
    TokenTypeFunction,

    TokenTypeVariable,
    TokenTypeNumber,

    TokenTypeParenthesisOpen,
    TokenTypeParenthesisClosed,
};

typedef struct {
    enum LexerTokenType type;
    char *value;
} LexerToken;

typedef struct {
    LexerToken *tokens;
    size_t token_count;
    bool success;

} LexerResult;

LexerResult lex(char *input);

void free_lexed(LexerResult result);

#endif
