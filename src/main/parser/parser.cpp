#include "parser.hpp"
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>


using boost::format;
using lang::lexer::Lexeme;
using lang::lexer::LexemeType;

namespace lang {
    namespace parser {
        Parser::Parser(logging::Logger &logger, util::OutputStream<lexer::Lexeme> &lexemes)
                : log(logger), lexemes(lexemes) {
        }

        std::unique_ptr<ScriptNode> Parser::getTree() {
            return parseScript();
        }

        std::unique_ptr<ScriptNode> Parser::parseScript() {
            auto result = std::unique_ptr<ScriptNode>(new ScriptNode);
            while (lexemes.hasNext())
                result->functions.emplace_back(parseFunctionDef());
            return move(result);
        }

        std::unique_ptr<FunctionDefNode> Parser::parseFunctionDef() {
            auto result = std::unique_ptr<FunctionDefNode>(new FunctionDefNode);

            auto def = getLex("'function'");
            if (def.type != LexemeType::FUNCTION) {
                log.error(str(format("Expected 'function', found %1%") % def.text));
                enforceLexType(def, LexemeType::IDENTIFIER);
                log.info(str(format("Assuming %1% is function name") % def.text));
                result->name = def.text;
            } else {
                auto funcName = getLex("function name");
                enforceLexType(funcName, LexemeType::IDENTIFIER);
                result->name = funcName.text;

                auto first = lookupLex("open bracket", 1);

                if (first.type == LexemeType::OPEN_BRACKET) {
                    enforceGetLexType(LexemeType::OPEN_BRACKET);
                    result->htmltemplate = parseHtmlTemplate();
                    enforceGetLexType(LexemeType::CLOSE_BRACKET);
                }

                result->params = parseFunctionDefParams();

                enforceGetLexType(LexemeType::OPEN_BRACE);
                Lexeme semicolon;
                do {
                    result->value = parseExpression();
                    semicolon = lookupLex("semicolon or end of expression list", 1);
                } while (semicolon.type == LexemeType::SEMICOLON && lexemes.take(semicolon));

                enforceGetLexType(LexemeType::CLOSE_BRACE);

            }
            return move(result);
        }

        std::unique_ptr<FunctionDefParamsNode> Parser::parseFunctionDefParams() {
            auto result = std::unique_ptr<FunctionDefParamsNode>(new FunctionDefParamsNode);
            enforceGetLexType(LexemeType::OPEN_PARENTHESIS);

            Lexeme next = lookupLex("function params", 1);
            if (next.type == LexemeType::IDENTIFIER) {
                Lexeme comma;
                do {
                    result->params.emplace_back(parseIdentifier());
                    comma = lookupLex("comma or end of argument list", 1);
                } while (comma.type == LexemeType::COMMA && lexemes.take(comma));
            }

            enforceGetLexType(LexemeType::CLOSE_PARENTHESIS);
            return move(result);
        }

        std::unique_ptr<HtmlTemplateNode> Parser::parseHtmlTemplate() {
            auto result = std::unique_ptr<HtmlTemplateNode>(new HtmlTemplateNode);
            auto first = lookupLex("open angle", 1);

            if (first.type == LexemeType::OPEN_ANGLE) {
                enforceGetLexType(LexemeType::OPEN_ANGLE);
                result->identifier = parseIdentifier();
                auto second = lookupLex("identifier", 1);

                if (second.type == LexemeType::IDENTIFIER) {
                    result->attributes = parseAttributeList();
                }
                enforceGetLexType(LexemeType::CLOSE_ANGLE);

                //auto second = lookupLex("open angle", 1);

                if (second.type == LexemeType::OPEN_ANGLE) {
                    return parseHtmlTemplate();
                } else {
                    result->value.emplace_back(parseTextContent());
                }

                enforceGetLexType(LexemeType::END_ANGLE);
                result->closingParams = parseIdentifier();
                enforceGetLexType(LexemeType::CLOSE_ANGLE);
            }

            return move(result);
        }

        std::unique_ptr<AttributeListNode> Parser::parseAttributeList() {
            auto result = std::unique_ptr<AttributeListNode>(new AttributeListNode);
            auto endOfAttr = lookupLex("End of argument list", 1);
            do {
                result->value.emplace_back(parseAttribute());
            } while (endOfAttr.type == LexemeType::IDENTIFIER && lexemes.take(endOfAttr));
            return move(result);
        }


