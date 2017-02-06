#ifndef RESPONSE
#define RESPONSE

#include <string>
#include <map>

class response {
  public:
    std::size_t to_buffer(char* response_buf);
    std::map<std::string, std::string> headers;
    std::string body;
    std::size_t length;
  private:
  	std::string get_header(std::string header);
};

#endif
