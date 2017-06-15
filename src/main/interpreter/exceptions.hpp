#ifndef INTERPRETER_TYPE_EXCEPTION_HPP
#define INTERPRETER_TYPE_EXCEPTION_HPP

#include "../util/generic-exception.hpp"

namespace lang {
    namespace interpreter {
        struct TypeExceptionTag {
        };
        using TypeException = util::GenericException<TypeExceptionTag>;

        struct InterpreterFailureTag {
        };
        using InterpreterFailure = util::GenericException<InterpreterFailureTag>;

        struct IndexOutOfBoundExceptionTag {
        };
        using IndexOutOfBoundException = util::GenericException<IndexOutOfBoundExceptionTag>;

        struct ArgumentExceptionTag {
        };
        using ArgumentException = util::GenericException<ArgumentExceptionTag>;
    }
}

#endif // INTERPRETER_TYPE_EXCEPTION_HPP