        std::unique_ptr<AttributeNode> Parser::parseAttribute() {
            auto result = std::unique_ptr<AttributeNode>(new AttributeNode);

            result->identifier = parseIdentifier();

            enforceGetLexType(LexemeType::ASSIGN);
            result->value = parseAttributeValue();

            return move(result);
        }


        std::unique_ptr<AttributeValueNode> Parser::parseAttributeValue() {
            auto result = std::unique_ptr<AttributeValueNode>(new AttributeValueNode);

            auto next = lookupLex("double open brace", 1);
            if (next.type == LexemeType::DOUBLE_OPEN_BRACE) {
                result->value = parseInjectedValue();
            } else {
                result->string = parseString();
            }

            return move(result);
        }

        std::unique_ptr<InjectedValueNode> Parser::parseInjectedValue() {
            auto result = std::unique_ptr<InjectedValueNode>(new InjectedValueNode);
            enforceGetLexType(LexemeType::DOUBLE_OPEN_BRACE);
            result->value = parseIdentifier();
            enforceGetLexType(LexemeType::DOUBLE_CLOSE_BRACE);

            return move(result);
        }

        std::unique_ptr<TextContentNode> Parser::parseTextContent() {
            auto result = std::unique_ptr<TextContentNode>(new TextContentNode);

            auto next = lookupLex("double open brace", 1);
            if (next.type == LexemeType::DOUBLE_OPEN_BRACE) {
                result->value = parseInjectedValue();
            } else {
                result->anytext = parseAnyText();
            }
            return move(result);
        }

        std::unique_ptr<ExpressionNode> Parser::parseExpression() {
            auto first = lookupLex("expression", 1);
            switch (first.type) {
                case LexemeType::LET:
                    return parseDeclarationExpression();

                case LexemeType::FOR:
                    return parseForExpression();

                case LexemeType::IF:
                    return parseIfExpression();

                case LexemeType::RETURN:
                    return parseReturnExpression();

                case LexemeType::NOT:
                case LexemeType::SUBTRACT:
                    return parseLogicExpression(nullptr);

                case LexemeType::STRING:
                case LexemeType::NUMBER:
                    return parseLogicExpression(nullptr);

                case LexemeType::TRUE:
                case LexemeType::FALSE:
                    return parseLiteral();

                case LexemeType::IDENTIFIER:
                    return parseExpressionIdentBegins();

                default:
                    throw ParserException();
            }

        }

        std::unique_ptr<ExpressionNode> Parser::parseExpressionIdentBegins() {
            auto next = lookupLex("expression", 2);
            if (next.type == LexemeType::OPEN_PARENTHESIS) { return parseFunctionCall(); }

            auto var = parseVariable();

            auto second = lookupLex("second of expr", 1);
            if (second.type == LexemeType::ASSIGN) { return parseAssignExpression(move(var)); }

            return move(var);

        }

        std::unique_ptr<DeclarationExpressionNode> Parser::parseDeclarationExpression() {
            auto result = std::unique_ptr<DeclarationExpressionNode>(new DeclarationExpressionNode);

            auto let = enforceGetLexType(LexemeType::LET);
            result->identifier = parseIdentifier();

            auto first = lookupLex("assign", 1);

            if (first.type == LexemeType::ASSIGN) {
                enforceGetLexType(LexemeType::ASSIGN);
                result->value = parseLogicExpression(std::unique_ptr<LogicExpressionNode>());
            }

            return move(result);
        }

        std::unique_ptr<ForExpressionNode> Parser::parseForExpression() {
            auto result = std::unique_ptr<ForExpressionNode>(new ForExpressionNode);

            enforceGetLexType(LexemeType::FOR);
            enforceGetLexType(LexemeType::OPEN_PARENTHESIS);
            result->iterator = parseDeclarationExpression();
            enforceGetLexType(LexemeType::SEMICOLON);
            result->condition = parseLogicExpression(nullptr);
            enforceGetLexType(LexemeType::SEMICOLON);
            result->expression = parseAssignExpression(nullptr);
            enforceGetLexType(LexemeType::CLOSE_PARENTHESIS);
            result->block = parseBlock();

            enforceGetLexType(LexemeType::SEMICOLON);
            return move(result);
        }

