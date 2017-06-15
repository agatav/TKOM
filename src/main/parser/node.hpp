#ifndef PARSER_NODE_HPP
#define PARSER_NODE_HPP

#include <vector>
#include <memory>


namespace lang {
    namespace parser {
        struct NodeVisitor;

        struct Node {
            virtual ~Node() {}

            virtual void visit(NodeVisitor &visitor) const = 0;
        };

#define DEF_VISIT_DECL() \
        void visit(NodeVisitor &visitor) const; \
private: \
        void visitChildren(NodeVisitor &visitor) const; \
public:

        struct ScriptNode;
        struct FunctionDefNode;
        struct FunctionDefParamsNode;
        struct HtmlTemplateNode;
        struct AttributeNode;
        struct AttributeValueNode;
        struct AttributeListNode;
        struct TextContentNode;
        struct InjectedValueNode;
        struct ExpressionNode;
        struct BlockNode;
        struct ReturnExpressionNode;
        struct DeclarationExpressionNode;
        struct ForExpressionNode;
        struct AssignExpressionNode;
        struct IfExpressionNode;
        struct MathExpressionNode;
        struct MathExpressionAddNode;
        struct MathExpressionSubtractNode;
        struct MathExpressionSingleNode;
        struct MultiplicativeExpressionNode;
        struct MultiplyExpressionNode;
        struct DivideExpressionNode;
        struct BaseMathExpressionNode;
        struct FunctionCallNode;
        struct ParenthesisExpressionNode;
        struct LogicExpressionNode;
        struct LogicExpressionOrNode;
        struct LogicExpressionSingleNode;
        struct LogicExpressionAndNode;
        struct LogicExpressionAndAndNode;
        struct LogicExpressionAndSingleNode;
        struct LogicExpressionEqualNode;
        struct LogicExpressionEqualEqualNode;
        struct LogicExpressionEqualSingleNode;
        struct LogicExpressionRelationalNode;
        struct LogicExpressionRelationalLessNode;
        struct LogicExpressionRelationalGreaterNode;
        struct LogicExpressionRelationalLessEqualNode;
        struct LogicExpressionRelationalGreaterEqualNode;
        struct LogicExpressionRelationalSingleNode;
        struct ObjectLiteralNode;
        struct ObjectFieldNode;
        struct VariableNode;
        struct IndexExpressionNode;
        struct IdentifierNode;
        struct AnyTextNode;
        struct AnyCharNode;
        struct LiteralNode;
        struct StringLiteralNode;
        struct NumberLiteralNode;
        struct BooleanLiteralNode;

        struct ScriptNode : Node {
            DEF_VISIT_DECL();
            std::vector<std::unique_ptr<FunctionDefNode>> functions;
        };

        struct FunctionDefNode : Node {
            DEF_VISIT_DECL();
            std::string name;
            std::unique_ptr<HtmlTemplateNode> htmltemplate;
            std::unique_ptr<FunctionDefParamsNode> params;
            std::unique_ptr<ExpressionNode> value;
        };

        struct FunctionDefParamsNode : Node {
            DEF_VISIT_DECL();
            std::vector<std::unique_ptr<IdentifierNode>> params;
        };

        struct HtmlTemplateNode : Node {
            DEF_VISIT_DECL();
            std::unique_ptr<IdentifierNode> identifier;
            std::unique_ptr<AttributeListNode> attributes;
            std::vector<std::unique_ptr<HtmlTemplateNode>> htmlValue;
            std::vector<std::unique_ptr<TextContentNode>> value;
            std::unique_ptr<IdentifierNode> closingParams;
        };

        struct AttributeListNode : HtmlTemplateNode {
            DEF_VISIT_DECL();
            std::vector<std::unique_ptr<AttributeNode>> value;
        };

        struct AttributeNode : AttributeListNode {
            DEF_VISIT_DECL();
            std::unique_ptr<IdentifierNode> identifier;
            std::unique_ptr<AttributeValueNode> value;
        };

