#ifndef RESPONSE
#define RESPONSE

#include <string>
#include <map>

class Response {
  public:
    std::string to_buffer();
    std::string http_version;
    enum status_type {
  	  ok = 200,
  	  bad_request = 400,
  	  not_found = 404
    } status;
    std::map<std::string, std::string> headers;
    std::string body;
  private:
    std::string get_header(std::string header);
    std::string to_string(Response::status_type status);
};

#endif
