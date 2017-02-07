#ifndef BAD_REQUEST_HANDLER
#define BAD_REQUEST_HANDLER

#include "request_handler.h"
#include "request.h"
#include "response.h"

class BadRequestHandler : public request_handler {
  public:
    BadRequestHandler() {}
    void handle_request(Request* req, Response* rep);
};

#endif