        struct AttributeValueNode : AttributeNode {
            DEF_VISIT_DECL();
            std::unique_ptr<InjectedValueNode> value;
            std::unique_ptr<BaseMathExpressionNode> string;
        };

        struct TextContentNode : HtmlTemplateNode {
            DEF_VISIT_DECL();
            std::unique_ptr<TextContentNode> value;
            std::unique_ptr<AnyTextNode> anytext;

        };

        struct InjectedValueNode : TextContentNode {
            DEF_VISIT_DECL();
            std::unique_ptr<IdentifierNode> value;
        };

        struct ExpressionNode : Node {
            // DEF_VISIT_DECL();
        };

        struct BlockNode : ExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<ExpressionNode> value;
        };

        struct ReturnExpressionNode : ExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<LogicExpressionNode> returnValue;
        };

        struct DeclarationExpressionNode : ExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<IdentifierNode> identifier;
            std::unique_ptr<ExpressionNode> value;
        };

        struct ForExpressionNode : ExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<DeclarationExpressionNode> iterator;
            std::unique_ptr<LogicExpressionNode> condition;
            std::unique_ptr<AssignExpressionNode> expression;
            std::unique_ptr<BlockNode> block;
        };

        struct AssignExpressionNode : ExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<VariableNode> variable;
            std::unique_ptr<ExpressionNode> value;
        };

        struct IfExpressionNode : ExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<ExpressionNode> condition;
            std::unique_ptr<BlockNode> ifBlock;
            std::unique_ptr<BlockNode> elseBlock;
        };

        struct MathExpressionNode : ExpressionNode {
        };

        struct MathExpressionAddNode : MathExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<MultiplicativeExpressionNode> left;
            std::unique_ptr<MathExpressionNode> right;

            MathExpressionAddNode() = default;

            MathExpressionAddNode(
                    std::unique_ptr<MultiplicativeExpressionNode> left,
                    std::unique_ptr<MathExpressionNode> right
            ) : left(move(left)), right(move(right)) {}
        };

        struct MathExpressionSubtractNode : MathExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<MultiplicativeExpressionNode> left;
            std::unique_ptr<MathExpressionNode> right;

            MathExpressionSubtractNode() = default;

            MathExpressionSubtractNode(
                    std::unique_ptr<MultiplicativeExpressionNode> left,
                    std::unique_ptr<MathExpressionNode> right
            ) : left(move(left)), right(move(right)) {}
        };

        struct MathExpressionSingleNode : MathExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<MultiplicativeExpressionNode> value;

            MathExpressionSingleNode() = default;

            MathExpressionSingleNode(std::unique_ptr<MultiplicativeExpressionNode> value) : value(move(value)) {}
        };

        struct MultiplicativeExpressionNode : Node {
        };

        struct MultiplyExpressionNode : MultiplicativeExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<BaseMathExpressionNode> left;
            std::unique_ptr<MultiplicativeExpressionNode> right;

            MultiplyExpressionNode() = default;

            MultiplyExpressionNode(
                    std::unique_ptr<BaseMathExpressionNode> left,
                    std::unique_ptr<MultiplicativeExpressionNode> right
            ) : left(move(left)), right(move(right)) {}
        };

        struct DivideExpressionNode : MultiplicativeExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<BaseMathExpressionNode> left;
            std::unique_ptr<MultiplicativeExpressionNode> right;

            DivideExpressionNode() = default;

            DivideExpressionNode(
                    std::unique_ptr<BaseMathExpressionNode> left,
                    std::unique_ptr<MultiplicativeExpressionNode> right
            ) : left(move(left)), right(move(right)) {}
        };

        struct MultiplicativeExpressionSingleNode : MultiplicativeExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<BaseMathExpressionNode> value;

            MultiplicativeExpressionSingleNode() = default;

            MultiplicativeExpressionSingleNode(std::unique_ptr<BaseMathExpressionNode> value)
                    : value(move(value)) {}
        };

        struct BaseMathExpressionNode : MathExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<LogicExpressionSingleNode> logicUnary;
            std::unique_ptr<MathExpressionSingleNode> mathLogic;
            bool negate;
            double value;

            BaseMathExpressionNode() = default;

            BaseMathExpressionNode(double value) : value(value) {}
        };

        struct FunctionCallNode : BaseMathExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<IdentifierNode> identifier;
            std::vector<std::unique_ptr<LogicExpressionNode>> value;
        };

        struct ParenthesisExpressionNode : BaseMathExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<MathExpressionNode> expression;

            ParenthesisExpressionNode() = default;

            ParenthesisExpressionNode(std::unique_ptr<MathExpressionNode> expression)
                    : expression(move(expression)) {}
        };

        struct LogicExpressionNode : ExpressionNode {
        };

        struct LogicExpressionOrNode : LogicExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<LogicExpressionAndNode> left;
            std::unique_ptr<LogicExpressionNode> right;

            LogicExpressionOrNode() = default;

            LogicExpressionOrNode(
                    std::unique_ptr<LogicExpressionAndNode> left,
                    std::unique_ptr<LogicExpressionNode> right
            ) : left(move(left)), right(move(right)) {}
        };

        struct LogicExpressionSingleNode : LogicExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<LogicExpressionAndNode> value;

            LogicExpressionSingleNode() = default;

            LogicExpressionSingleNode(std::unique_ptr<LogicExpressionAndNode> value) : value(move(value)) {}
        };

        struct LogicExpressionAndNode : Node {
        };

        struct LogicExpressionAndAndNode : LogicExpressionAndNode {
            DEF_VISIT_DECL();
            std::unique_ptr<LogicExpressionEqualNode> left;
            std::unique_ptr<LogicExpressionAndNode> right;

            LogicExpressionAndAndNode() = default;

            LogicExpressionAndAndNode(
                    std::unique_ptr<LogicExpressionEqualNode> left,
                    std::unique_ptr<LogicExpressionAndNode> right
            ) : left(move(left)), right(move(right)) {}
        };

        struct LogicExpressionAndSingleNode : LogicExpressionAndNode {
            DEF_VISIT_DECL();
            std::unique_ptr<LogicExpressionEqualNode> value;

            LogicExpressionAndSingleNode() = default;

            LogicExpressionAndSingleNode(std::unique_ptr<LogicExpressionEqualNode> value) : value(move(value)) {}
        };

        struct LogicExpressionEqualNode : Node {
        };

        struct LogicExpressionEqualEqualNode : LogicExpressionEqualNode {
            DEF_VISIT_DECL();
            std::unique_ptr<LogicExpressionRelationalNode> left;
            std::unique_ptr<LogicExpressionEqualNode> right;

            LogicExpressionEqualEqualNode() = default;

            LogicExpressionEqualEqualNode(
                    std::unique_ptr<LogicExpressionRelationalNode> left,
                    std::unique_ptr<LogicExpressionEqualNode> right
            ) : left(move(left)), right(move(right)) {}
        };

        struct LogicExpressionEqualSingleNode : LogicExpressionEqualNode {
            DEF_VISIT_DECL();
            std::unique_ptr<LogicExpressionRelationalNode> value;

            LogicExpressionEqualSingleNode() = default;

            LogicExpressionEqualSingleNode(std::unique_ptr<LogicExpressionRelationalNode> value)
                    : value(move(value)) {}
        };

        struct LogicExpressionRelationalNode : Node {
        };

        struct LogicExpressionRelationalLessNode : LogicExpressionRelationalNode {
            DEF_VISIT_DECL();
            std::unique_ptr<MathExpressionNode> left;
            std::unique_ptr<LogicExpressionRelationalNode> right;

            LogicExpressionRelationalLessNode(
                    std::unique_ptr<MathExpressionNode> left,
                    std::unique_ptr<LogicExpressionRelationalNode> right
            ) : left(move(left)), right(move(right)) {}
        };

        struct LogicExpressionRelationalGreaterNode : LogicExpressionRelationalNode {
            DEF_VISIT_DECL();
            std::unique_ptr<MathExpressionNode> left;
            std::unique_ptr<LogicExpressionRelationalNode> right;

            LogicExpressionRelationalGreaterNode(
                    std::unique_ptr<MathExpressionNode> left,
                    std::unique_ptr<LogicExpressionRelationalNode> right
            ) : left(move(left)), right(move(right)) {}
        };

        struct LogicExpressionRelationalLessEqualNode : LogicExpressionRelationalNode {
            DEF_VISIT_DECL();
            std::unique_ptr<MathExpressionNode> left;
            std::unique_ptr<LogicExpressionRelationalNode> right;

            LogicExpressionRelationalLessEqualNode(
                    std::unique_ptr<MathExpressionNode> left,
                    std::unique_ptr<LogicExpressionRelationalNode> right
            ) : left(move(left)), right(move(right)) {}
        };

        struct LogicExpressionRelationalGreaterEqualNode : LogicExpressionRelationalNode {
            DEF_VISIT_DECL();
            std::unique_ptr<MathExpressionNode> left;
            std::unique_ptr<LogicExpressionRelationalNode> right;

            LogicExpressionRelationalGreaterEqualNode() = default;

            LogicExpressionRelationalGreaterEqualNode(
                    std::unique_ptr<MathExpressionNode> left,
                    std::unique_ptr<LogicExpressionRelationalNode> right
            ) : left(move(left)), right(move(right)) {}
        };

        struct LogicExpressionRelationalSingleNode : LogicExpressionRelationalNode {
            DEF_VISIT_DECL();
            std::unique_ptr<MathExpressionNode> value;

            LogicExpressionRelationalSingleNode() = default;

            LogicExpressionRelationalSingleNode(std::unique_ptr<MathExpressionNode> value)
                    : value(move(value)) {}
        };

        struct ObjectLiteralNode : BaseMathExpressionNode {
            DEF_VISIT_DECL();
            std::vector<std::unique_ptr<ObjectFieldNode>> injection;
        };

        struct ObjectFieldNode : ObjectLiteralNode {
            DEF_VISIT_DECL();
            std::unique_ptr<IdentifierNode> identifier;
            std::unique_ptr<LogicExpressionNode> expression;
        };

        struct LiteralNode : ExpressionNode {
        };

        struct AnyTextNode : LiteralNode {
            DEF_VISIT_DECL();
            std::vector<std::unique_ptr<AnyCharNode>> chars;
        };

        struct AnyCharNode : AnyTextNode {
            DEF_VISIT_DECL();
            std::unique_ptr<IdentifierNode> identifier;
            std::unique_ptr<BaseMathExpressionNode> number;
        };

        struct VariableNode : ExpressionNode {
            DEF_VISIT_DECL();
            std::unique_ptr<IdentifierNode> identifier;
            std::vector<std::unique_ptr<IndexExpressionNode>> indices;
        };

        struct IndexExpressionNode : Node {
            DEF_VISIT_DECL();
            std::unique_ptr<ExpressionNode> value;
        };

        struct IdentifierNode : Node {
            DEF_VISIT_DECL();
            std::string name;
        };

        struct StringLiteralNode : BaseMathExpressionNode {
            DEF_VISIT_DECL();
            std::string value;

            StringLiteralNode() = default;

            StringLiteralNode(std::string value) : value(move(value)) {}
        };

        struct NumberLiteralNode : BaseMathExpressionNode {
            DEF_VISIT_DECL();
            double value;

            NumberLiteralNode() = default;

            NumberLiteralNode(double value) : value(value) {}
        };

        struct BooleanLiteralNode : LiteralNode {
            DEF_VISIT_DECL();
            bool value;

            BooleanLiteralNode() = default;

            BooleanLiteralNode(bool value) : value(value) {}
        };

#undef DEF_VISIT_DECL
    }
}

#endif // PARSER_NODE_HPP
