#include "ConfigParser.hpp"

ConfigParser::ConfigParser() : _lexer(NULL) {}

ServerConfig ConfigParser::parse(const std::string &filename) {
	Lexer lexer(filename);
	_lexer = &lexer;
	_cur = _lexer->next();
	if (_lexer->isEnd()) {
		throw std::runtime_error("Config file is empty");
	}
	if (!(_cur.type == TOKEN_WORD && _cur.value == "server")) {
		throw std::runtime_error("Config must start with 'server'");
	}
	ServerConfig server;
	parseServer(server);
	if (!_lexer->isEnd()) {
		throw std::runtime_error("Unexpected tokens after 'server' block");
	}
	return server;
}

// helper
void ConfigParser::advance() {
	_cur = _lexer->next();
}

// helper
void ConfigParser::expect(TokenType type, const std::string &msg) {
	if (_cur.type != type) {
		throw std::runtime_error(msg);
	}
}

int ConfigParser::parseInt(const std::string &s) {
    char *end = 0;
    long v = std::strtol(s.c_str(), &end, 10);
    if (*end || v < 0 || v > 65535) {
        throw std::runtime_error("Invalid integer: " + s);
    }
    return static_cast<int>(v);
}

// pas de K/M, juste en bytes
size_t ConfigParser::parseSize(const std::string &s) {
    char *end = 0;
    long v = std::strtol(s.c_str(), &end, 10);
    if (*end || v < 0) {
        throw std::runtime_error("Invalid size: " + s);
    }
    return static_cast<size_t>(v);
}

void ConfigParser::parseServer(ServerConfig &server) {
    advance(); // consomme server
    expect(TOKEN_LBRACE, "Expected '{' after 'server'");
    advance(); // consomme la brace
	while (!_lexer->isEnd() && _cur.type != TOKEN_RBRACE) {
        if (_cur.type != TOKEN_WORD) {
            throw std::runtime_error("Unexpected token in server block");
        }
        parseServerDirective(server);
    }
	expect(TOKEN_RBRACE, "Missing '}' at end of server block");
    advance(); // consomme rbrace pour etre à end
}

void ConfigParser::parseServerDirective(ServerConfig &server) {
    std::string name = _cur.value;
    advance();

    if (name == "listen") {
		expect(TOKEN_WORD, "Expected value after 'listen'");
        std::string value = _cur.value;
        advance();
        expect(TOKEN_SEMICOLON, "Missing ';' after listen");
        advance();

        ListenAddress la;
        std::string::size_type pos = value.find(':');
        if (pos == std::string::npos) {
            // pas de host
            la.host = "";
            la.port = parseInt(value);
        } else {
            la.host = value.substr(0, pos);
            la.port = parseInt(value.substr(pos + 1));
        }
        server.listens.push_back(la);
    } else if (name == "root") {
		expect(TOKEN_WORD, "Expected path after 'root'");
        server.root = _cur.value;
        advance();
        expect(TOKEN_SEMICOLON, "Missing ';' after root");
        advance();
    } else if (name == "index") {
		expect(TOKEN_WORD, "Expected at least one file after 'index'");
        while (_cur.type == TOKEN_WORD) {
            server.indexFiles.push_back(_cur.value);
            advance();
        }
        expect(TOKEN_SEMICOLON, "Missing ';' after index");
        advance();
    } else if (name == "client_max_body_size") {
		expect(TOKEN_WORD, "Expected size after 'client_max_body_size'");
        server.clientMaxBodySize = parseSize(_cur.value);
        advance();
        expect(TOKEN_SEMICOLON, "Missing ';' after client_max_body_size");
        advance();
    } else if (name == "error_page") {
        std::vector<int> codes;
        while (_cur.type == TOKEN_WORD) {
            const std::string &v = _cur.value;
            bool isNum = true;
            for (size_t i = 0; i < v.size(); ++i) {
                if (!std::isdigit(static_cast<unsigned char>(v[i]))) {
                    isNum = false;
                    break;
                }
            }
            if (!isNum)
                break;
            codes.push_back(parseInt(v));
            advance();
        }
        if (codes.empty()) {
            throw std::runtime_error("Expected error code(s) after 'error_page'");
        }

		expect(TOKEN_WORD, "Expected path after error codes");
        std::string path = _cur.value;
        advance();
        expect(TOKEN_SEMICOLON, "Missing ';' after error_page");
        advance();
        for (size_t i = 0; i < codes.size(); ++i) {
            server.errorPages[codes[i]] = path;
        }
    } else if (name == "location") {
		// ! WARN ! à faire plus tard
		advance();
		expect(TOKEN_LBRACE, "Expected '{' after 'location <name>'");
		advance();
		while (_cur.type != TOKEN_RBRACE) {
			advance();
		}
		expect(TOKEN_RBRACE, "Missing '}' at end of location block");
		advance();
	} else {
        throw std::runtime_error("Unknown server directive: '" + name + "'");
    }
}
