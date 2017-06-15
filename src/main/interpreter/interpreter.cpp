#include "interpreter.hpp"
#include "../parser/node-visitor.hpp"
#include "exceptions.hpp"
#include <algorithm>
#include <iostream>
#include <cassert>

namespace lang {
    namespace interpreter {
        namespace {
            struct ExecutionContext {
                Scope &globalScope;
                std::vector<Scope> currentScope;

                ExecutionContext(Scope &globalScope, Scope currentScope)
                        : globalScope(globalScope) {
                    this->currentScope.push_back(std::move(currentScope));
                }

                void push() {
                    currentScope.push_back(Scope());
                }

                void pop() {
                    currentScope.pop_back();
                }

                Variable &resolve(const std::string &name) {
                    for (auto it = currentScope.rbegin(); it != currentScope.rend(); ++it) {
                        auto &maybeResult = it->get(name);
                        if (maybeResult.getType() != VariableType::Undefined)
                            return maybeResult;
                    }
                    return globalScope.get(name);
                }

                Scope &top() {
                    return currentScope.back();
                }
            };

            struct Executor : parser::NodeVisitor {
                ExecutionContext &context;
                Variable result;

                Executor(ExecutionContext &context)
                        : context(context) {}

                void visit(const parser::ScriptNode &node) override {
                    assert(false);
                }

                void visit(const parser::FunctionDefNode &node) override {
                    assert(false);
                }

                void visit(const parser::FunctionDefParamsNode &node) override {
                    assert(false);
                }

                void visit(const parser::HtmlTemplateNode &node) override {
                    assert(false);
                }

                void visit(const parser::AttributeNode &node) override {
                    node.value->visit(*this);
                    node.identifier->visit(*this);
                }

                void visit(const parser::AttributeValueNode &node) override {
                    node.value->visit(*this);
                    node.string->visit(*this);
                }

                void visit(const parser::AttributeListNode &node) override {
                    std::vector<Variable> value;
                    for (auto &arg : node.value) {
                        arg->visit(*this);
                        value.push_back(result);
                    }
                    result = result(move(value));
                }

                void visit(const parser::TextContentNode &node) override {
                    node.value->visit(*this);
                    node.anytext->visit(*this);
                }

                void visit(const parser::InjectedValueNode &node) override {
                    node.value->visit(*this);
                }

                void visit(const parser::ReturnExpressionNode &node) override {
                    node.returnValue->visit(*this);
                }

                void visit(const parser::BlockNode &node) override {
                    node.value->visit(*this);
                }

                void visit(const parser::DeclarationExpressionNode &node) override {
                    if (node.value) {
                        node.value.get()->visit(*this);
                    } else {
                        result = Variable();
                    }
                    context.top().put(node.identifier->name, result);
                }

                void visit(const parser::ForExpressionNode &node) override {
                    node.iterator->visit(*this);
                    while (node.condition->visit(*this), result) {
                        node.expression->visit(*this);
                        node.block->visit(*this);
                    }
                }

                void visit(const parser::AssignExpressionNode &node) override {
                    node.value->visit(*this);
                    Variable value = result;
                    Variable *target = &context.resolve(node.variable->identifier->name);
                    for (auto &idx : node.variable->indices) {
                        idx->value->visit(*this);
                        target = &target->getArrayItem(result.getNumberAsSize());
                    }
                    (*target) = value;
                }

                void visit(const parser::IfExpressionNode &node) override {
                    node.condition->visit(*this);
                    if (result) {
                        node.ifBlock->visit(*this);
                    } else if (node.elseBlock) {
                        node.elseBlock.get()->visit(*this);
                    }
                }

                void visit(const parser::MathExpressionAddNode &node) override {
                    node.left->visit(*this);
                    Variable left = result;
                    node.right->visit(*this);
                    result = left + result;
                }

                void visit(const parser::MathExpressionSubtractNode &node) override {
                    node.left->visit(*this);
                    Variable left = result;
                    node.right->visit(*this);
                    result = left - result;
                }

                void visit(const parser::MathExpressionSingleNode &node) override {
                    node.value->visit(*this);
                }

                void visit(const parser::MultiplyExpressionNode &node) override {
                    node.left->visit(*this);
                    Variable left = result;
                    node.right->visit(*this);
                    result = left * result;
                }

                void visit(const parser::DivideExpressionNode &node) override {
                    node.left->visit(*this);
                    Variable left = result;
                    node.right->visit(*this);
                    result = left / result;
                }

                void visit(const parser::MultiplicativeExpressionSingleNode &node) override {
                    node.value->visit(*this);
                }

                void visit(const parser::BaseMathExpressionNode &node) override {
                    // node.value->visit(*this);
                    if (node.negate)
                        result = -result;
                }

                void visit(const parser::FunctionCallNode &node) override {
                    std::vector<Variable> value;
                    for (auto &arg : node.value) {
                        arg->visit(*this);
                        value.push_back(result);
                    }
                    node.identifier->visit(*this);
                    result = result(move(value));
                }

                void visit(const parser::ParenthesisExpressionNode &node) override {
                    node.expression->visit(*this);
                }

                void visit(const parser::LogicExpressionOrNode &node) override {
                    node.left->visit(*this);
                    if (!result) {
                        node.right->visit(*this);
                    }
                }

