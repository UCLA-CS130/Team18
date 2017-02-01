#include "gtest/gtest.h"
#include "server.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

TEST(ServerTest, GetPort) {
  Server s(1024);
  s.start();
  EXPECT_EQ(1024, s.get_port()) << "Expected port to be 1024";
}

TEST(ServerTest, RunAndAccept) {
  Server s(1024);
  s.run();
  EXPECT_EQ(1024, s.get_port()) << "Expected port to be 1024";
}
