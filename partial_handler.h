#ifndef PARTIAL_HANDLER
#define PARTIAL_HANDLER

#include "request_handler.h"

class PartialHandler : public RequestHandler {
  public:
    virtual RequestHandler::Status Init(const std::string& uri_prefix,
                                        const NginxConfig& config);
    virtual RequestHandler::Status HandleRequest(const Request& request,
                                                 Response* response);
};

REGISTER_REQUEST_HANDLER(PartialHandler);

#endif
