#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>

class Request {
  public:
  using Headers = std::vector<std::pair<std::string, std::string>>;

 Request(const Request& r):original_string_(r.original_string_),method_(r.method_),
    uri_(r.uri_),http_version_(r.http_version_),body_(r.body_),headers_(r.headers_){}
    static std::unique_ptr<Request> Parse(const std::string& raw_request)
    { 
      std::unique_ptr<Request> req(new Request(raw_request));
      if (req->valid_)
        return req;
      else
        return std::unique_ptr<Request>(nullptr);
    }
    std::string method() const;
    void SetMethod(const std::string& method) { method_ = method; }
    std::string uri() const;
    void SetUri(const std::string& uri) { uri_ = uri; }
    std::string version() const;
    void SetVersion(const std::string& version) { http_version_ = version; }
    std::string raw_request() const;
    
    std::string ToString() const;

    Headers headers() const;
    
    std::string body() const;
    void SetBody(const std::string& body) { body_ = body; }
    void SetHeader(const std::string& header, const std::string& val);
  private:
    Request(const std::string& request_string); 
    std::string original_string_;
    std::string method_;
    std::string uri_;
    std::string http_version_;
    std::string body_;
    Headers headers_;
    bool ParseRequestString(std::string request_string);
    bool DecodeStatus(std::string status_line);
    bool valid_;
};


#endif
