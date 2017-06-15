#ifndef UTIL_GENERIC_EXCEPTION_HPP
#define UTIL_GENERIC_EXCEPTION_HPP

#include <exception>
#include <string>
#include <type_traits>

namespace lang {
    namespace util {
        template<typename TypeTag, typename Base = std::exception>
        class GenericException : public Base {
            static_assert(std::is_base_of<std::exception, Base>::value);

            const std::string message;

        public:
            explicit GenericException(std::string message = "") : message(move(message)) {}

            virtual const char *what() const noexcept {
                return message.c_str();
            }
        };
    }
}

#endif // UTIL_GENERIC_EXCEPTION_HPP