        std::unique_ptr<AssignExpressionNode> Parser::parseAssignExpression(std::unique_ptr<VariableNode> var) {
            auto result = std::unique_ptr<AssignExpressionNode>(new AssignExpressionNode);

            result->variable = move(var);
            enforceGetLexType(LexemeType::ASSIGN);
            result->value = parseExpression();

            enforceGetLexType(LexemeType::SEMICOLON);
            return move(result);
        }

        std::unique_ptr<IfExpressionNode> Parser::parseIfExpression() {
            auto result = std::unique_ptr<IfExpressionNode>(new IfExpressionNode);

            enforceGetLexType(LexemeType::IF);
            enforceGetLexType(LexemeType::OPEN_PARENTHESIS);
            result->condition = parseExpression();
            enforceGetLexType(LexemeType::CLOSE_PARENTHESIS);
            result->ifBlock = parseBlock();

            auto next = lookupLex("maybe else", 1);
            if (next.type == LexemeType::ELSE) {
                enforceGetLexType(LexemeType::ELSE);
                result->elseBlock = parseBlock();
            }

            enforceGetLexType(LexemeType::SEMICOLON);
            return move(result);
        }

        std::unique_ptr<BlockNode> Parser::parseBlock() {
            auto result = std::unique_ptr<BlockNode>(new BlockNode);

            enforceGetLexType(LexemeType::OPEN_BRACE);
            result->value = parseExpression();
            enforceGetLexType(LexemeType::CLOSE_BRACE);

            return move(result);
        }

        std::unique_ptr<ReturnExpressionNode> Parser::parseReturnExpression() {
            auto result = std::unique_ptr<ReturnExpressionNode>(new ReturnExpressionNode);

            enforceGetLexType(LexemeType::RETURN);
            result->returnValue = parseLogicExpression(nullptr);

            enforceGetLexType(LexemeType::SEMICOLON);
            return move(result);
        }


        std::unique_ptr<MathExpressionNode> Parser::parseMathExpression(
                std::unique_ptr<ExpressionNode> maybeFirstVar
        ) {
            auto firstExpr = parseMultiplicativeExpression(move(maybeFirstVar));

            auto next = lookupLex("maybe additiveOp", 1);
            switch (next.type) {
                case LexemeType::ADD:
                    enforceGetLexType(LexemeType::ADD);
                    return std::unique_ptr<MathExpressionNode>{
                            new MathExpressionAddNode(move(firstExpr), parseMathExpression(nullptr))
                    };

                case LexemeType::SUBTRACT:
                    enforceGetLexType(LexemeType::SUBTRACT);
                    return std::unique_ptr<MathExpressionNode>{
                            new MathExpressionSubtractNode(move(firstExpr), parseMathExpression(nullptr))
                    };

                default:
                    return std::unique_ptr<MathExpressionNode>{
                            new MathExpressionSingleNode(move(firstExpr))
                    };
            }
        }

        std::unique_ptr<MultiplicativeExpressionNode> Parser::parseMultiplicativeExpression(
                std::unique_ptr<ExpressionNode> maybeFirstVar
        ) {
            auto firstExpr = parseBaseMathExpression(move(maybeFirstVar));

            const auto next = lookupLex("maybe mutliplicativeOp", 1);
            switch (next.type) {
                case LexemeType::MULTIPLY:
                    enforceGetLexType(LexemeType::MULTIPLY);
                    return std::unique_ptr<MultiplicativeExpressionNode>{
                            new MultiplyExpressionNode(move(firstExpr), parseMultiplicativeExpression(nullptr))
                    };

                case LexemeType::DIVIDE:
                    enforceGetLexType(LexemeType::DIVIDE);
                    return std::unique_ptr<MultiplicativeExpressionNode>{
                            new DivideExpressionNode(move(firstExpr), parseMultiplicativeExpression(nullptr))
                    };

                default:
                    return std::unique_ptr<MultiplicativeExpressionNode>{
                            new MultiplicativeExpressionSingleNode(move(firstExpr))
                    };
            }
        }

