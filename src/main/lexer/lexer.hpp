#ifndef LEXER_LEXER_HPP
#define LEXER_LEXER_HPP

#include "../util/output-stream.hpp"
#include "lexeme.hpp"

namespace lang {
    namespace lexer {
        class Lexer : public util::OutputStream<Lexeme> {
            static Lexer *instance;

            util::OutputStream<char> &input;
            int currentLine = 1;

        public:
            explicit Lexer(util::OutputStream<char> &input);

            virtual ~Lexer();

            bool take(Lexeme &out) override;

            /**
             * Reads specified number of characters from input stream of current Lexer instance.
             *
             * @param buffer Output buffer.
             * @param maxRead Buffer size.
             * @return Number of characters read.
             * @remarks For internal use in flex file.
             */
            static int getInput(char *buffer, int maxRead);

            /**
             * @remarks For internal use in flex file.
             */
            static void incrementLine();
        };
    }
}

#endif // LEXER_LEXER_HPP
