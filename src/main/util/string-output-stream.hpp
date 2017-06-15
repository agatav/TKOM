#ifndef UTIL_STRING_OUTPUT_STREAM_HPP
#define UTIL_STRING_OUTPUT_STREAM_HPP

#include <string>
#include "output-stream.hpp"

namespace lang {
    namespace util {
        class StringOutputStream : public OutputStream<char> {
            const std::string content;
            std::string::const_iterator position;

        public:
            explicit StringOutputStream(std::string content);

            bool take(char &out) override;
        };
    }
}

#endif // UTIL_STRING_OUTPUT_STREAM_HPP