        std::unique_ptr<BaseMathExpressionNode> Parser::parseBaseMathExpression(
                std::unique_ptr<ExpressionNode> maybeFirstVar
        ) {
            bool negative = false;
            std::unique_ptr<BaseMathExpressionNode> result;

            auto maybeUnary = lookupLex("maybe unary", 1);
            if (!maybeFirstVar && maybeUnary.type == LexemeType::SUBTRACT) {
                enforceGetLexType(LexemeType::SUBTRACT);
                negative = true;
            } else if (!maybeFirstVar && maybeUnary.type == LexemeType::NOT) {
                enforceGetLexType(LexemeType::NOT);
                negative = true;
            }
            auto first = lookupLex("maybe first", 1);
            switch (first.type) {
                case LexemeType::OPEN_PARENTHESIS:
                    result = std::unique_ptr<BaseMathExpressionNode>{
                            new ParenthesisExpressionNode(parseMathExpression(nullptr))
                    };

                case LexemeType::OPEN_BRACE:
                    result = std::unique_ptr<BaseMathExpressionNode>{
                            new ParenthesisExpressionNode(parseObjectLiteral())
                    };

                case LexemeType::IDENTIFIER: {
                    auto second = lookupLex("maybe second", 1);
                    if (second.type == LexemeType::OPEN_PARENTHESIS)
                        result = parseFunctionCall();
                    else result = parseString();
                };
                case LexemeType::NUMBER: {
                    result = parseNumber();
                };

                default:;

            }
            result->negate = negative;
            return move(result);

        }

        std::unique_ptr<FunctionCallNode> Parser::parseFunctionCall() {
            auto result = std::unique_ptr<FunctionCallNode>(new FunctionCallNode);

            result->identifier = parseIdentifier();
            enforceGetLexType(LexemeType::OPEN_PARENTHESIS);

            auto next = lookupLex("arguments", 1);
            if (next.type == LexemeType::CLOSE_PARENTHESIS) {
                enforceGetLexType(LexemeType::CLOSE_PARENTHESIS);
                return move(result);
            }

            Lexeme comma;

            do {
                result->value.emplace_back(parseLogicExpression(nullptr));
                comma = lookupLex("comma or end of argument list", 1);
            } while (comma.type == LexemeType::COMMA && lexemes.take(comma));


            enforceGetLexType(LexemeType::CLOSE_PARENTHESIS);
            return move(result);
        }

        std::unique_ptr<ParenthesisExpressionNode> Parser::parseParenthesisExpression() {
            enforceGetLexType(LexemeType::OPEN_PARENTHESIS);
            std::unique_ptr<ParenthesisExpressionNode> result{
                    new ParenthesisExpressionNode(parseMathExpression(nullptr))
            };
            enforceGetLexType(LexemeType::CLOSE_PARENTHESIS);

            return move(result);
        }

        std::unique_ptr<LogicExpressionNode> Parser::parseLogicExpression(
                std::unique_ptr<ExpressionNode> maybeFirstVar
        ) {
            auto firstExpr = parseLogicExpressionAnd(move(maybeFirstVar));

            auto maybeOrOp = lookupLex("maybe orOp", 1);
            switch (maybeOrOp.type) {
                case LexemeType::OR:
                    enforceGetLexType(LexemeType::OR);
                    enforceGetLexType(LexemeType::SEMICOLON);
                    return std::unique_ptr<LogicExpressionNode>{
                            new LogicExpressionOrNode(move(firstExpr), parseLogicExpression(nullptr))

                    };

                default:
                    enforceGetLexType(LexemeType::SEMICOLON);
                    return std::unique_ptr<LogicExpressionNode>{
                            new LogicExpressionSingleNode(move(firstExpr))
                    };
            }

        }

        std::unique_ptr<LogicExpressionAndNode> Parser::parseLogicExpressionAnd(
                std::unique_ptr<ExpressionNode> maybeFirstVar
        ) {
            auto firstExpr = parseLogicExpressionEqual(move(maybeFirstVar));

            auto maybeAndOp = lookupLex("maybe andOp", 1);
            switch (maybeAndOp.type) {
                case LexemeType::AND:
                    enforceGetLexType(LexemeType::AND);
                    return std::unique_ptr<LogicExpressionAndNode>{
                            new LogicExpressionAndAndNode(move(firstExpr), parseLogicExpressionAnd(nullptr))
                    };

                default:
                    return std::unique_ptr<LogicExpressionAndNode>{
                            new LogicExpressionAndSingleNode(move(firstExpr))
                    };
            }
        }

