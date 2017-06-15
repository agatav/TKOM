#ifndef TEST_PARSER_FAKE_VISITOR_HPP
#define TEST_PARSER_FAKE_VISITOR_HPP

#include "../../main/parser/node-visitor.hpp"
#include <vector>

namespace lang {
    namespace parser {
#define FAKE_VISITOR_DECL_VISIT(type) \
        void visit(const type &node) override; \
private: \
        void visitChildren(const type &node); \
public: \
        std::vector<const type*> type ## _visited

        struct FakeVisitor : NodeVisitor {
            FAKE_VISITOR_DECL_VISIT(ScriptNode);

            FAKE_VISITOR_DECL_VISIT(FunctionDefNode);

            FAKE_VISITOR_DECL_VISIT(FunctionDefParamsNode);

            FAKE_VISITOR_DECL_VISIT(HtmlTemplateNode);

            FAKE_VISITOR_DECL_VISIT(AttributeNode);

            FAKE_VISITOR_DECL_VISIT(AttributeValueNode);

            FAKE_VISITOR_DECL_VISIT(AttributeListNode);

            FAKE_VISITOR_DECL_VISIT(TextContentNode);

            FAKE_VISITOR_DECL_VISIT(InjectedValueNode);

            FAKE_VISITOR_DECL_VISIT(ReturnExpressionNode);

            FAKE_VISITOR_DECL_VISIT(BlockNode);

            FAKE_VISITOR_DECL_VISIT(DeclarationExpressionNode);

            FAKE_VISITOR_DECL_VISIT(ForExpressionNode);

            FAKE_VISITOR_DECL_VISIT(AssignExpressionNode);

            FAKE_VISITOR_DECL_VISIT(IfExpressionNode);

            FAKE_VISITOR_DECL_VISIT(MathExpressionAddNode);

            FAKE_VISITOR_DECL_VISIT(MathExpressionSubtractNode);

            FAKE_VISITOR_DECL_VISIT(MathExpressionSingleNode);

            FAKE_VISITOR_DECL_VISIT(MultiplyExpressionNode);

            FAKE_VISITOR_DECL_VISIT(DivideExpressionNode);

            FAKE_VISITOR_DECL_VISIT(MultiplicativeExpressionSingleNode);

            FAKE_VISITOR_DECL_VISIT(BaseMathExpressionNode);

            FAKE_VISITOR_DECL_VISIT(FunctionCallNode);

            FAKE_VISITOR_DECL_VISIT(ParenthesisExpressionNode);

            FAKE_VISITOR_DECL_VISIT(LogicExpressionOrNode);

            FAKE_VISITOR_DECL_VISIT(LogicExpressionSingleNode);

            FAKE_VISITOR_DECL_VISIT(LogicExpressionAndAndNode);

            FAKE_VISITOR_DECL_VISIT(LogicExpressionAndSingleNode);

            FAKE_VISITOR_DECL_VISIT(LogicExpressionEqualEqualNode);

            FAKE_VISITOR_DECL_VISIT(LogicExpressionEqualSingleNode);

            FAKE_VISITOR_DECL_VISIT(LogicExpressionRelationalLessNode);

            FAKE_VISITOR_DECL_VISIT(LogicExpressionRelationalGreaterNode);

            FAKE_VISITOR_DECL_VISIT(LogicExpressionRelationalLessEqualNode);

            FAKE_VISITOR_DECL_VISIT(LogicExpressionRelationalGreaterEqualNode);

            FAKE_VISITOR_DECL_VISIT(LogicExpressionRelationalSingleNode);

            FAKE_VISITOR_DECL_VISIT(ObjectLiteralNode);

            FAKE_VISITOR_DECL_VISIT(ObjectFieldNode);

            FAKE_VISITOR_DECL_VISIT(VariableNode);

            FAKE_VISITOR_DECL_VISIT(IndexExpressionNode);

            FAKE_VISITOR_DECL_VISIT(AnyTextNode);

            FAKE_VISITOR_DECL_VISIT(AnyCharNode);

            FAKE_VISITOR_DECL_VISIT(IdentifierNode);

            FAKE_VISITOR_DECL_VISIT(StringLiteralNode);

            FAKE_VISITOR_DECL_VISIT(NumberLiteralNode);

            FAKE_VISITOR_DECL_VISIT(BooleanLiteralNode);
        };

#undef FAKE_VISITOR_DECL_VISIT
    }
}

#endif // TEST_PARSER_FAKE_VISITOR_HPP
