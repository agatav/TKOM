#include <gtest/gtest.h>
#include "parser/parser.hpp"
#include "lexer/lexer.hpp"
#include "util/string-output-stream.hpp"
#include "logging/logger.hpp"
#include "fake-visitor.hpp"

namespace lang {
    namespace parser {
        struct ParserTest : ::testing::Test {
            std::unique_ptr<ScriptNode> getTree(const std::string &code) {
                util::StringOutputStream codeStream(code);
                lexer::Lexer lexer(codeStream);
                logging::Logger logger;
                Parser parser(logger, lexer);
                return parser.getTree();
            }
        };

        TEST_F(ParserTest, declaration_expression) {
            auto tree = getTree("function foo() {let a = 1 + 3 ;}");

            FakeVisitor visitor;
            tree->visit(visitor);

            ASSERT_EQ(1, visitor.DeclarationExpressionNode_visited.size());
        }

        TEST_F(ParserTest, index_expression) {
            auto tree = getTree("function foo() { a[2;] }");

            FakeVisitor visitor;
            tree->visit(visitor);

            ASSERT_EQ(1, visitor.IndexExpressionNode_visited.size());
        }

        TEST_F(ParserTest, return_expression) {
            auto tree = getTree("function foo() {return 5;;}");

            FakeVisitor visitor;
            tree->visit(visitor);

            ASSERT_EQ(1, visitor.ReturnExpressionNode_visited.size());
        }

        TEST_F(ParserTest, html_template) {
            auto tree = getTree("function Nodes [ <table> </table> ] (nodes) {return 5>1 ;;}");

            FakeVisitor visitor;
            tree->visit(visitor);

            ASSERT_EQ(1, visitor.HtmlTemplateNode_visited.size());
            ASSERT_EQ(1, visitor.ReturnExpressionNode_visited.size());
            ASSERT_EQ(1, visitor.LogicExpressionRelationalGreaterNode_visited.size());
        }


        TEST_F(ParserTest, if_expression) {
            auto tree = getTree("function foo(){if (false){10;} else {2;} ;}");

            FakeVisitor visitor;
            tree->visit(visitor);

            ASSERT_EQ(1, visitor.IfExpressionNode_visited.size());
        }

        TEST_F(ParserTest, unary_minus) {
            auto tree = getTree("function foo() { -3 ;}");

            FakeVisitor visitor;
            tree->visit(visitor);

            ASSERT_EQ(1, visitor.MultiplicativeExpressionSingleNode_visited.size());
            ASSERT_EQ(1, visitor.LogicExpressionSingleNode_visited.size());
        }

        TEST_F(ParserTest, argument_list) {
            auto tree = getTree("function foo(a, b, c) { print(6;)}");

            FakeVisitor visitor;
            tree->visit(visitor);

            ASSERT_EQ(1, visitor.FunctionDefParamsNode_visited.size());
            ASSERT_EQ(3, visitor.FunctionDefParamsNode_visited[0]->params.size());
            ASSERT_EQ(1, visitor.FunctionCallNode_visited.size());
        }

    }
}