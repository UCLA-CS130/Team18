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
      std::map<std::string,std::string> stat_map;
      stat_map.insert(std::make_pair("static", "src"));
      request = new Request(req_str, "echo", stat_map);       
    }
    
    void SetNone(std::string method, std::string uri, std::string version) {
      std::string req_str = CreateRequestString(method, uri, version);
      std::map<std::string,std::string> stat_map;
      stat_map.insert(std::make_pair("static", "src"));
      request = new Request(req_str, "echo", stat_map);
    }

    void SetEcho(std::string echo_str) {
      std::string req_str = CreateRequestString("GET", "/" + echo_str, "1.1");
      std::map<std::string,std::string> stat_map;
      stat_map.insert(std::make_pair("static", "src"));
      request = new Request(req_str, echo_str, stat_map);
    }

    void SetStatic(std::string stat_str, std::string file_str) {
      std::string req_str = CreateRequestString("GET", "/" + stat_str, "1.1");
      std::map<std::string,std::string> stat_map;
      stat_map.insert(std::make_pair(stat_str,file_str));
      request = new Request(req_str, "echo", stat_map);
    }
    
    void CleanUp() { delete request; }

    Request* request;	
};

TEST_F(RequestTest, GetURI) {
  SetNone("GET", "/example", "1.1");
  EXPECT_EQ(request->GetURI(), "/example");
  CleanUp();
}

TEST_F(RequestTest, GetMethod) {
  SetNone("GET", "/example", "1.1");
  EXPECT_EQ(request->GetMethod(), "GET");
  CleanUp();
}


TEST_F(RequestTest, GetVersion) {
  SetNone("GET", "/example", "1.1");
  EXPECT_EQ(request->GetVersion(), "1.1");
  CleanUp();
}

TEST_F(RequestTest, GetHeader) {
  SetNone("GET", "/example", "1.1");
  EXPECT_EQ(request->GetHeader("Host"), "localhost");
  CleanUp();
}

TEST_F(RequestTest, GetHeaders) {
  SetNone("GET", "/example", "1.1");
  std::map<std::string, std::string> header_map = request->GetHeaders();
  EXPECT_EQ(header_map.size(), 1);
  EXPECT_EQ(header_map["Host"], "localhost");
  CleanUp();
}


TEST_F(RequestTest, GetTypeNone) {
  SetNone("GET", "/example", "1.1");
  EXPECT_EQ(request->GetType(), "None");
  CleanUp();
}

TEST_F(RequestTest, GetTypeEcho) {
  SetEcho("echo");
  EXPECT_EQ(request->GetType(), "Echo");
  CleanUp();
}

TEST_F(RequestTest, GetTypeStatic) {
  SetStatic("static", "src");
  EXPECT_EQ(request->GetType(), "Static");
  CleanUp();
}

TEST_F(RequestTest, GetOriginal) {
  std::string req_str = CreateRequestString("GET", "/example", "1.1");
  SetFromString(req_str);
  EXPECT_EQ(request->GetOriginalString(), req_str);
  CleanUp();
}

TEST_F(RequestTest, GetFileAndStaticPaths) {
  SetStatic("static", "src");
  EXPECT_EQ(request->GetStaticPath(), "static");
  EXPECT_EQ(request->GetFilePath(), "src");
  CleanUp();
}

TEST_F(RequestTest, BadStatusOnlyMethod) {
  SetNone("GET", "", "");
  EXPECT_FALSE(request->IsValid());
  CleanUp();
}

 
TEST_F(RequestTest, BadStatusNoVersion) {
  std::string req_str = "GET /example\r\n";
  SetFromString(req_str);
  EXPECT_FALSE(request->IsValid());
  CleanUp();
}

TEST_F(RequestTest, NoStatus) {
  std::string req_str = "\r\n";
  SetFromString(req_str);
  EXPECT_FALSE(request->IsValid());
  CleanUp();
}

