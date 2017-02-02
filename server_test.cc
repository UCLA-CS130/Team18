#include "gtest/gtest.h"
#include "server.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

TEST(ServerTest, GetPort) {
  Server s(1024);
  s.start();
  EXPECT_EQ(1024, s.get_port()) << "Expected port to be 1024";
}

TEST(ServerTest, OverflowPort) {
  Server s(65535);
  s.start();
  EXPECT_EQ(8080, s.get_port()) << "Expected port to be -1";
}

TEST(ServerTest, NegativePort) {
  Server s(-1);
  s.start();
  EXPECT_EQ(8080, s.get_port()) << "Expected port to be 1";
}

TEST(ServerTest, RunAndAccept) {
  Server s(1024);
  s.run();
}
