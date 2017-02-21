#include "gtest/gtest.h"
#include "request.h"
#include <string>

class RequestTest : public ::testing::Test {
  protected:
    std::string CreateRequestString(std::string method, 
                                    std::string uri, 
                                    std::string version) 
    {
      std::string request_str = method + " " + uri + " " + "HTTP/" + version;
      request_str +=  "\r\nHost: localhost\r\n\r\n";
      return request_str;
    }
    
    void SetFromString(std::string req_str) {
      request = Request::Parse(req_str);     
    }
    
    void SetNone(std::string method, std::string uri, std::string version) {
      std::string req_str = CreateRequestString(method, uri, version);
      request = Request::Parse(req_str);
    }

    void SetEcho(std::string echo_str) {
      std::string req_str = CreateRequestString("GET", "/" + echo_str, "1.1");
      request = Request::Parse(req_str);
    }

    void SetStatic(std::string stat_str, std::string file_str) {
      std::string req_str = CreateRequestString("GET", "/" + stat_str, "1.1");
      request = Request::Parse(req_str);
    }
    
    void CleanUp() { request.release(); }

    std::unique_ptr<Request> request;	
};

TEST_F(RequestTest, GetURI) {
  SetNone("GET", "/example", "1.1");
  EXPECT_EQ(request->uri(), "/example");
  CleanUp();
}

TEST_F(RequestTest, GetMethod) {
  SetNone("GET", "/example", "1.1");
  EXPECT_EQ(request->method(), "GET");
  CleanUp();
}


TEST_F(RequestTest, GetVersion) {
  SetNone("GET", "/example", "1.1");
  EXPECT_EQ(request->version(), "1.1");
  CleanUp();
}

TEST_F(RequestTest, GetHeader) {
  SetNone("GET", "/example", "1.1");
  std::pair<std::string, std::string> header("Host", "localhost");
  EXPECT_EQ(header, request->headers().front());
  CleanUp();
}

TEST_F(RequestTest, GetOriginal) {
  std::string req_str = CreateRequestString("GET", "/example", "1.1");
  SetFromString(req_str);
  EXPECT_EQ(request->raw_request(), req_str);
  CleanUp();
}

