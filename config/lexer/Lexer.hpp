#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>

#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cctype>   // std::isspace

enum TokenType {
	TOKEN_WORD,
	TOKEN_LBRACE,
	TOKEN_RBRACE,
	TOKEN_SEMICOLON,
	TOKEN_END
};

struct Token {
	TokenType	type;
	std::string	value;
	size_t		line;
	size_t		column;
};


class Lexer {
	public:
		Lexer(const std::string &filename);

		const Token &peek() const; // regarder le token courant
		Token next(); // avancer et retourner le token courant
		bool isEnd() const;

	private:
		std::vector<Token>	_tokens;
		size_t				_index;

		void tokenize(const std::string &filename);
};

#endif