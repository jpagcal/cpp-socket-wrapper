#pragma once

#include "endpoint_info.hpp"
#include "networking.hpp"
#include "tcp.hh"
#include <cstdint>
#include <netdb.h>
#include <string>
#include <vector>

namespace conn_resolver {

struct ResolverHints {
	int32_t endpoint_type = networking::socket_type::raw;
	int32_t ip_domain = networking::domain::unspecified_domain;
	int32_t protocol;
	int32_t flags;
};

addrinfo craft_resolver_hints(ResolverHints &hints);

class Resolver {
	using QueryResults = std::vector<AddressInfo>;
	using RawResults = addrinfo *;
public:
	Resolver(std::string hostname, std::string service);
	QueryResults results() const;
	QueryResults udp_nodes() const;
	QueryResults tcp_nodes() const;
	tcp::Connection try_connect_tcp();
	void try_connect_udp(); // TODO: create UDPConnection class
	void print_results() const;

private:
	QueryResults query_results_;
};
} // namespace resolver
