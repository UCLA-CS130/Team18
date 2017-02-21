#ifndef NOT_FOUND_HANDLER
#define NOT_FOUND_HANDLER

#include "request_handler.h"
#include "request.h"
#include "response.h"

class NotFoundHandler : public RequestHandler {
  public:
    NotFoundHandler() {}
    virtual RequestHandler::Status Init(const std::string& uri_prefix,
    									const NginxConfig& config);
    virtual RequestHandler::Status HandleRequest(const Request& request,
    											 Response* repsponse);
};

REGISTER_REQUEST_HANDLER(NotFoundHandler);

#endif
