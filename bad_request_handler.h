#ifndef BAD_REQUEST_HANDLER
#define BAD_REQUEST_HANDLER

#include "request_handler.h"
#include "request.h"
#include "response.h"

class BadRequestHandler : public RequestHandler {
  public:
    BadRequestHandler() {}
    virtual RequestHandler::Status Init(const std::string& uri_prefix,
					const NginxConfig& config);
    virtual RequestHandler::Status HandleRequest(const Request& request,
						 Response* repsponse);
};

REGISTER_REQUEST_HANDLER(BadRequestHandler);

#endif
