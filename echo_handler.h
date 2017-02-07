#ifndef ECHO_HANDLER
#define ECHO_HANDLER

#include "request_handler.h"
#include "request.h"
#include "response.h"

class EchoHandler : public request_handler {
  public:
    EchoHandler() {}
    void handle_request(Request* req, Response* rep);
};

#endif