#include "gtest/gtest.h"
#include "status_handler.h"
#include "request_handler_stats.h"
#include "request.h"
#include "response.h"
#include <string>
#include <map>

class StatusHandlerTest : public ::testing::Test {
  protected:
    void HandleRequest(std::string request_string) {
      request_ = Request::Parse(request_string);
      response_ = new Response();
      StatusHandler handler_;
      handler_.HandleRequest(*request_, response_);
    }

    void CleanUp() {
    	request_.release();
    	delete response_;
    }
    std::unique_ptr<Request> request_;
    Response* response_;
};

TEST_F(StatusHandlerTest, StatusRequest) {
  std::string request =
		"GET /status HTTP/1.1\r\n\
		Host: localhost:1024\r\n\
		User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0\r\n\
		Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
		Accept-Language: en-US,en;q=0.5\r\n\
		Accept-Encoding: gzip, deflate\r\n\
		Connection: keep-alive\r\n\
		Upgrade-Insecure-Requests: 1\r\n\r\n";
  HandleRequest(request);
  EXPECT_EQ("HTTP/1.1 200 OK\r\nContent-Length: 227\r\n\r\n<html><p>Default Handler: Default Handler not specified</p><table><tr><th>Request Handler</th><th>URL Prefix</th></tr></table><p>Number of Requests: 0</p><table><tr><th>URL Request</th><th>Response Code</th></tr></table></html>",
            response_->ToString()) << "Expected different ToString";

  CleanUp();
}

