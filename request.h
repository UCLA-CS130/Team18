#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>

class Request {
  public:
    static std::unique_ptr<Request> Parse(const std::string& raw_request)
    {  return std::unique_ptr<Request>(new Request(raw_request)); }
    std::string method() const;
    std::string uri() const;
    std::string version() const;
    std::string raw_request() const;
    
    using Headers = std::vector<std::pair<std::string, std::string>>;
    Headers headers() const;
 
    std::string body() const;
 
  private:
    Request(std::string request_string); 
    std::string original_string_;
    std::string method_;
    std::string uri_;
    std::string http_version_;
    std::string body_;
    Headers headers_;
    bool ParseRequestString(std::string request_string);
    bool DecodeStatus(std::string status_line);
};


#endif
