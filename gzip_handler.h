#ifndef GZIP_HANDLER
#define GZIP_HANDLER

#include "request_handler.h"

class GzipHandler: public RequestHandler {
  public:
    virtual RequestHandler::Status Init(const std::string& uri_prefix,
                                        const NginxConfig& config);
    virtual RequestHandler::Status HandleRequest(const Request& request,
    							                 Response* response);
};

REGISTER_REQUEST_HANDLER(GzipHandler);

#endif
