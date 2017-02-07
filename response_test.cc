#include "gtest/gtest.h"
#include "response.h"

class ResponseTest : public ::testing::Test {
  protected:
    std::string GetBuffer(std::string http_version,
    					  Response::status_type status,
    					  std::map<std::string, std::string> headers,
    					  std::string body) {
      _response = new Response();
      _response->http_version = http_version;
      _response->status = status;
      _response->headers = headers;
      _response->body = body;
      return _response->to_buffer();
    }

    Response* _response;	
};

TEST_F(ResponseTest, ok_Response) {
  std::map<std::string, std::string> headers;
  headers["Content-Type"] = "text/plain";
  headers["Content-Length"] = "10";
  EXPECT_EQ("HTTP/1.0 200 OK\r\nContent-Length: 10\r\nContent-Type: text/plain\r\n\r\nVery simple body",
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
  EXPECT_EQ("HTTP/1.0 400 Bad Request\r\nContent-Length: 10\r\nContent-Type: text/plain\r\n\r\nVery simple body",
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
  EXPECT_EQ("HTTP/1.0 404 Not Found\r\nContent-Length: 10\r\nContent-Type: text/plain\r\n\r\nVery simple body",
  			GetBuffer("1.0",
					  Response::not_found,
					  headers,
					  "Very simple body")
  		   ) << "Expected different buffer";
}
