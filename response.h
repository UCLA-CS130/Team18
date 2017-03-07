#ifndef RESPONSE
#define RESPONSE

#include <string>
#include <map>
#include <memory>
#include <sstream>
class Response {
  public:
 Response():version("HTTP/1.1"){}
    std::string ToString();
    enum ResponseCode {
  	  ok = 200,
  	  bad_request = 400,
  	  not_found = 404
    };
static std::unique_ptr<Response> Parse(const std::string& raw_response)
    { 
      std::unique_ptr<Response> req(new Response(raw_response));
      if (req->valid_)
        return req;
      else
        return std::unique_ptr<Response>(nullptr);
    }
    void SetStatus(const ResponseCode response_code);
    void AddHeader(const std::string& header_name, const std::string& header_value);
    void SetBody(const std::string& body);
    std::string get_header(std::string header);
    std::string GetStatus() const { return status; }
  private:
    Response(const std::string& response_string);

    std::string to_string(Response::ResponseCode status);
    std::string status;
    std::map<std::string,std::string> headers;
    std::string response_body;
    std::string version;
    
    bool ParseResponseString(const std::string& response_string);
    bool DecodeStatus(std::string status_line);
    bool valid_;
};

#endif
