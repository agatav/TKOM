#include "stdin-output-stream.hpp"
#include <iostream>

namespace lang {
    namespace util {
        bool StdinOutputStream::take(char &out) {
            return (bool) std::cin.get(out);
        }
    }
}