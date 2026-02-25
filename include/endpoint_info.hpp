#pragma once

#include <cstdint>
#include <string>
#include <netdb.h>
#include <sys/socket.h>

inline namespace endpoint_info {

class Address {
public:
	Address(sockaddr *raw_address) :
		raw_address_ { reinterpret_cast<sockaddr_storage *>(raw_address) },
		ip_domain_{ raw_address->sa_family } {}

	Address(std::string &address, std::string &port);

	addrinfo *c_addr();
	int32_t ip_domain();
	void print_address();

private:
	sockaddr_storage *raw_address_;
	int32_t ip_domain_;
};

class AddressInfo {
public:
	AddressInfo(addrinfo *raw_node) :
		raw_node_{ raw_node },
	 	address_{ Address(raw_node->ai_addr) } {}

	void string_repr();
	int domain();
	int socket_type();
	int protocol();
	const addrinfo *const c_addrinfo();
	int create_socket();

private:
	const addrinfo *const raw_node_;
	const Address address_;

};

} // namespace endpoint_info
