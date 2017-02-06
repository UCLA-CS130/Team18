#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include <stdlib.h>
#include <string>
#include <map>

class Request {
  public:
    Request(std::string request_string, std::string echo, std::string stat);
    std::string GetMethod();
    std::string GetURI();
    std::string GetVersion();
    std::map<std::string,std::string> GetHeaders();
    std::string GetHeader(std::string header_name);
    std::string GetType();
    enum Type { NONE, ECHO_MODE, STAT_MODE };
 
  private:
    std::string original_string;
    std::string method;
    std::string uri;
    std::string http_version;
    std::map<std::string, std::string> headers;
    bool ParseRequestString(std::string request_string);
    bool DecodeStatus(std::string status_line);
    bool GetRequestType(std::string uri);
    Request::Type message_type;
    std::string echo_string;
    std::string static_string;
};


#endif