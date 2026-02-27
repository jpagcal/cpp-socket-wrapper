#include <gtest/gtest.h>
#include "../include/endpoint_info.hpp"

class AddressTest : public testing::Test {
	protected:
		AddressTest() {
			std::string address{ "127.0.0.1" };
			std::string port{ "8080" };

			addr_ = Address(address, port);
		}

	Address addr_ = Address();
};

TEST_F(AddressTest, test) {
	switch (addr_.ip_domain()) {
		case networking::domain::ipv4:
			std::cout << "ipv4!" << std::endl;
			break;
		case networking::domain::ipv6:
			std::cout << "ipv6" << std::endl;
			break;
	}
}
