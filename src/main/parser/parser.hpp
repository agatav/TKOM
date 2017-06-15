#ifndef PARSER_PARSER_HPP
#define PARSER_PARSER_HPP

#include "../logging/logger.hpp"
#include "../util/output-stream-lookup-buffer.hpp"
#include "../lexer/lexeme.hpp"
#include "node.hpp"
#include <memory>
#include <exception>

namespace lang {
    namespace parser {
        struct ParserException : std::exception {
        };

        class Parser {
            logging::Logger &log;
            util::OutputStreamLookupBuffer<lexer::Lexeme> lexemes;

        public:
            Parser(
                    logging::Logger &logger,
                    util::OutputStream<lexer::Lexeme> &lexemes
            );

            std::unique_ptr<ScriptNode> getTree();

        private:
            std::unique_ptr<ScriptNode> parseScript();

            std::unique_ptr<FunctionDefNode> parseFunctionDef();

            std::unique_ptr<FunctionDefParamsNode> parseFunctionDefParams();

            std::unique_ptr<HtmlTemplateNode> parseHtmlTemplate();

            std::unique_ptr<AttributeNode> parseAttribute();

            std::unique_ptr<AttributeValueNode> parseAttributeValue();

            std::unique_ptr<AttributeListNode> parseAttributeList();

            std::unique_ptr<TextContentNode> parseTextContent();

            std::unique_ptr<InjectedValueNode> parseInjectedValue();

            std::unique_ptr<ExpressionNode> parseExpression();

            std::unique_ptr<ExpressionNode> parseExpressionIdentBegins();

            std::unique_ptr<ReturnExpressionNode> parseReturnExpression();

            std::unique_ptr<BlockNode> parseBlock();

            std::unique_ptr<DeclarationExpressionNode> parseDeclarationExpression();

            std::unique_ptr<ForExpressionNode> parseForExpression();

            std::unique_ptr<AssignExpressionNode> parseAssignExpression(std::unique_ptr<VariableNode> var);

            std::unique_ptr<IfExpressionNode> parseIfExpression();

            std::unique_ptr<MathExpressionNode> parseMathExpression(
                    std::unique_ptr<ExpressionNode> maybeFirstVar
            );

            std::unique_ptr<MultiplicativeExpressionNode> parseMultiplicativeExpression(
                    std::unique_ptr<ExpressionNode> maybeFirstVar
            );

            std::unique_ptr<BaseMathExpressionNode> parseBaseMathExpression(
                    std::unique_ptr<ExpressionNode> maybeFirstVar
            );

            std::unique_ptr<BaseMathExpressionNode> parseString();

            std::unique_ptr<BaseMathExpressionNode> parseNumber();

            std::unique_ptr<FunctionCallNode> parseFunctionCall();

            std::unique_ptr<ParenthesisExpressionNode> parseParenthesisExpression();

            std::unique_ptr<LogicExpressionNode> parseLogicExpression(
                    std::unique_ptr<ExpressionNode> maybeFirstVar
            );

            std::unique_ptr<LogicExpressionAndNode> parseLogicExpressionAnd(
                    std::unique_ptr<ExpressionNode> maybeFirstVar
            );

            std::unique_ptr<LogicExpressionEqualNode> parseLogicExpressionEqual(
                    std::unique_ptr<ExpressionNode> maybeFirstVar
            );

            std::unique_ptr<LogicExpressionRelationalNode> parseLogicExpressionRelational(
                    std::unique_ptr<ExpressionNode> maybeFirstVar
            );

            std::unique_ptr<ObjectLiteralNode> parseObjectLiteral();

            std::unique_ptr<ObjectFieldNode> parseObjectField();

            std::unique_ptr<VariableNode> parseVariable();

            std::unique_ptr<IndexExpressionNode> parseIndexExpression();

            std::unique_ptr<AnyTextNode> parseAnyText();

            std::unique_ptr<AnyCharNode> parseAnyChar();

            std::unique_ptr<IdentifierNode> parseIdentifier();

            std::unique_ptr<LiteralNode> parseLiteral();

            lexer::Lexeme getLex(const std::string &expected);

            void enforceLexType(const lexer::Lexeme &lexeme, lexer::LexemeType lexemeType);

            lexer::Lexeme enforceGetLexType(lexer::LexemeType lexemeType);

            lexer::Lexeme lookupLex(const std::string &expected, std::size_t index);

        };
    }
}

#endif // PARSER_PARSER_HPP
