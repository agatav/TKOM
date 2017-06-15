#include <gtest/gtest.h>
#include "../../main/lexer/lexer.hpp"
#include "../../main/util/string-output-stream.hpp"

using namespace lang::util;

namespace lang {
    namespace lexer {
        struct LexerTestParams {
            std::string input;
            std::vector<Lexeme> expectedLexemes;
        };

        struct LexerTest : ::testing::TestWithParam<LexerTestParams> {
        };

        TEST_P(LexerTest, check_lexemes) {
            StringOutputStream input(GetParam().input);
            Lexer lexer(input);

            for (std::size_t i = 0; i < GetParam().expectedLexemes.size(); ++i) {
                const Lexeme &expected = GetParam().expectedLexemes[i];

                Lexeme current;
                ASSERT_TRUE(lexer.take(current)) << "unexpected end of lexemes for i=" << i;
                EXPECT_EQ(expected.type, current.type) << "invalid type for i=" << i;
                EXPECT_EQ(expected.text, current.text) << "invalid text for i=" << i;
                EXPECT_EQ(expected.line, current.line) << "invalid line for i=" << i;
            }

            Lexeme dummy;
            EXPECT_FALSE(lexer.take(dummy)) << "unexpected tokens";
        }

        INSTANTIATE_TEST_CASE_P(LexerTests,
                                LexerTest, ::testing::Values(
                LexerTestParams {
                        "",
                        {}
                },
                LexerTestParams {
                        "function", {
                                {LexemeType::FUNCTION, "function", 1}
                        }},

                LexerTestParams {
                        "{{", {
                                {LexemeType::DOUBLE_OPEN_BRACE, "{{", 1}
                        }},
                LexerTestParams {
                        "<", {
                                {LexemeType::OPEN_ANGLE, "<", 1}
                        }},
                LexerTestParams {
                        "</", {
                                {LexemeType::END_ANGLE, "</", 1}
                        }},
                LexerTestParams {
                        "return", {
                                {LexemeType::RETURN, "return", 1}
                        }},
                LexerTestParams {
                        "1", {
                                {LexemeType::NUMBER, "1", 1}
                        }},
                LexerTestParams {
                        ":", {
                                {LexemeType::COLON, ":", 1}
                        }},
                LexerTestParams {
                        "&&", {
                                {LexemeType::AND, "&&", 1}
                        }},
                LexerTestParams {
                        "||", {
                                {LexemeType::OR, "||", 1}
                        }},
                LexerTestParams {
                        ">=", {
                                {LexemeType::GREATER_EQ, ">=", 1}
                        }},
                LexerTestParams {
                        "function Empty[](){}", {
                                {LexemeType::FUNCTION, "function", 1},
                                {LexemeType::IDENTIFIER, "Empty", 1},
                                {LexemeType::OPEN_BRACKET, "[", 1},
                                {LexemeType::CLOSE_BRACKET, "]", 1},
                                {LexemeType::OPEN_PARENTHESIS, "(", 1},
                                {LexemeType::CLOSE_PARENTHESIS, ")", 1},
                                {LexemeType::OPEN_BRACE, "{", 1},
                                {LexemeType::CLOSE_BRACE, "}", 1}
                        }},

                LexerTestParams {
                        "result = array();", {
                                {LexemeType::IDENTIFIER, "result", 1},
                                {LexemeType::ASSIGN, "=", 1},
                                {LexemeType::IDENTIFIER, "array", 1},
                                {LexemeType::OPEN_PARENTHESIS, "(", 1},
                                {LexemeType::CLOSE_PARENTHESIS, ")", 1},
                                {LexemeType::SEMICOLON, ";", 1}
                        }},

                LexerTestParams {
                        "for (let i = 0; i < len(items); i = i+1) \n "
                                "{push(result, func(items[i]))}", {
                                {LexemeType::FOR, "for", 1},
                                {LexemeType::OPEN_PARENTHESIS, "(", 1},
                                {LexemeType::LET, "let", 1},
                                {LexemeType::IDENTIFIER, "i", 1},
                                {LexemeType::ASSIGN, "=", 1},
                                {LexemeType::NUMBER, "0", 1},
                                {LexemeType::SEMICOLON, ";", 1},
                                {LexemeType::IDENTIFIER, "i", 1},
                                {LexemeType::OPEN_ANGLE, "<", 1},
                                {LexemeType::IDENTIFIER, "len", 1},
                                {LexemeType::OPEN_PARENTHESIS, "(", 1},
                                {LexemeType::IDENTIFIER, "items", 1},
                                {LexemeType::CLOSE_PARENTHESIS, ")", 1},
                                {LexemeType::SEMICOLON, ";", 1},
                                {LexemeType::IDENTIFIER, "i", 1},
                                {LexemeType::ASSIGN, "=", 1},
                                {LexemeType::IDENTIFIER, "i", 1},
                                {LexemeType::ADD, "+", 1},
                                {LexemeType::NUMBER, "1", 1},
                                {LexemeType::CLOSE_PARENTHESIS, ")", 1},
                                {LexemeType::OPEN_BRACE, "{", 2},
                                {LexemeType::IDENTIFIER, "push", 2},
                                {LexemeType::OPEN_PARENTHESIS, "(", 2},
                                {LexemeType::IDENTIFIER, "result", 2},
                                {LexemeType::COMMA, ",", 2},
                                {LexemeType::IDENTIFIER, "func", 2},
                                {LexemeType::OPEN_PARENTHESIS, "(", 2},
                                {LexemeType::IDENTIFIER, "items", 2},
                                {LexemeType::OPEN_BRACKET, "[", 2},
                                {LexemeType::IDENTIFIER, "i", 2},
                                {LexemeType::CLOSE_BRACKET, "]", 2},
                                {LexemeType::CLOSE_PARENTHESIS, ")", 2},
                                {LexemeType::CLOSE_PARENTHESIS, ")", 2},
                                {LexemeType::CLOSE_BRACE, "}", 2}
                        }},

                LexerTestParams {
                        "function map(items, func) { \n"
                                "result = array();\n"
                                "for (let i = 0; i < len(items); i = i+1)\n"
                                "push(result, func(items[i]));\n"
                                "return result }", {
                                {LexemeType::FUNCTION, "function", 1},
                                {LexemeType::IDENTIFIER, "map", 1},
                                {LexemeType::OPEN_PARENTHESIS, "(", 1},
                                {LexemeType::IDENTIFIER, "items", 1},
                                {LexemeType::COMMA, ",", 1},
                                {LexemeType::IDENTIFIER, "func", 1},
                                {LexemeType::CLOSE_PARENTHESIS, ")", 1},
                                {LexemeType::OPEN_BRACE, "{", 1},
                                {LexemeType::IDENTIFIER, "result", 2},
                                {LexemeType::ASSIGN, "=", 2},
                                {LexemeType::IDENTIFIER, "array", 2},
                                {LexemeType::OPEN_PARENTHESIS, "(", 2},
                                {LexemeType::CLOSE_PARENTHESIS, ")", 2},
                                {LexemeType::SEMICOLON, ";", 2},
                                {LexemeType::FOR, "for", 3},
                                {LexemeType::OPEN_PARENTHESIS, "(", 3},
                                {LexemeType::LET, "let", 3},
                                {LexemeType::IDENTIFIER, "i", 3},
                                {LexemeType::ASSIGN, "=", 3},
                                {LexemeType::NUMBER, "0", 3},
                                {LexemeType::SEMICOLON, ";", 3},
                                {LexemeType::IDENTIFIER, "i", 3},
                                {LexemeType::OPEN_ANGLE, "<", 3},
                                {LexemeType::IDENTIFIER, "len", 3},
                                {LexemeType::OPEN_PARENTHESIS, "(", 3},
                                {LexemeType::IDENTIFIER, "items", 3},
                                {LexemeType::CLOSE_PARENTHESIS, ")", 3},
                                {LexemeType::SEMICOLON, ";", 3},
                                {LexemeType::IDENTIFIER, "i", 3},
                                {LexemeType::ASSIGN, "=", 3},
                                {LexemeType::IDENTIFIER, "i", 3},
                                {LexemeType::ADD, "+", 3},
                                {LexemeType::NUMBER, "1", 3},
                                {LexemeType::CLOSE_PARENTHESIS, ")", 3},
                                {LexemeType::IDENTIFIER, "push", 4},
                                {LexemeType::OPEN_PARENTHESIS, "(", 4},
                                {LexemeType::IDENTIFIER, "result", 4},
                                {LexemeType::COMMA, ",", 4},
                                {LexemeType::IDENTIFIER, "func", 4},
                                {LexemeType::OPEN_PARENTHESIS, "(", 4},
                                {LexemeType::IDENTIFIER, "items", 4},
                                {LexemeType::OPEN_BRACKET, "[", 4},
                                {LexemeType::IDENTIFIER, "i", 4},
                                {LexemeType::CLOSE_BRACKET, "]", 4},
                                {LexemeType::CLOSE_PARENTHESIS, ")", 4},
                                {LexemeType::CLOSE_PARENTHESIS, ")", 4},
                                {LexemeType::SEMICOLON, ";", 4},
                                {LexemeType::RETURN, "return", 5},
                                {LexemeType::IDENTIFIER, "result", 5},
                                {LexemeType::CLOSE_BRACE, "}", 5}
                        }
                }
        ));
    }
}