                void visit(const parser::LogicExpressionSingleNode &node) override {
                    node.value->visit(*this);
                }

                void visit(const parser::LogicExpressionAndAndNode &node) override {
                    node.left->visit(*this);
                    if (!result) {
                        node.right->visit(*this);
                    }
                }

                void visit(const parser::LogicExpressionAndSingleNode &node) override {
                    node.value->visit(*this);
                }

                void visit(const parser::LogicExpressionEqualEqualNode &node) override {
                    node.left->visit(*this);
                    Variable left = result;
                    node.right->visit(*this);
                    result = left == result;
                }

                void visit(const parser::LogicExpressionEqualSingleNode &node) override {
                    node.value->visit(*this);
                }

                void visit(const parser::LogicExpressionRelationalLessNode &node) override {
                    node.left->visit(*this);
                    Variable left = result;
                    node.right->visit(*this);
                    result = left < result;
                }

                void visit(const parser::LogicExpressionRelationalGreaterNode &node) override {
                    node.left->visit(*this);
                    Variable left = result;
                    node.right->visit(*this);
                    result = left > result;
                }

                void visit(const parser::LogicExpressionRelationalLessEqualNode &node) override {
                    node.left->visit(*this);
                    Variable left = result;
                    node.right->visit(*this);
                    result = left <= result;
                }

                void visit(const parser::LogicExpressionRelationalGreaterEqualNode &node) override {
                    node.left->visit(*this);
                    Variable left = result;
                    node.right->visit(*this);
                    result = left >= result;
                }

                void visit(const parser::LogicExpressionRelationalSingleNode &node) override {
                    node.value->visit(*this);
                }

                void visit(const parser::ObjectFieldNode &node) override {
                    node.identifier->visit(*this);
                    node.expression->visit(*this);
                }

                void visit(const parser::ObjectLiteralNode &node) override {
                    std::vector<Variable> injection;
                    for (auto &arg : node.injection) {
                        arg->visit(*this);
                        injection.push_back(result);
                    }
                    result = result(move(injection));
                }

                void visit(const parser::VariableNode &node) override {
                    node.identifier->visit(*this);
                    Variable current = result;
                    for (auto &idx : node.indices) {
                        idx->visit(*this);
                        current = current.getArrayItem(result.getNumberAsSize());
                    }
                    result = current;
                }

                void visit(const parser::IndexExpressionNode &node) override {
                    node.value->visit(*this);
                }

                void visit(const parser::IdentifierNode &node) override {
                    result = context.resolve(node.name);
                }

                void visit(const parser::AnyCharNode &node) override {
                    node.identifier->visit(*this);
                    node.number->visit(*this);
                }

                void visit(const parser::AnyTextNode &node) override {
                    std::vector<Variable> chars;
                    for (auto &arg : node.chars) {
                        arg->visit(*this);
                        chars.push_back(result);
                    }
                    result = result(move(chars));
                }

                void visit(const parser::StringLiteralNode &node) override {
                    result = node.value;
                }

                void visit(const parser::NumberLiteralNode &node) override {
                    double value[] = {node.value};
                    result = Variable(value, 1);
                }

                void visit(const parser::BooleanLiteralNode &node) override {
                    result = Variable(node.value);
                }
            };
        }

        Interpreter::Interpreter(parser::ScriptNode &script) : script(script) {
            prepareGlobalScope();
        }

        Variable Interpreter::execute(
                const std::string &function,
                std::vector<Variable> arguments
        ) {
            callFrames.push(Scope());
            auto &func = getFunction(function);

            for (std::size_t i = 0; i < func.params->params.size(); ++i) {
                callFrames.top().put(func.params->params[i]->name, arguments[i]);
            }

            ExecutionContext context = ExecutionContext(globalScope, callFrames.top());
            Executor executor(context);

            func.value->visit(executor);
            callFrames.pop();

            return executor.result;
        }

        void Interpreter::prepareGlobalScope() {
            for (auto &func : script.functions) {
                globalScope.put(func->name, Variable(std::bind(
                        &Interpreter::execute,
                        this,
                        func->name,
                        std::placeholders::_1
                )));
            }

            using ftype = std::function<Variable(std::vector<Variable>)>;

            globalScope.put("print", ftype([](std::vector<Variable> args) -> Variable {
                for (auto &a : args)
                    std::cout << a.toString();
                std::cout << std::endl;
                return Variable();
            }));

            globalScope.put("len", ftype([](std::vector<Variable> args) -> Variable {
                if (args.size() != 1)
                    throw ArgumentException("invalid argument count for len");
                double value[] = {static_cast<double>(args[0].getLength())};
                return Variable(value, 1);
            }));

            globalScope.put("array", ftype([](std::vector<Variable> args) -> Variable {
                return args;
            }));

            globalScope.put("push", ftype([](std::vector<Variable> args) -> Variable {
                if (args.size() < 2)
                    throw ArgumentException("nothing to push to array");
                args[0].getArray().insert(args[0].getArray().end(), args.begin() + 1, args.end());
                return args[0];
            }));
        }

        parser::FunctionDefNode &Interpreter::getFunction(const std::string &name) {
            for (auto &f : script.functions)
                if (f->name == name)
                    return *f;
            assert(false);
        }
    }
}