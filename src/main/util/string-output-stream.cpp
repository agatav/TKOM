#include "string-output-stream.hpp"
#include <iostream>

namespace lang {
    namespace util {
        StringOutputStream::StringOutputStream(std::string content) : content(std::move(content)) {
            position = this->content.begin();
        }

        bool StringOutputStream::take(char &out) {
            if (position != content.end()) {
                out = *(position++);
                return true;
            }
            return false;
        }
    }
}