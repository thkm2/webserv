#include "webserv.hpp"

// AF_INET = le type d'adresse IP (IPv4 ou IPv6), dans ce cas c'est IPv4 sinon ça serait AF_INET6
// SOCK_STREAM = flux orienté connexion et donc en gros c'est comme TCP. Sinon on aurait mis SOCK_DGRAM (datagrammes) qui est comme UDP
// 0 = dis que ça choisis le protocol en fonction du type et vu que notre type est comme TCP, il prend TCP. Mais on aurait pu clarifier en mettant IPPROTO_TCP
static int createSocket() {
	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0)
		throw std::runtime_error("socket() failed");
	return listen_fd;
}

void setSocketOptions(int listen_fd) {
	int one = 1;
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0)
		throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");
}

static void bindAndListen(int listen_fd, uint16_t port) {
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0
    addr.sin_port = htons(port);

    if (bind(listen_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
        throw std::runtime_error("bind() failed");

    if (listen(listen_fd, 128) < 0)
        throw std::runtime_error("listen() failed");
}

static void setClientOptions(int client_fd) {
    int one = 1;
    // Latence basse pour petits envois (headers HTTP)
    if (setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one)) < 0)
		throw std::runtime_error("setsockopt(TCP_NODELAY) failed");
}

// lit tout un fichier en mémoire (texte/binaire) — simple et compatible C++98
static std::string loadFile(const std::string &path) {
    std::ifstream f(path.c_str(), std::ios::in | std::ios::binary);
    if (!f) throw std::runtime_error("cannot open file: " + path);
    std::ostringstream oss;
    oss << f.rdbuf(); // safe en C++98
    return oss.str();
}

// guess minimal du Content-Type (ici on vise juste .html)
static const char* guessContentType(const std::string& path) {
    // pour un MVP on renvoie text/html, tu peux étendre plus tard (.css, .js, .png, ...)
	(void)path;
    return "text/html; charset=utf-8";
}

int main(int ac, char **av) {
	if (ac != 2) {
		std::cerr << "use : ./webserv [configuration file]" << std::endl;
		return 1;
	}
	(void)av;
	try {
		int listen_fd = createSocket();
		setSocketOptions(listen_fd);
		bindAndListen(listen_fd, 8080);
		std::cout << "Listening on http://127.0.0.1:8080" << std::endl;

		// MVP: on accepte UNE connexion, on répond, on ferme
        sockaddr_in peer; socklen_t plen = sizeof(peer);
        int client_fd = ::accept(listen_fd, reinterpret_cast<sockaddr*>(&peer), &plen);
        if (client_fd < 0) throw std::runtime_error("accept() failed");

		setClientOptions(client_fd);

        // 1) lire le fichier
        std::string body;
        std::string htmlPath = "index.html";
        try {
            body = loadFile(htmlPath);
        } catch (const std::exception& ex) {
            // simple 404 si le fichier n'existe pas
            const char* notFoundBody =
                "<!doctype html><html><body><h1>404 Not Found</h1></body></html>";
            char lenbuf[32];
            std::snprintf(lenbuf, sizeof(lenbuf), "%lu",
                          (unsigned long)std::strlen(notFoundBody));
            std::string resp =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/html; charset=utf-8\r\n"
                "Content-Length: ";
            resp += lenbuf;
            resp += "\r\nConnection: close\r\n\r\n";
            resp += notFoundBody;
            ::send(client_fd, resp.data(), resp.size(), 0);
            ::close(client_fd);
            ::close(listen_fd);
            return 0;
        }

        // 2) construire la réponse HTTP
        const char* ctype = guessContentType(htmlPath);
        char lenbuf[32];
        std::snprintf(lenbuf, sizeof(lenbuf), "%lu", (unsigned long)body.size());

        std::string resp;
        resp.reserve(128 + body.size());
        resp  = "HTTP/1.1 200 OK\r\n";
        resp += "Content-Type: ";
        resp += ctype;
        resp += "\r\nContent-Length: ";
        resp += lenbuf;
        resp += "\r\nConnection: close\r\n\r\n";
        resp += body;

        // 3) envoyer
        (void)::send(client_fd, resp.data(), resp.size(), 0);

        close(client_fd);
        close(listen_fd);
	} catch(const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	
	return 0;
}
