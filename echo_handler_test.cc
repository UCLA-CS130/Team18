#include "gtest/gtest.h"
#include "echo_handler.h"
#include "request.h"
#include "response.h"
#include <string>
#include <map>

class EchoHandlerTest : public ::testing::Test {
  protected:
    void HandleRequest(std::string request_string) {
      request_ = Request::Parse(request_string);
      response_ = new Response();
      EchoHandler handler_;
      handler_.HandleRequest(*request_, response_);
    }

    void CleanUp() {
    	request_.release();
    	delete response_;
    }
    std::unique_ptr<Request> request_;
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
  HandleRequest(request);
  EXPECT_EQ("HTTP/1.1 200 OK\r\nContent-Length: 340\r\nContent-Type: text/plain\r\n\r\nGET /echo HTTP/1.1\r\n\t\tHost: localhost:1024\r\n\t\tUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0\r\n\t\tAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\t\tAccept-Language: en-US,en;q=0.5\r\n\t\tAccept-Encoding: gzip, deflate\r\n\t\tConnection: keep-alive\r\n\t\tUpgrade-Insecure-Requests: 1\r\n\r\n",
            response_->ToString()) << "Expected different ToString";

  CleanUp();
}
