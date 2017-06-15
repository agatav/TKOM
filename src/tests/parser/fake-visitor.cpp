#include "fake-visitor.hpp"

namespace lang {
    namespace parser {
#define FAKE_VISITOR_IMPL_VISIT(type) \
        void FakeVisitor::visit(const type &node) { \
            (type ## _visited).push_back(&node); \
            visitChildren(node); \
        } \
        void FakeVisitor::visitChildren(const type &node)

        FAKE_VISITOR_IMPL_VISIT(ScriptNode) {
            for (const auto &f : node.functions)
                f->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(FunctionDefNode) {
            node.value->visit(*this);
            node.params->visit(*this);
            if (node.htmltemplate) {
                node.htmltemplate.get()->visit(*this);
            }
        }

        FAKE_VISITOR_IMPL_VISIT(FunctionDefParamsNode) {
            for (const auto &p : node.params)
                p->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(HtmlTemplateNode) {
            node.identifier->visit(*this);

            if (node.attributes) {
                node.attributes->visit(*this);
            }

            for (const auto &p : node.htmlValue)
                p->visit(*this);
            node.closingParams->visit(*this);

        }

        FAKE_VISITOR_IMPL_VISIT(AttributeListNode) {
            for (const auto &p : node.value)
                p->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(AttributeNode) {
            node.identifier->visit(*this);
            node.value->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(AttributeValueNode) {
            node.value->visit(*this);
            node.string->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(TextContentNode) {
            node.value->visit(*this);
            node.anytext->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(InjectedValueNode) {
            node.value->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(ReturnExpressionNode) {
            node.returnValue->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(BlockNode) {
            node.value->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(DeclarationExpressionNode) {
            node.identifier->visit(*this);
            if (node.value)
                node.value.get()->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(ForExpressionNode) {
            node.iterator->visit(*this);
            node.condition->visit(*this);
            node.expression->visit(*this);
            node.block->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(AssignExpressionNode) {
            node.variable->visit(*this);
            node.value->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(IfExpressionNode) {
            node.condition->visit(*this);
            node.ifBlock->visit(*this);
            if (node.elseBlock)
                node.elseBlock.get()->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(MathExpressionAddNode) {
            node.left->visit(*this);
            node.right->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(MathExpressionSubtractNode) {
            node.left->visit(*this);
            node.right->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(MathExpressionSingleNode) {
            node.value->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(MultiplyExpressionNode) {
            node.left->visit(*this);
            node.right->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(DivideExpressionNode) {
            node.left->visit(*this);
            node.right->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(MultiplicativeExpressionSingleNode) {
            node.value->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(BaseMathExpressionNode) {
            if (node.logicUnary)
                node.logicUnary.get()->visit(*this);
            if (node.mathLogic)
                node.mathLogic.get()->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(FunctionCallNode) {
            node.identifier->visit(*this);
            for (const auto &p : node.value)
                p->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(ParenthesisExpressionNode) {
            node.expression->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(LogicExpressionOrNode) {
            node.left->visit(*this);
            node.right->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(LogicExpressionSingleNode) {
            node.value->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(LogicExpressionAndAndNode) {
            node.left->visit(*this);
            node.right->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(LogicExpressionAndSingleNode) {
            node.value->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(LogicExpressionEqualEqualNode) {
            node.left->visit(*this);
            node.right->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(LogicExpressionEqualSingleNode) {
            node.value->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(LogicExpressionRelationalLessNode) {
            node.left->visit(*this);
            node.right->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(LogicExpressionRelationalGreaterNode) {
            node.left->visit(*this);
            node.right->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(LogicExpressionRelationalLessEqualNode) {
            node.left->visit(*this);
            node.right->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(LogicExpressionRelationalGreaterEqualNode) {
            node.left->visit(*this);
            node.right->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(LogicExpressionRelationalSingleNode) {
            node.value->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(ObjectLiteralNode) {
            for (const auto &it : node.injection)
                it->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(ObjectFieldNode) {
            node.identifier->visit(*this);
            node.expression->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(VariableNode) {
            node.identifier->visit(*this);
            for (const auto &it : node.indices)
                it->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(IndexExpressionNode) {
            node.value->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(AnyTextNode) {
            for (const auto &it : node.chars)
                it->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(AnyCharNode) {
            node.identifier->visit(*this);
            node.number->visit(*this);
        }

        FAKE_VISITOR_IMPL_VISIT(IdentifierNode) {}

        FAKE_VISITOR_IMPL_VISIT(StringLiteralNode) {}

        FAKE_VISITOR_IMPL_VISIT(NumberLiteralNode) {}

        FAKE_VISITOR_IMPL_VISIT(BooleanLiteralNode) {}
    }
}
