#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "request_handler.h"

class RequestHandlerStats
{
  public:
    static RequestHandlerStats* getInstance();
    void SetDefaultHandler(std::string handler);
    void InsertHandler(std::string handler, std::string uri);
    void InsertRequest(std::string url, RequestHandler::Status response);
    std::string GetDefaultHandler() { return default_handler_; }
    std::vector<std::pair<std::string, std::string>> GetHandlers() { return handlers_; }
    std::vector<std::pair<std::string, RequestHandler::Status>> GetRequests() { return requests_; }
    ~RequestHandlerStats() { instance_flag = false; }
  private:
    static bool instance_flag;
    static RequestHandlerStats *stats;
    std::string default_handler_;
    std::vector<std::pair<std::string, std::string>> handlers_;
    std::vector<std::pair<std::string, RequestHandler::Status>> requests_;
};