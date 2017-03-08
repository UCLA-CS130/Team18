#include "gtest/gtest.h"
#include "proxy_handler.h"
#include "config_parser.h"
#include "request_handler.h"
#include "request.h"
#include "response.h"
#include <string>
#include <sstream>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <memory>
#include <iostream>

class ProxyHandlerTest : public ::testing::Test {
protected:
  
  //  void PrepareCurl
  RequestHandler::Status HandleRequest(std::string request_string) 
    {
      request_ = Request::Parse(request_string);
      return handler_->HandleRequest(*request_, &response_);
    }
  RequestHandler::Status Initialize(std::string proxy_path, std::string host, std::string port)
    {
      std::string config_string = "host " + host + ";\nport " + port + " ;\n";
      std::stringstream config_stream(config_string);
      NginxConfigParser parser;
      NginxConfig out_config;
      parser.Parse(&config_stream, &out_config);
      std::cout << "Init" << std::endl;
      std::cout << out_config.ToString() << std::endl;
      handler_ = RequestHandler::CreateByName("ProxyHandler");
      std::cout << "Handler: " << handler_ << std::endl;
      return handler_->Init(proxy_path, out_config);
    }
  std::unique_ptr<Request> request_;
  Response response_;
  RequestHandler* handler_;
};

TEST_F(ProxyHandlerTest, SimpleUCLACurl) {
}
TEST_F(ProxyHandlerTest, RedirectTest) {

}
TEST_F(ProxyHandlerTest,WrongPort) {
  std::string host = "hello.hello";
  std::cout << "" << std::endl;
  EXPECT_EQ(Initialize("/",host,"80"),RequestHandler::Status::NOT_FOUND);
   
}
TEST_F(ProxyHandlerTest,WrongHost) {
}
