#include "variable.hpp"
#include <functional>
#include <algorithm>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <cmath>
#include "exceptions.hpp"

namespace lang {
    namespace interpreter {
        namespace {
            const std::string variableTypeToStr[] = {
                    "undefined",
                    "number",
                    "string",
                    "boolean",
                    "function",
                    "array"
            };

            const std::size_t typeToLen[] = {
                    0, // undefined
                    1, // number
                    0, // string
                    0, // boolean,
                    0, // function
                    0, // array
            };

            namespace detail {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "InfiniteRecursion"

                template<typename... Types>
                bool checkIfOneOfImpl(VariableType type, VariableType other, Types... args) {
                    return checkIfOneOfImpl(type, other) || checkIfOneOfImpl(type, args...);
                }

                template<>
                bool checkIfOneOfImpl(VariableType type, VariableType other) {
                    return type == other;
                }

#pragma clang diagnostic pop
            }

            template<typename... Types>
            void checkIfOneOf(VariableType type, Types... args) {
                if (!detail::checkIfOneOfImpl(type, args...)) {
                    throw TypeException("Type " + variableTypeToStr[(int) type] + " is not valid for operation");
                }
            }

            template<typename OP>
            Variable vectorOp(VariableType type, const double first[], const double second[], OP op) {
                double result[4];
                const std::size_t len = typeToLen[(int) type];
                for (std::size_t i = 0; i < len; ++i) {
                    result[i] = op(first[i], second[i]);
                }
                return Variable(result, len);
            }
        }

        Variable::Variable(double *numeric, std::size_t len) {
            assert(numeric);
            assert(len > 0 && len <= 4);

            const VariableType typeMap[] = {
                    VariableType::Undefined,
                    VariableType::Number,
            };

            type = typeMap[len];
            std::copy(numeric, numeric + len, this->numeric);
        }

        Variable::Variable(std::function<Variable(std::vector<Variable>)> function)
                : type(VariableType::Function), function(move(function)) {
        }

        Variable::Variable(std::string string) : type(VariableType::String), string(move(string)) {
        }

        Variable::Variable(std::vector<Variable> array) : type(VariableType::Array), array(move(array)) {
        }

        Variable::Variable(bool boolean) : type(VariableType::Boolean), boolean(boolean) {
        }

        Variable::Variable(const Variable &other) noexcept {
            *this = other;
        }

        Variable::Variable(Variable &&other) noexcept {
            *this = std::move(other);
        }

        VariableType Variable::getType() const {
            return type;
        }

        std::size_t Variable::getNumberAsSize() const {
            if (getType() != VariableType::Number)
                throw TypeException("expected number");
            if (ceil(numeric[0]) != numeric[0])
                throw TypeException("expected integer");
            if (numeric[0] < 0.)
                throw IndexOutOfBoundException("value is lower than zero");

            return (size_t) numeric[0];
        }

        Variable Variable::operator()(std::vector<Variable> arguments) const {
            checkIfOneOf(type, VariableType::Function);
            return function(arguments);
        }

        Variable &Variable::getArrayItem(std::size_t index) {
            if (getType() != VariableType::Array)
                throw TypeException("expected array");
            if (array.size() < index)
                throw IndexOutOfBoundException(std::to_string(index));

            return array[index];
        }

        Variable::operator bool() const {
            checkIfOneOf(type, VariableType::Boolean);
            return boolean;
        }

        Variable Variable::operator+(const Variable &other) const {
            checkSameType(other);
            checkIfOneOf(type,
                         VariableType::Number,
                         VariableType::String
            );

            switch (type) {
                case VariableType::Number:
                    return vectorOp(type, numeric, other.numeric, [](auto a, auto b) { return a + b; });

                case VariableType::String:
                    return Variable(string + other.string);

                default:
                    throw InterpreterFailure();
            }
        }

        Variable Variable::operator-(const Variable &other) const {
            checkSameType(other);
            checkIfOneOf(type, VariableType::Number);

            return (*this) + (-other);
        }

        Variable Variable::operator*(const Variable &other) const {
            checkSameType(other);
            checkIfOneOf(type, VariableType::Number);

            switch (type) {
                case VariableType::Number:
                    return vectorOp(type, numeric, other.numeric, [](auto a, auto b) { return a * b; });

                default:
                    throw InterpreterFailure();
            }
        }

        Variable Variable::operator/(const Variable &other) const {
            checkSameType(other);
            checkIfOneOf(type, VariableType::Number);

            switch (type) {
                case VariableType::Number:
                    return vectorOp(type, numeric, other.numeric, [](auto a, auto b) { return a / b; });

                default:
                    throw InterpreterFailure();
            }
        }

