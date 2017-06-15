#include "scope.hpp"
#include <cassert>

namespace lang {
    namespace interpreter {
        void Scope::put(std::string name, Variable value) {
            assert(variables.find(name) == variables.end());
            variables.emplace(std::make_pair(move(name), std::move(value)));
        }

        void Scope::replace(const std::string &name, Variable value) {
            variables.erase(name);
            put(name, value);
        }

        Variable &Scope::get(const std::string &name) {
            undefined = Variable();
            auto result = variables.find(name);
            return result != variables.end() ? result->second : undefined;
        }
    }
}