        std::unique_ptr<LogicExpressionEqualNode> Parser::parseLogicExpressionEqual(
                std::unique_ptr<ExpressionNode> maybeFirstVar
        ) {
            auto firstExpr = parseLogicExpressionRelational(move(maybeFirstVar));

            auto maybeEqualOp = lookupLex("maybe equalOp", 1);
            switch (maybeEqualOp.type) {
                case LexemeType::EQUAL:
                    enforceGetLexType(LexemeType::EQUAL);
                    return std::unique_ptr<LogicExpressionEqualNode>{
                            new LogicExpressionEqualEqualNode(move(firstExpr), parseLogicExpressionEqual(nullptr))
                    };

                default:
                    return std::unique_ptr<LogicExpressionEqualNode>{
                            new LogicExpressionEqualSingleNode(move(firstExpr))
                    };
            }
        }

        std::unique_ptr<LogicExpressionRelationalNode> Parser::parseLogicExpressionRelational(
                std::unique_ptr<ExpressionNode> maybeFirstVar
        ) {
            auto firstExpr = parseMathExpression(move(maybeFirstVar));

            auto maybeRelationalOp = lookupLex("maybe relationalOp", 1);
            switch (maybeRelationalOp.type) {
                case LexemeType::OPEN_ANGLE:
                    enforceGetLexType(LexemeType::OPEN_ANGLE);
                    return std::unique_ptr<LogicExpressionRelationalNode>{
                            new LogicExpressionRelationalLessNode(
                                    move(firstExpr),
                                    parseLogicExpressionRelational(nullptr)
                            )
                    };

                case LexemeType::LESS_EQ:
                    enforceGetLexType(LexemeType::LESS_EQ);
                    return std::unique_ptr<LogicExpressionRelationalNode>{
                            new LogicExpressionRelationalLessEqualNode(
                                    move(firstExpr),
                                    parseLogicExpressionRelational(nullptr)
                            )
                    };

                case LexemeType::CLOSE_ANGLE :
                    enforceGetLexType(LexemeType::CLOSE_ANGLE);
                    return std::unique_ptr<LogicExpressionRelationalNode>{
                            new LogicExpressionRelationalGreaterNode(
                                    move(firstExpr),
                                    parseLogicExpressionRelational(nullptr)
                            )
                    };

                case LexemeType::GREATER_EQ:
                    enforceGetLexType(LexemeType::GREATER_EQ);
                    return std::unique_ptr<LogicExpressionRelationalNode>{
                            new LogicExpressionRelationalGreaterEqualNode(
                                    move(firstExpr),
                                    parseLogicExpressionRelational(nullptr)
                            )
                    };

                default:
                    return std::unique_ptr<LogicExpressionRelationalNode>{
                            new LogicExpressionRelationalSingleNode(move(firstExpr))
                    };
            }
        }

        std::unique_ptr<ObjectLiteralNode> Parser::parseObjectLiteral() {
            auto result = std::unique_ptr<ObjectLiteralNode>(new ObjectLiteralNode);
            enforceGetLexType(LexemeType::OPEN_BRACKET);

            Lexeme next = lookupLex("object literal params", 1);
            if (next.type == LexemeType::IDENTIFIER) {
                Lexeme comma;
                do {
                    result->injection.emplace_back(parseObjectField());
                    comma = lookupLex("comma or end of argument list", 1);
                } while (comma.type == LexemeType::COMMA && lexemes.take(comma));
            }

            enforceGetLexType(LexemeType::CLOSE_BRACKET);
            return move(result);
        }

        std::unique_ptr<ObjectFieldNode> Parser::parseObjectField() {
            auto result = std::unique_ptr<ObjectFieldNode>(new ObjectFieldNode);
            result->identifier = parseIdentifier();
            enforceGetLexType(LexemeType::COLON);
            result->expression = parseLogicExpression(nullptr);
            return move(result);
        }

