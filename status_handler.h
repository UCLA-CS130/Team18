#ifndef STATUS_HANDLER
#define STATUS_HANDLER

#include "request_handler.h"
#include "request.h"
#include "response.h"

class StatusHandler : public RequestHandler {
  public:
    StatusHandler() {}
    virtual RequestHandler::Status Init(const std::string& uri_prefix,
                                        const NginxConfig& config);
    virtual RequestHandler::Status HandleRequest(const Request& request,
                                                 Response* response);
  private:
    std::string GetDefaultHTML();
    std::string GetRequestHandlerHTML();
    std::string GetURLRequestsHTML();
};

REGISTER_REQUEST_HANDLER(StatusHandler);

#endif
