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

TEST_F(AddressTest, PrintsAddressWithPort) {
	testing::internal::CaptureStdout();
	addr_.print_address();
	std::string output{ testing::internal::GetCapturedStdout() };
	std::string addressWithPort{ "127.0.0.1:8080\n" };
	EXPECT_STREQ(addressWithPort.data(), output.data());
}

TEST_F(AddressTest, emits_a_proper_sockaddr_struct_with_the_correct_fields) {
	auto c_sockaddr = addr_.c_addr();

	sockaddr_in *const ipv4_sockaddr{ reinterpret_cast<sockaddr_in *const>(c_sockaddr) };
	EXPECT_EQ(networking::domain::ipv4, ipv4_sockaddr->sin_family);
	EXPECT_EQ(htons(atoi("8080")), ipv4_sockaddr->sin_port);
}