        std::unique_ptr<VariableNode> Parser::parseVariable() {
            auto result = std::unique_ptr<VariableNode>(new VariableNode);
            result->identifier = parseIdentifier();
            while (lookupLex("index expression", 1).type == LexemeType::OPEN_BRACKET)
                result->indices.emplace_back(parseIndexExpression());
            return move(result);
        }

        std::unique_ptr<AnyTextNode> Parser::parseAnyText() {
            auto result = std::unique_ptr<AnyTextNode>(new AnyTextNode);
            while ((lookupLex("any char", 1).type == LexemeType::IDENTIFIER) ||
                   (lookupLex("any number", 1).type == LexemeType::NUMBER))
                result->chars.emplace_back(parseAnyChar());
            return move(result);
        }

        std::unique_ptr<AnyCharNode> Parser::parseAnyChar() {
            auto result = std::unique_ptr<AnyCharNode>(new AnyCharNode);
            auto item = getLex("any char");
            switch (item.type) {
                case LexemeType::IDENTIFIER:
                    result->identifier = parseIdentifier();
                    return move(result);
                case LexemeType::NUMBER:
                    result->number = parseNumber();
                    return move(result);
                default:
                    throw ParserException();
            }
        }

        std::unique_ptr<IndexExpressionNode> Parser::parseIndexExpression() {
            auto result = std::unique_ptr<IndexExpressionNode>(new IndexExpressionNode);
            enforceGetLexType(LexemeType::OPEN_BRACKET);
            result->value = parseExpression();
            enforceGetLexType(LexemeType::CLOSE_BRACKET);
            return move(result);
        }

        std::unique_ptr<IdentifierNode> Parser::parseIdentifier() {
            auto result = std::unique_ptr<IdentifierNode>(new IdentifierNode);
            result->name = enforceGetLexType(LexemeType::IDENTIFIER).text;
            return move(result);
        }

        std::unique_ptr<BaseMathExpressionNode> Parser::parseString() {
            auto item = getLex("string");
            auto withoutQuotes = item.text.substr(1, item.text.length() - 2);
            return std::unique_ptr<BaseMathExpressionNode>{new StringLiteralNode(move(withoutQuotes))};

        }

        std::unique_ptr<BaseMathExpressionNode> Parser::parseNumber() {
            auto item = getLex("number");
            return std::unique_ptr<BaseMathExpressionNode>{
                    new NumberLiteralNode(boost::lexical_cast<double>(item.text))};

        }

        std::unique_ptr<LiteralNode> Parser::parseLiteral() {
            auto item = getLex("literal");
            switch (item.type) {
                case LexemeType::TRUE:
                    return std::unique_ptr<LiteralNode>{new BooleanLiteralNode(true)};

                case LexemeType::FALSE:
                    return std::unique_ptr<LiteralNode>{new BooleanLiteralNode(false)};

                default:
                    throw ParserException();
            }
        }

        lexer::Lexeme Parser::getLex(const std::string &expected) {
            Lexeme lexeme;
            if (!lexemes.take(lexeme)) {
                log.error(str(format("Expected %1%, found end of file; Aborting") % expected));
                throw ParserException();
            }
            return lexeme;
        }

        void Parser::enforceLexType(const lexer::Lexeme &lexeme, lexer::LexemeType lexemeType) {
            if (lexeme.type != lexemeType) {
                log.error(str(format("Expected lexeme type %1%, got %2%; Aborting")
                              % static_cast<int>(lexemeType) % lexeme.text));
                throw ParserException();
            }
        }

        lexer::Lexeme Parser::enforceGetLexType(lexer::LexemeType lexemeType) {
            auto result = getLex(str(format("[LexemeType: %1%]") % static_cast<int>(lexemeType)));
            enforceLexType(result, lexemeType);
            return result;
        }

        lexer::Lexeme Parser::lookupLex(const std::string &expected, std::size_t index) {
            Lexeme lexeme;
            if (!lexemes.lookup(index, lexeme)) {
                log.error(str(format("Expected %1%, found end of file; Aborting") % expected));
                throw ParserException();
            }
            return lexeme;
        }
    }
}
