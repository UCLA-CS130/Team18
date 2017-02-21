#ifndef ECHO_HANDLER
#define ECHO_HANDLER

#include "request_handler.h"

class EchoHandler : public RequestHandler {
  public:
    virtual RequestHandler::Status Init(const std::string& uri_prefix,
					const NginxConfig& config);
    virtual RequestHandler::Status HandleRequest(const Request& request,
						 Response* repsponse);
};

REGISTER_REQUEST_HANDLER(EchoHandler);

#endif
