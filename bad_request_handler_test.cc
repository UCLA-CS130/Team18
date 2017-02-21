#include "gtest/gtest.h"
#include "bad_request_handler.h"
#include "request.h"
#include "response.h"
#include <string>
#include <map>

class BadRequestHandlerTest : public ::testing::Test {
  protected:
    void HandleRequest(std::string request_string) {
      request_ = Request::Parse(request_string);
      response_ = new Response();
      BadRequestHandler handler_;
      handler_.HandleRequest(*request_, response_);
    }

    void CleanUp() {
    	request_.release();
    	delete response_;
    }
    std::unique_ptr<Request> request_;
    Response* response_;
};

TEST_F(BadRequestHandlerTest, BadEchoRequest) {
  std::string request =
		"GET /echo_different HTTP/1.1\r\n\
		Host: localhost:1024\r\n\
		User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0\r\n\
		Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
		Accept-Language: en-US,en;q=0.5\r\n\
		Accept-Encoding: gzip, deflate\r\n\
		Connection: keep-alive\r\n\
		Upgrade-Insecure-Requests: 1\r\n\r\n";
  HandleRequest(request);
  EXPECT_EQ("HTTP/1.1 400 Bad Request\r\nContent-Length: 47\r\n\r\n<html><body><h1>Bad Request!</h1></body></html>",
            response_->ToString()) << "Expected different ToString";

  CleanUp();
}

