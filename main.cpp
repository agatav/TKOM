#include <iostream>
#include <fstream>
#include "src/main/logging/logger.hpp"
#include "src/main/util/stdin-output-stream.hpp"
#include "src/main/lexer/lexer.hpp"
#include "src/main/parser/parser.hpp"
#include "src/main/interpreter/interpreter.hpp"

int main() {
    std::ifstream in("/home/agata/CLionProjects/TKOM/src/test.lang");
    std::cin.rdbuf(in.rdbuf());

    try {
        lang::logging::Logger logger;
        lang::util::StdinOutputStream inputFile;
        lang::lexer::Lexer lexer(inputFile);
        lang::parser::Parser parser(logger, lexer);
        auto script = parser.getTree();
        lang::interpreter::Interpreter interpreter(*script);
        interpreter.execute("main");
    } catch (std::exception e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}