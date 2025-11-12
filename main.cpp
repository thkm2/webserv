#include "webserv.hpp"

// AF_INET = le type d'adresse IP (IPv4 ou IPv6), dans ce cas c'est IPv4 sinon ça serait AF_INET6
// SOCK_STREAM = flux orienté connexion et donc en gros c'est comme TCP. Sinon on aurait mis SOCK_DGRAM (datagrammes) qui est comme UDP
// 0 = dis que ça choisis le protocol en fonction du type et vu que notre type est comme TCP, il prend TCP. Mais on aurait pu clarifier en mettant IPPROTO_TCP
static int createSocket() {
	int socket_d = socket(AF_INET, SOCK_STREAM, 0);
	if (!socket)
		throw std::runtime_error("socket creation");
	return socket_d;
}

int main(int ac, char **av) {
	if (ac != 2) {
		std::cerr << "use : ./webserv [configuration file]" << std::endl;
		return 1;
	}
	try {
		int socket_d = createSocket();
		// setsockopt
	} catch(const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	
	return 0;
}