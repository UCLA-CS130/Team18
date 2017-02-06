#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include <stdlib.h>
#include <string>
#include <map>

class Request {
  public:
    Request(std::string request_string, std::string echo, std::string stat) 
    : echo_string(echo), static_string(stat)
    {
      message_type = NONE;
      original_string = request_string;
      ParseRequestString(request_string);
    }

    std::string GetMethod() { return method; }
    std::string GetURI() { return uri; }
    std::string GetVersion() { return http_version; }
    std::map<std::string,std::string> GetHeaders() { return headers; }
    std::string GetHeader(std::string header_name);
    std::string GetType();

  private:
    std::string original_string;
    std::string method;
    std::string uri;
    std::string http_version;
    std::map<std::string, std::string> headers;
    bool ParseRequestString(std::string request_string);
    bool DecodeStatus(std::string status_line);
    bool GetRequestType(std::string uri);
    enum Type { NONE, ECHO, STAT };
    std::string echo_string;
    std::string static_string;
    Type message_type;
};


#endif
