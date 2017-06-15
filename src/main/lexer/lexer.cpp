#include "lexer.hpp"

namespace lang {
    namespace lexer {
        Lexer *Lexer::instance = nullptr;

        Lexer::Lexer(util::OutputStream<char> &input) : input(input) {
            assert(instance == nullptr);
            instance = this;
        }

        Lexer::~Lexer() {
            instance = nullptr;
        }

        int Lexer::getInput(char *buffer, int maxRead) {
            int count = 0;
            while (count < maxRead && instance->input.take(buffer[count]))
                ++count;
            return count;
        }

        void Lexer::incrementLine() {
            ++instance->currentLine;
        }
    }
}