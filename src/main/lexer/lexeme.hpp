#ifndef LEXER_LEXEME_HPP
#define LEXER_LEXEME_HPP

#include <cassert>
#include <string>

namespace lang {
    namespace lexer {
        enum class LexemeType : int {
            OPEN_PARENTHESIS = 1,
            CLOSE_PARENTHESIS,
            OPEN_BRACE,
            CLOSE_BRACE,
            DOUBLE_OPEN_BRACE,
            DOUBLE_CLOSE_BRACE,
            OPEN_BRACKET,
            CLOSE_BRACKET,
            OPEN_ANGLE,
            END_ANGLE,
            CLOSE_ANGLE,

            SEMICOLON,
            COMMA,
            APOSTROPHE,
            COLON,

            ADD,
            SUBTRACT,
            MULTIPLY,
            DIVIDE,
            EQUAL,
            NOT_EQUAL,
            LESS_EQ,
            GREATER_EQ,
            AND,
            OR,
            ASSIGN,
            NOT,

            FUNCTION,
            IF,
            ELSE,
            FOR,
            LET,
            RETURN,
            TRUE,
            FALSE,

            IDENTIFIER,
            STRING,
            NUMBER,
        };

        struct Lexeme {
            LexemeType type;
            std::string text;
            int line;
        };
    }
}

#endif // LEXER_LEXEME_HPP
