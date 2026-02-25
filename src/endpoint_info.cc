#include "../include/endpoint_info.hpp"
#include "../include/networking.hpp"
#include <cstdint>
#include <netinet/in.h>
#include <sys/_endian.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>

inline namespace endpoint_info {
	Address::Address(std::string &address, std::string &port) {
		int32_t ip_domain;
		uint16_t port_num = htons(std::stoi(port));
		auto colon_delim = address.find(":");
		sockaddr_storage *raw_address;

		memset(raw_address, 0, sizeof(sockaddr_storage));

		sockaddr_in *ipv4_raw_address{ reinterpret_cast<sockaddr_in *>(raw_address) };
		sockaddr_in6 *ipv6_raw_address { reinterpret_cast<sockaddr_in6 *>(raw_address) };

		// depending on whether or not we can find a colon we can identify the address domain
		switch (colon_delim) {
			// then we set the fields on on the storage
			case std::string::npos: // ipv4
				ip_domain = networking::domain::ipv4;

				ipv4_raw_address->sin_family = networking::domain::ipv4;
				ipv4_raw_address->sin_port = port_num;
				inet_pton(
					networking::domain::ipv4,
					address.c_str(),
					reinterpret_cast<void *>(&(ipv4_raw_address->sin_addr))
				);
				break;

				this->raw_address_ = raw_address;
				this->ip_domain_ = ip_domain;
			default: // ipv6
				ip_domain = networking::domain::ipv6;

				ipv6_raw_address->sin6_family = networking::domain::ipv6;
				ipv6_raw_address->sin6_port = port_num;
				inet_pton(
					networking::domain::ipv6,
					address.c_str(),
					reinterpret_cast<void *>(&(ipv6_raw_address->sin6_addr))
				);
				break;
		}

		this->raw_address_ = raw_address;
		this->ip_domain_ = ip_domain;
	}

	int32_t Address::ip_domain() {
		return this->ip_domain_;
	}

	 addrinfo *Address::c_addr() {
		return reinterpret_cast<addrinfo *>(this->raw_address_);
	}

	void Address::print_address() {
		auto get_port_str{
			[](uint16_t port_num) -> std::string {
				return std::to_string(
					ntohs(port_num)
				);
			}
		};

		char addr_buf[64];
		std::string port;

		switch (this->ip_domain_) {
			case networking::domain::ipv4: {
				auto ipv4_raw_address{ reinterpret_cast<sockaddr_in *>(this->raw_address_) };
				inet_ntop(
					networking::domain::ipv4,
					&ipv4_raw_address->sin_addr,
					addr_buf,
					ipv4_raw_address->sin_len
				);

				port = get_port_str(ipv4_raw_address->sin_port);
				break;
			}

			default: {
				auto ipv6_raw_address{ reinterpret_cast<sockaddr_in6 *>(this->raw_address_) };
				inet_ntop(
					networking::domain::ipv6,
					&ipv6_raw_address->sin6_addr,
					addr_buf,
					ipv6_raw_address->sin6_len
				);

				port = get_port_str(ipv6_raw_address->sin6_port);
				break;
			}
		}

		std::cout << addr_buf << ":" << port << '\n';
	}

	void AddressInfo::string_repr() {
		//TODO: finish implementation
	}

	int AddressInfo::domain() {
		return this->raw_node_->ai_family;
	}

	int AddressInfo::socket_type() {
		return this->raw_node_->ai_socktype;
	}

	int AddressInfo::protocol() {
		return this->raw_node_->ai_protocol;
	}

	const addrinfo *const AddressInfo::c_addrinfo() {
		return this->raw_node_;
	}

	int AddressInfo::create_socket() {
		int socket_fd = socket(this->domain(), this->socket_type(), 0);

		//TODO: error handling here
	 	return socket_fd;
	}




}
