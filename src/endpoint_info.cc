#include "../include/endpoint_info.hpp"
#include "../include/networking.hpp"
#include <sys/_endian.h>
#include <arpa/inet.h>

inline namespace endpoint_info {
	Address::Address(std::string &address, std::string &port) {
		int32_t ip_domain;
		uint16_t port_num = static_cast<uint16_t>(std::stoi(port));
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
}