        Variable Variable::operator-() const {
            checkIfOneOf(type, VariableType::Number);

            switch (type) {
                case VariableType::Number: {
                    double result[1] = {-numeric[0]};
                    return Variable(result, typeToLen[(int) type]);
                }

                default:
                    throw InterpreterFailure();
            }
        }

        Variable Variable::operator!() const {
            checkIfOneOf(type, VariableType::Boolean);
            return !boolean;
        }

        Variable Variable::operator==(const Variable &other) const {
            if (type != other.type)
                return false;


            switch (type) {
                case VariableType::Number: {
                    const std::size_t len = typeToLen[(int) type];
                    return std::equal(numeric, numeric + len, other.numeric, other.numeric + len);
                }

                case VariableType::String:
                    return string == other.string;

                case VariableType::Boolean:
                    return boolean == other.boolean;

                case VariableType::Function:
                    return false;

                case VariableType::Array: {
                    return std::equal(array.begin(), array.end(), other.array.begin(), other.array.end());
                }

                default:
                    throw InterpreterFailure();
            }
        }

        Variable Variable::operator!=(const Variable &other) const {
            return !(*this == other);
        }

        Variable Variable::operator<(const Variable &other) const {
            return compareRelation(other, [](auto a, auto b) -> bool { return a < b; });
        }

        Variable Variable::operator<=(const Variable &other) const {
            return compareRelation(other, [](auto a, auto b) -> bool { return a <= b; });
        }

        Variable Variable::operator>(const Variable &other) const {
            return compareRelation(other, [](auto a, auto b) -> bool { return a > b; });
        }

        Variable Variable::operator>=(const Variable &other) const {
            return compareRelation(other, [](auto a, auto b) -> bool { return a >= b; });
        }

        void Variable::checkSameType(const Variable &other) const {
            if (type != other.type) {
                throw TypeException(
                        "cannot perform operation on two different types: " +
                        variableTypeToStr[(int) type] + " and " + variableTypeToStr[(int) other.type]);
            }
        }

        std::string Variable::toString() const {
            using boost::adaptors::transformed;
            using boost::algorithm::join;
            using std::bind;
            using std::placeholders::_1;

            switch (type) {
                case VariableType::Undefined:
                    return "undefined";

                case VariableType::Number:
                    return std::to_string(numeric[0]);

                case VariableType::Boolean:
                    return boolean ? "true" : "false";

                case VariableType::Function:
                    return "function";

                case VariableType::Array: {
                    return "[" + join(array | transformed(bind(&Variable::toString, _1)), ", ") + "]";
                }

                case VariableType::String:
                    return string;

                default:
                    throw InterpreterFailure();
            }
        }

        template<typename OP>
        Variable Variable::compareRelation(const Variable &other, OP op) const {
            checkIfOneOf(type, VariableType::Number);
            switch (type) {
                case VariableType::Number:
                    return op(numeric[0], other.numeric[0]);

                default:
                    throw InterpreterFailure();
            }
        }

        double Variable::getNumeric() const {
            checkIfOneOf(type, VariableType::Number);
            return numeric[0];
        }

        std::size_t Variable::getLength() const {
            checkIfOneOf(type, VariableType::String, VariableType::Array);

            switch (type) {
                case VariableType::String:
                    return string.length();

                case VariableType::Array:
                    return array.size();

                default:
                    throw InterpreterFailure();
            }
        }

        std::vector<Variable> &Variable::getArray() {
            checkIfOneOf(type, VariableType::Array);
            return array;
        }

        Variable &Variable::operator=(const Variable &other) noexcept {
            if (this == &other)
                return *this;

            type = other.type;
            switch (other.type) {
                case VariableType::Number:
                    std::copy(other.numeric, other.numeric, numeric);
                    return *this;

                case VariableType::String:
                    string = other.string;
                    return *this;

                case VariableType::Boolean:
                    boolean = other.boolean;
                    return *this;

                case VariableType::Function:
                    function = other.function;
                    return *this;

                case VariableType::Array:
                    array = other.array;
                    return *this;

                default:
                    return *this;
            }
        }

        Variable &Variable::operator=(Variable &&other) noexcept {
            if (this == &other)
                return *this;

            type = other.type;
            switch (other.type) {
                case VariableType::Number:
                    std::copy(other.numeric, other.numeric, numeric);
                    return *this;

                case VariableType::String:
                    string = std::move(other.string);
                    return *this;

                case VariableType::Boolean:
                    boolean = other.boolean;
                    return *this;

                case VariableType::Function:
                    function = std::move(other.function);
                    return *this;

                case VariableType::Array:
                    array = std::move(other.array);
                    return *this;

                default:
                    return *this;
            };
        }
    }
}
