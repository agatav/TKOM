#include "node.hpp"
#include "node-visitor.hpp"

namespace lang {
    namespace parser {
#define DEF_VISIT_IMPL(className) \
        void className::visit(NodeVisitor &visitor) const { \
            visitor.visit(*this); \
        }

        DEF_VISIT_IMPL(ScriptNode);

        DEF_VISIT_IMPL(FunctionDefNode);

        DEF_VISIT_IMPL(FunctionDefParamsNode);

        DEF_VISIT_IMPL(HtmlTemplateNode);

        DEF_VISIT_IMPL(AttributeNode);

        DEF_VISIT_IMPL(AttributeValueNode);

        DEF_VISIT_IMPL(AttributeListNode)

        DEF_VISIT_IMPL(TextContentNode);

        DEF_VISIT_IMPL(InjectedValueNode);

        DEF_VISIT_IMPL(ReturnExpressionNode);

        DEF_VISIT_IMPL(BlockNode);

        DEF_VISIT_IMPL(DeclarationExpressionNode);

        DEF_VISIT_IMPL(ForExpressionNode);

        DEF_VISIT_IMPL(AssignExpressionNode);

        DEF_VISIT_IMPL(IfExpressionNode);

        DEF_VISIT_IMPL(MathExpressionAddNode);

        DEF_VISIT_IMPL(MathExpressionSubtractNode);

        DEF_VISIT_IMPL(MathExpressionSingleNode);

        DEF_VISIT_IMPL(MultiplyExpressionNode);

        DEF_VISIT_IMPL(DivideExpressionNode);

        DEF_VISIT_IMPL(MultiplicativeExpressionSingleNode);

        DEF_VISIT_IMPL(BaseMathExpressionNode);

        DEF_VISIT_IMPL(FunctionCallNode);

        DEF_VISIT_IMPL(ParenthesisExpressionNode);

        DEF_VISIT_IMPL(LogicExpressionOrNode);

        DEF_VISIT_IMPL(LogicExpressionSingleNode);

        DEF_VISIT_IMPL(LogicExpressionAndAndNode);

        DEF_VISIT_IMPL(LogicExpressionAndSingleNode);

        DEF_VISIT_IMPL(LogicExpressionEqualEqualNode);

        DEF_VISIT_IMPL(LogicExpressionEqualSingleNode);

        DEF_VISIT_IMPL(LogicExpressionRelationalLessNode);

        DEF_VISIT_IMPL(LogicExpressionRelationalGreaterNode);

        DEF_VISIT_IMPL(LogicExpressionRelationalLessEqualNode);

        DEF_VISIT_IMPL(LogicExpressionRelationalGreaterEqualNode);

        DEF_VISIT_IMPL(LogicExpressionRelationalSingleNode);

        DEF_VISIT_IMPL(ObjectLiteralNode);

        DEF_VISIT_IMPL(ObjectFieldNode);

        DEF_VISIT_IMPL(VariableNode);

        DEF_VISIT_IMPL(IndexExpressionNode);

        DEF_VISIT_IMPL(AnyTextNode);

        DEF_VISIT_IMPL(AnyCharNode);

        DEF_VISIT_IMPL(IdentifierNode);

        DEF_VISIT_IMPL(StringLiteralNode);

        DEF_VISIT_IMPL(NumberLiteralNode);

        DEF_VISIT_IMPL(BooleanLiteralNode);

#undef DEF_VISIT_IMPL
    }
}
