#include "gtest/gtest.h"
#include "static_handler.h"
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

class StaticHandlerTest : public ::testing::Test {
  protected:
    void PrepareFile(std::string directory, 
                     std::string filename, 
                     std::string filebody) 
      {
      struct stat st = {0};
      std::string dir_str = "./" + directory;
      if (stat(dir_str.c_str(), &st) == -1) {
        mkdir(dir_str.c_str(), 0700);
      }
      std::fstream testfile;
      testfile.open(directory + "/" + filename,std::fstream::out);
      testfile << filebody;
      testfile.close();
    }
    void HandleRequest(std::string request_string) 
    {
      request_ = Request::Parse(request_string);
      response_ = new Response();
      RequestHandler::Status stat = handler_->HandleRequest(*request_, response_);
    }
    void Initialize(std::string static_path, std::string root)
    {
      std::string config_string = "root " + root + ";";
      std::stringstream config_stream(config_string);
      NginxConfigParser parser;
      NginxConfig out_config;
      parser.Parse(&config_stream, &out_config);
      std::cout << "Init" << std::endl;
      std::cout << out_config.ToString() << std::endl;
      handler_ = RequestHandler::CreateByName("StaticHandler");
      RequestHandler::Status stat = handler_->Init(static_path, out_config);
    }
    void CleanUp() {
    	delete response_;
        delete handler_;
    }
    std::unique_ptr<Request> request_;
    Response* response_;
    RequestHandler* handler_;
};

TEST_F(StaticHandlerTest, SimpleStaticRequest) {
  std::string dir_str = "test_dir";
  std::string file_body = "<html><body><h1>Header</h1></body></html>";
  PrepareFile(dir_str, "index.html", file_body);
  std::string request =
		"GET /static/index.html HTTP/1.1\r\n\
		Host: localhost:1024\r\n\
		User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0\r\n\
		Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
		Accept-Language: en-US,en;q=0.5\r\n\
		Accept-Encoding: gzip, deflate\r\n\
		Connection: keep-alive\r\n\
		Upgrade-Insecure-Requests: 1\r\n\r\n";
  std::cout << "Initializing" << std::endl;
  Initialize("/static", dir_str);
  std::cout << "Handling" << std::endl;
  HandleRequest(request);
  std::string response_str = response_->ToString();
  std::string body = response_str.substr(response_str.find("\r\n\r\n") + 4);
  std::string expected_status = "HTTP/1.1 200 OK";
  std::cout << "Testing Values" << std::endl;
  EXPECT_EQ(0, expected_status.compare(response_str.substr(0,expected_status.size())));
  EXPECT_EQ(0, file_body.compare(body));
  CleanUp();
}

TEST_F(StaticHandlerTest, SimpleStaticImageRequest) {
  std::string dir_str = "test_dir";
  std::string file_body = "JJJJJJJJJJ";
  PrepareFile(dir_str, "test.jpg", file_body);
  std::string request =
		"GET /static/test.jpg HTTP/1.1\r\n\
		Host: localhost:1024\r\n\
		User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0\r\n\
		Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
		Accept-Language: en-US,en;q=0.5\r\n\
		Accept-Encoding: gzip, deflate\r\n\
		Connection: keep-alive\r\n\
		Upgrade-Insecure-Requests: 1\r\n\r\n";
  Initialize("/static", dir_str);
  HandleRequest(request);
  std::string response_str = response_->ToString();
  std::string body = response_str.substr(response_str.find("\r\n\r\n") + 4);
  std::string expected_status = "HTTP/1.1 200 OK";
  EXPECT_EQ(0, expected_status.compare(response_str.substr(0,expected_status.size())));
  EXPECT_EQ(0, file_body.compare(body));
 
  CleanUp();
}

TEST_F(StaticHandlerTest, FailedRequest) {
  std::string dir_str = "test_dir";
  std::string file_body = "<html><body><h1>Header</h1></body></html>";
  PrepareFile(dir_str, "index.html", file_body);
  std::string request =
		"GET /static/index.txt HTTP/1.1\r\n\
		Host: localhost:1024\r\n\
		User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0\r\n\
		Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
		Accept-Language: en-US,en;q=0.5\r\n\
		Accept-Encoding: gzip, deflate\r\n\
		Connection: keep-alive\r\n\
		Upgrade-Insecure-Requests: 1\r\n\r\n";
  Initialize("/static", dir_str);
  HandleRequest(request);
  std::string response_str = response_->ToString();
  std::string body = response_str.substr(response_str.find("\r\n\r\n") + 4);
  std::string expected_status = "HTTP/1.1 404 Not Found";
  EXPECT_EQ(0, expected_status.compare(response_str.substr(0,expected_status.size())));
    
  CleanUp();
}


