#ifndef RESPONSE
#define RESPONSE

#include <string>
#include <map>

class Response {
  public:
    std::string ToString();
    enum ResponseCode {
  	  ok = 200,
  	  bad_request = 400,
  	  not_found = 404
    };
    void SetStatus(const ResponseCode response_code);
    void AddHeader(const std::string& header_name, const std::string& header_value);
    void SetBody(const std::string& body);
       
  private:
    std::string get_header(std::string header);
    std::string to_string(Response::ResponseCode status);
    Response::ResponseCode status;
    std::map<std::string,std::string> headers;
    std::string response_body;
};

#endif
