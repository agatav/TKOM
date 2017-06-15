#ifndef UTIL_STDIN_OUTPUT_STREAM_HPP
#define UTIL_STDIN_OUTPUT_STREAM_HPP

#include "output-stream.hpp"

namespace lang {
    namespace util {
        struct StdinOutputStream : OutputStream<char> {
            bool take(char &out) override;
        };
    }
}

#endif // UTIL_STDIN_OUTPUT_STREAM_HPP