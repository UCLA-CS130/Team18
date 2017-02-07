#include "gtest/gtest.h"
#include "static_handler.h"
#include "request.h"
#include "response.h"
#include <string>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

class StaticHandlerTest : public ::testing::Test {
  protected:
    void PrepareFile(std::string directory, std::string filename, 
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
    void HandleRequest(std::string request_string,
    				   std::string echo, 
              		   std::map<std::string,std::string> stat) {
      request_ = new Request(request_string, echo, stat);
      response_ = new Response();
      StaticHandler* handler_ = new StaticHandler();
      handler_->handle_request(request_, response_);
      delete handler_;
    }

    void CleanUp() {
    	delete request_;
    	delete response_;
    }
    Request* request_;
    Response* response_;
};

TEST_F(StaticHandlerTest, SimpleStaticRequest) {
  std::string dir_str = "test_dir";
  PrepareFile(dir_str, "index.html", 
  "<html><body><h1>Header</h1></body></html>");
  std::string request =
		"GET /static/index.html HTTP/1.1\r\n\
		Host: localhost:1024\r\n\
		User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0\r\n\
		Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
		Accept-Language: en-US,en;q=0.5\r\n\
		Accept-Encoding: gzip, deflate\r\n\
		Connection: keep-alive\r\n\
		Upgrade-Insecure-Requests: 1\r\n\r\n";
  std::map<std::string, std::string> static_options;
  static_options.insert(std::make_pair("static", dir_str));
  HandleRequest(request,"echo",static_options);
  EXPECT_EQ("1.1", response_->http_version) << "Expected different HTTP Version";
  EXPECT_EQ(Response::ok, response_->status) << "Expected different status";
  EXPECT_EQ("text/html", response_->headers["Content-Type"]) << "Expected different Content-Type";
  EXPECT_EQ(std::to_string(response_->body.size()), response_->headers["Content-Length"]) << "Expected different Content-Length";

  CleanUp();
}

TEST_F(StaticHandlerTest, SimpleStaticImageRequest) {
  std::string dir_str = "test_dir";
  PrepareFile(dir_str, "test.jpg", 
  "JJJJJJJJJJ");
  std::string request =
		"GET /static/test.jpg HTTP/1.1\r\n\
		Host: localhost:1024\r\n\
		User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0\r\n\
		Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
		Accept-Language: en-US,en;q=0.5\r\n\
		Accept-Encoding: gzip, deflate\r\n\
		Connection: keep-alive\r\n\
		Upgrade-Insecure-Requests: 1\r\n\r\n";
  std::map<std::string, std::string> static_options;
  static_options.insert(std::make_pair("static", dir_str));
  HandleRequest(request,"echo",static_options);
  EXPECT_EQ("1.1", response_->http_version) << "Expected different HTTP Version";
  EXPECT_EQ(Response::ok, response_->status) << "Expected different status";
  EXPECT_EQ("image/jpeg", response_->headers["Content-Type"]) << "Expected different Content-Type";
  EXPECT_EQ(std::to_string(response_->body.size()), response_->headers["Content-Length"]) << "Expected different Content-Length";

  CleanUp();
}

TEST_F(StaticHandlerTest, FailedRequest) {
  std::string dir_str = "test_dir";
  PrepareFile(dir_str, "index.html", 
  "<html><body><h1>Header</h1></body></html>");
  std::string request =
		"GET /static/index.txt HTTP/1.1\r\n\
		Host: localhost:1024\r\n\
		User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0\r\n\
		Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
		Accept-Language: en-US,en;q=0.5\r\n\
		Accept-Encoding: gzip, deflate\r\n\
		Connection: keep-alive\r\n\
		Upgrade-Insecure-Requests: 1\r\n\r\n";
  std::map<std::string, std::string> static_options;
  static_options.insert(std::make_pair("static", dir_str));
  HandleRequest(request,"echo",static_options);
  EXPECT_EQ("1.1", response_->http_version) << "Expected different HTTP Version";
  EXPECT_EQ(Response::not_found, response_->status) << "Expected different status";
  EXPECT_EQ("text/plain", response_->headers["Content-Type"]) << "Expected different Content-Type";
  EXPECT_EQ(std::to_string(response_->body.size()), response_->headers["Content-Length"]) << "Expected different Content-Length";

  CleanUp();
}


