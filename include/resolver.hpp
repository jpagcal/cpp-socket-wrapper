#pragma once

#include "endpoint_info.hpp"
#include <cstdint>
#include <netdb.h>
#include <string>
#include <vector>

namespace conn_resolver {

struct ResolverHints {
	int32_t endpoint_type;
	int32_t ip_domain;
	int32_t protocol;
	int32_t flags;
};

addrinfo craft_resolver_hints(ResolverHints &hints);

class Resolver {
	using QueryResults = std::vector<AddressInfo>;
	using RawResults = addrinfo *;
public:
	Resolver(std::string hostname, std::string service);
	Resolver();
	~Resolver();
	QueryResults results();
	std::string print_results();

private:
	QueryResults query_results_;
};
} // namespace resolver
