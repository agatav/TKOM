#ifndef INTERPRETER_VARIABLE_HPP
#define INTERPRETER_VARIABLE_HPP

#include <functional>
#include <vector>

namespace lang {
    namespace interpreter {
        enum class VariableType {
            Undefined = 0,

            Number,
            String,
            Boolean,
            Function,
            Array
        };

        class Variable {
            VariableType type = VariableType::Undefined;

            double numeric[4];
            std::function<Variable(std::vector<Variable>)> function;
            std::string string;
            std::vector<Variable> array;
            bool boolean;

        public:
            Variable() = default;

            Variable(double numeric[], std::size_t len);

            Variable(std::function<Variable(std::vector<Variable>)> function);

            Variable(std::string string);

            Variable(std::vector<Variable> array);

            Variable(bool boolean);

            Variable(const Variable &other) noexcept;

            Variable(Variable &&other) noexcept;

            operator bool() const;

            VariableType getType() const;

            Variable &getArrayItem(std::size_t index);

            std::size_t getNumberAsSize() const;

            Variable operator()(std::vector<Variable> arguments) const;

            Variable operator+(const Variable &other) const;

            Variable operator-(const Variable &other) const;

            Variable operator*(const Variable &other) const;

            Variable operator/(const Variable &other) const;

            Variable operator-() const;

            Variable operator!() const;

            Variable operator==(const Variable &other) const;

            Variable operator!=(const Variable &other) const;

            Variable operator<(const Variable &other) const;

            Variable operator<=(const Variable &other) const;

            Variable operator>(const Variable &other) const;

            Variable operator>=(const Variable &other) const;

            std::string toString() const;

            double getNumeric() const;

            std::size_t getLength() const;

            std::vector<Variable> &getArray();

            Variable &operator=(const Variable &other) noexcept;

            Variable &operator=(Variable &&other) noexcept;

        private:
            void checkSameType(const Variable &other) const;

            template<typename OP>
            Variable compareRelation(const Variable &other, OP op) const;
        };
    }
}

#endif // INTERPRETER_VARIABLE_HPP