#include "config/parsing/ConfigParser.hpp"
#include <iostream>

int main(int ac, char **av) {
    if (ac != 2) {
        std::cerr << "Usage: " << av[0] << " <config_file>\n";
        return 1;
    }

    try {
        ConfigParser parser;

		const ServerConfig s = parser.parse(av[1]);
		std::cout << "  listens:\n";
		for (size_t j = 0; j < s.listens.size(); ++j) {
			std::cout << "    " << s.listens[j].host
						<< ":" << s.listens[j].port << "\n";
		}
		std::cout << "  root: " << s.root << "\n";
		std::cout << "  index:";
		for (size_t j = 0; j < s.indexFiles.size(); ++j) {
			std::cout << " " << s.indexFiles[j];
		}
		std::cout << "\n";
		std::cout << "  client_max_body_size: "
					<< s.clientMaxBodySize << "\n";
		std::cout << "  error_pages:\n";
		for (std::map<int, std::string>::const_iterator it = s.errorPages.begin();
				it != s.errorPages.end(); ++it) {
			std::cout << "    " << it->first << " -> " << it->second << "\n";
		}

    } catch (const std::exception &e) {
        std::cerr << "Config error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
