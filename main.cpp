#include "config/lexer/Lexer.hpp"
#include <iostream>

static const char *tokenTypeToString(TokenType t) {
    switch (t) {
        case TOKEN_WORD:       return "WORD";
        case TOKEN_LBRACE:     return "LBRACE";
        case TOKEN_RBRACE:     return "RBRACE";
        case TOKEN_SEMICOLON:  return "SEMICOLON";
        case TOKEN_END:        return "END";
    }
    return "UNKNOWN";
}

int main(int ac, char **av) {
    if (ac != 2) {
        std::cerr << "Usage: " << av[0] << " <config_file>\n";
        return 1;
    }

    try {
        Lexer lexer(av[1]);

        while (!lexer.isEnd()) {
            Token t = lexer.next();
            std::cout << "Line " << t.line << ", Col " << t.column
                      << " : " << tokenTypeToString(t.type);

            if (t.type == TOKEN_WORD)
                std::cout << " => \"" << t.value << "\"";

            std::cout << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Lexer error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

