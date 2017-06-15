#ifndef PARSER_NODE_VISITOR_HPP
#define PARSER_NODE_VISITOR_HPP

#include "node.hpp"

namespace lang {
    namespace parser {
#define DEF_VISIT(NodeType) \
        virtual void visit(const NodeType &node) = 0;

        struct NodeVisitor {
            virtual ~NodeVisitor() {};

            DEF_VISIT(ScriptNode);

            DEF_VISIT(FunctionDefNode);

            DEF_VISIT(FunctionDefParamsNode);

            DEF_VISIT(HtmlTemplateNode);

            DEF_VISIT(AttributeNode);

            DEF_VISIT(AttributeValueNode);

            DEF_VISIT(AttributeListNode);

            DEF_VISIT(TextContentNode);

            DEF_VISIT(InjectedValueNode);

            DEF_VISIT(ReturnExpressionNode);

            DEF_VISIT(BlockNode);

            DEF_VISIT(DeclarationExpressionNode);

            DEF_VISIT(ForExpressionNode);

            DEF_VISIT(AssignExpressionNode);

            DEF_VISIT(IfExpressionNode);

            DEF_VISIT(MathExpressionAddNode);

            DEF_VISIT(MathExpressionSubtractNode);

            DEF_VISIT(MathExpressionSingleNode);

            DEF_VISIT(MultiplyExpressionNode);

            DEF_VISIT(DivideExpressionNode);

            DEF_VISIT(MultiplicativeExpressionSingleNode);

            DEF_VISIT(BaseMathExpressionNode);

            DEF_VISIT(FunctionCallNode);

            DEF_VISIT(ParenthesisExpressionNode);

            DEF_VISIT(LogicExpressionOrNode);

            DEF_VISIT(LogicExpressionSingleNode);

            DEF_VISIT(LogicExpressionAndAndNode);

            DEF_VISIT(LogicExpressionAndSingleNode);

            DEF_VISIT(LogicExpressionEqualEqualNode);

            DEF_VISIT(LogicExpressionEqualSingleNode);

            DEF_VISIT(LogicExpressionRelationalLessNode);

            DEF_VISIT(LogicExpressionRelationalGreaterNode);

            DEF_VISIT(LogicExpressionRelationalLessEqualNode);

            DEF_VISIT(LogicExpressionRelationalGreaterEqualNode);

            DEF_VISIT(LogicExpressionRelationalSingleNode);

            DEF_VISIT(ObjectLiteralNode);

            DEF_VISIT(ObjectFieldNode);

            DEF_VISIT(VariableNode);

            DEF_VISIT(IndexExpressionNode);

            DEF_VISIT(AnyTextNode);

            DEF_VISIT(AnyCharNode);

            DEF_VISIT(IdentifierNode);

            DEF_VISIT(StringLiteralNode);

            DEF_VISIT(NumberLiteralNode);

            DEF_VISIT(BooleanLiteralNode);
        };

#undef DEF_VISIT
    }
}

#endif // PARSER_NODE_VISITOR_HPP
