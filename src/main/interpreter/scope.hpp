#ifndef INTERPRETER_SCOPE_HPP
#define INTERPRETER_SCOPE_HPP

#include "variable.hpp"
#include <string>
#include <unordered_map>

namespace lang {
    namespace interpreter {
        class Scope {
            Variable undefined;
            std::unordered_map<std::string, Variable> variables;

        public:
            void put(std::string name, Variable value);

            void replace(const std::string &name, Variable value);

            Variable &get(const std::string &name);
        };
    }
}

#endif // INTERPRETER_SCOPE_HPP