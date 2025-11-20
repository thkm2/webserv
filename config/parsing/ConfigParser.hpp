#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "../lexer/Lexer.hpp"
#include "../Config.hpp"
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <cctype>

class ConfigParser {
public:
    ConfigParser();

    // Parse un fichier de config qui doit contenir
    // exactement UN bloc "server { ... }"
    ServerConfig parse(const std::string &filename);

private:
    Lexer *_lexer;
    Token  _cur; // token courant

    // helpers de parsing
    void   advance();                         // _cur = _lexer->next()
    void   expect(TokenType type, const std::string &msg, const Token &t);
	std::string lineAndCol(const Token &t);

    // parse du bloc server
    void   parseServer(ServerConfig &server);
    void   parseServerDirective(ServerConfig &server);

    // helpers pour convertir des strings
    int    parseInt(const std::string &s, const Token &t);    // pour les ports / codes HTTP
    size_t parseSize(const std::string &s, const Token &t);   // pour client_max_body_size (version simple)
};

#endif