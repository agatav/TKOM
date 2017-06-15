#ifndef INTERPRETER_INTERPRETER_HPP
#define INTERPRETER_INTERPRETER_HPP

#include <stack>
#include "../parser/node.hpp"
#include "scope.hpp"
#include "variable.hpp"

namespace lang {
    namespace interpreter {
        class Interpreter {
            Scope globalScope;
            std::stack<Scope> callFrames;

            parser::ScriptNode &script;

        public:
            explicit Interpreter(parser::ScriptNode &script);

            Variable execute(
                    const std::string &functionName,
                    std::vector<Variable> arguments = std::vector<Variable>()
            );

        private:
            void prepareGlobalScope();

            parser::FunctionDefNode &getFunction(const std::string &name);
        };
    }
}

#endif // INTERPRETER_INTERPRETER_HPP