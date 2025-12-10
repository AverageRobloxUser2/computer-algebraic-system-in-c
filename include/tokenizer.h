#ifndef TOKENIZER_H
#define TOKENIZER_H

enum LexerTokenType {
    TokenTypeOperator,
    TokenTypeUnaryOperator,
    TokenTypeNumber,
    TokenTypeParenthesis,
    TokenTypeVariable,
    TokenTypeFunction,
};

typedef struct {
    enum LexerTokenType type;
    char *start_ptr;
    char *end_ptr;
} LexerToken;

#endif
