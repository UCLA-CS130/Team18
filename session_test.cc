#include "gtest/gtest.h"
#include "session.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class SessionInputTest : public ::testing::Test {
  protected:
  
    //Tests Parse with config in stringstream form
    bool CheckString(std::string str) {
      boost::asio::io_service io_service;
      tcp::socket socket(io_service);
      options_ = new config_options();
      Session s(&socket, options_);
      char buffer[256];
      std::size_t size = str.size();
      strncpy(buffer, str.c_str(), size);
      return s.check_input(size, buffer);
    }

    std::string GetOutput(std::string str, int status_code) {
      boost::asio::io_service io_service;
      tcp::socket socket(io_service);
      options_ = new config_options();
      Session s(&socket, options_);
      std::size_t length = s.prepare_response(status_code, str);
      return s.OutputToString();
    }

    void CleanUp() {
      delete options_;
    }

  config_options* options_;
};

TEST_F(SessionInputTest, CompleteMessage) {
  EXPECT_TRUE(CheckString("host\r\n\r\n"));
  CleanUp();
}

TEST_F(SessionInputTest, IncompleteMessage) {
  EXPECT_FALSE(CheckString("host"));
  CleanUp();
}

TEST_F(SessionInputTest, SampleOutput) {
  std::string message = "abc";
  std::string output = GetOutput(message, 200);
  EXPECT_TRUE(output.find(message) != std::string::npos);
  EXPECT_GT(output.size(), message.size());
  CleanUp();
}

TEST_F(SessionInputTest, BadRequest) {
  std::string message = "abc";
  std::string output = GetOutput(message, 400);

  EXPECT_TRUE(output.find(message) != std::string::npos);
  EXPECT_GT(output.size(), message.size());
  CleanUp();
}
