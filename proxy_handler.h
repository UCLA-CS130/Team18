#ifndef PROXY_HANDLER
#define PROXY_HANDLER
#include <boost/asio.hpp>
#include "request_handler.h"

class ProxyHandler : public RequestHandler {
  public:
    virtual RequestHandler::Status Init(const std::string& uri_prefix,
                                        const NginxConfig& config);
    virtual RequestHandler::Status HandleRequest(const Request& request,
                                                 Response* response);
 private:
    const bool GetHostAndPort(std::string& host, std::string& port, const NginxConfig& config);
    RequestHandler::Status PerformRequest(const Request& request,
					  Response* response);
    std::string uri_prefix_;
    std::string host_uri_;
    std::string host_port_;
};

REGISTER_REQUEST_HANDLER(ProxyHandler);

#endif
