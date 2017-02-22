#include "gtest/gtest.h"
#include "config_parser.h"
#include "server.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class ServerTest : public ::testing::Test {
  protected:
    int CreateServerGetPort(int port) {
      options_ = new config_options();
      options_->port = port;
      Server server_(config_, options_);
      return server_.get_port();
    }
    void CleanUp() {
      delete options_;
    }

    NginxConfig* config_;
    config_options* options_;
};

TEST_F(ServerTest, GetPort) {
  EXPECT_EQ(1024, CreateServerGetPort(1024)) << "Expected port to be 1024";
  CleanUp();
}

TEST_F(ServerTest, OverflowPort) {
  EXPECT_EQ(8080, CreateServerGetPort(65535)) << "Expected port to be -1";
  CleanUp();
}

TEST_F(ServerTest, NegativePort) {
  EXPECT_EQ(8080, CreateServerGetPort(-1)) << "Expected port to be 1";
  CleanUp();
}

