#include "gtest/gtest.h"
#include "response.h"

class ResponseTest : public ::testing::Test {
  protected:
    std::string GetBuffer(std::string http_version,
    					  Response::ResponseCode status,
    					  std::map<std::string, std::string> headers,
    					  std::string body) {
      _response = new Response();
      _response->SetStatus(status);
      _response->SetBody(body);
      for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
        _response->AddHeader(it->first, it->second);
      }
      return _response->ToString();
    }

    Response* _response;	
};

TEST_F(ResponseTest, ok_Response) {
  std::map<std::string, std::string> headers;
  headers["Content-Type"] = "text/plain";
  headers["Content-Length"] = "10";
  EXPECT_EQ("HTTP/1.1 200 OK\r\nContent-Length: 10\r\nContent-Type: text/plain\r\n\r\nVery simple body",
  			GetBuffer("1.0",
					  Response::ok,
					  headers,
					  "Very simple body")
  		   ) << "Expected different buffer";
}

TEST_F(ResponseTest, bad_request_Response) {
  std::map<std::string, std::string> headers;
  headers["Content-Type"] = "text/plain";
  headers["Content-Length"] = "10";
  EXPECT_EQ("HTTP/1.1 400 Bad Request\r\nContent-Length: 10\r\nContent-Type: text/plain\r\n\r\nVery simple body",
  			GetBuffer("1.0",
					  Response::bad_request,
					  headers,
					  "Very simple body")
  		   ) << "Expected different buffer";
}

TEST_F(ResponseTest, not_found_Response) {
  std::map<std::string, std::string> headers;
  headers["Content-Type"] = "text/plain";
  headers["Content-Length"] = "10";
  EXPECT_EQ("HTTP/1.1 404 Not Found\r\nContent-Length: 10\r\nContent-Type: text/plain\r\n\r\nVery simple body",
  			GetBuffer("1.0",
					  Response::not_found,
					  headers,
					  "Very simple body")
  		   ) << "Expected different buffer";
}
