#include "../include/networking.hh"
#include "../include/tcp.hh"
#include <stdexcept>
#include <sys/_types/_ssize_t.h>
#include <sys/types.h>
#include <netdb.h>
#include <system_error>
#include <unistd.h>
#include <cstddef>
#include <vector>

namespace tcp {
Connection::Connection(int socketFD) : socket_(socketFD) {}

Connection::~Connection() {
	close(this->socket_);
}

Connection Connection::resolve(const std::string host, const std::string service, const int domain) {
	addrinfo hints, *res;

	// zero out hints
	memset(&hints, 0, sizeof(addrinfo));

	hints.ai_family = domain;
	hints.ai_socktype = networking::socket_type::tcp;

	if (!(getaddrinfo(host.c_str(), service.c_str(), &hints, &res))) {
		freeaddrinfo(res);
		throw std::system_error(errno, std::generic_category());
	}

	int socketFD{ socket(domain, networking::socket_type::tcp, 0) };

	// connect the socket to the host
	for (addrinfo *cur{ res }; res != nullptr; cur = cur->ai_next) {
		if (!(connect(socketFD, cur->ai_addr, cur->ai_addrlen))) {
			break;
		}

		// for case n - 1, if it hasnt connected at this point we may throw an exception
		if (cur->ai_next == nullptr) {
			freeaddrinfo(res);
			throw std::runtime_error("Failed to connect to a host");
		}
	}

	freeaddrinfo(res);

	return Connection(socketFD);
}

void Connection::sendAll(std::string_view msg) {
	ssize_t total{};
	ssize_t sent{};
	ssize_t bytesLeft{ static_cast<ssize_t>(msg.size()) };

	while (total < msg.size()) {
		ssize_t sentThisIteration{ send(this->socket_, msg.data() + sent, bytesLeft, 0) };

		if (sentThisIteration == -1) {

		}

		total += sentThisIteration;
		bytesLeft -= sentThisIteration;
	}

}

void Connection::recvAll() {

}

} // namespace tcp
