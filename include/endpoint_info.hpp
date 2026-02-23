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

	int32_t ip_domain();
	Address get_addr();
	virtual void print_address();

private:
	sockaddr_storage *raw_address_;
	int32_t ip_domain_;
};

class IPV4Address : public Address{
public:
	void print_address() override;

};

class IPV6Address : public Address {
public:
	void print_address() override;
};

class AddressInfo {
public:
	AddressInfo(addrinfo *raw_node) :
		raw_node_{ raw_node },
		canonical_name_{ raw_node->ai_canonname },
	 	address_{ Address(raw_node->ai_addr) } {}

private:
	addrinfo *raw_node_;
	std::string canonical_name_;
	Address address_;
};

} // namespace endpoint_info
