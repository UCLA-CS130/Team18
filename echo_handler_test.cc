#include "gtest/gtest.h"
#include "echo_handler.h"
#include "request.h"
#include "response.h"
#include <string>
#include <map>

class EchoHandlerTest : public ::testing::Test {
  protected:
    void HandleRequest(std::string request_string,
    				   std::string echo, 
              		   std::map<std::string,std::string> stat) {
      request_ = new Request(request_string, echo, stat);
      response_ = new Response();
      EchoHandler handler_;
      handler_.handle_request(request_, response_);
    }

    void CleanUp() {
    	delete request_;
    	delete response_;
    }
    Request* request_;
    Response* response_;
};

TEST_F(EchoHandlerTest, SimpleEchoRequest) {
  std::string request =
		"GET /echo HTTP/1.1\r\n\
		Host: localhost:1024\r\n\
		User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0\r\n\
		Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
		Accept-Language: en-US,en;q=0.5\r\n\
		Accept-Encoding: gzip, deflate\r\n\
		Connection: keep-alive\r\n\
		Upgrade-Insecure-Requests: 1\r\n\r\n";
  std::map<std::string, std::string> static_options;
  HandleRequest(request,"echo",static_options);
  EXPECT_EQ("1.1", response_->http_version) << "Expected different HTTP Version";
  EXPECT_EQ(Response::ok, response_->status) << "Expected different status";
  EXPECT_EQ("text/plain", response_->headers["Content-Type"]) << "Expected different Content-Type";
  EXPECT_EQ(std::to_string(request.size()), response_->headers["Content-Length"]) << "Expected different Content-Length";
  EXPECT_EQ(request, response_->body) << "Expected different body";

  CleanUp();
}

TEST_F(EchoHandlerTest, DifferentHTTPVersionRequest) {
  std::string request =
		"GET /echo HTTP/1.0\r\n\r\n";
  std::map<std::string, std::string> static_options;
  HandleRequest(request,"echo",static_options);
  EXPECT_EQ("1.0", response_->http_version) << "Expected different HTTP Version";
  EXPECT_EQ(Response::ok, response_->status) << "Expected different status";
  EXPECT_EQ("text/plain", response_->headers["Content-Type"]) << "Expected different Content-Type";
  EXPECT_EQ(std::to_string(request.size()), response_->headers["Content-Length"]) << "Expected different Content-Length";
  EXPECT_EQ(request, response_->body) << "Expected different body";

  CleanUp();
}

TEST_F(EchoHandlerTest, DifferentEchoOptionRequest) {
  std::string request =
		"GET /echo_different HTTP/1.0\r\n\r\n";
  std::map<std::string, std::string> static_options;
  HandleRequest(request,"echo_different",static_options);
  EXPECT_EQ("1.0", response_->http_version) << "Expected different HTTP Version";
  EXPECT_EQ(Response::ok, response_->status) << "Expected different status";
  EXPECT_EQ("text/plain", response_->headers["Content-Type"]) << "Expected different Content-Type";
  EXPECT_EQ(std::to_string(request.size()), response_->headers["Content-Length"]) << "Expected different Content-Length";
  EXPECT_EQ(request, response_->body) << "Expected different body";

  CleanUp();
}