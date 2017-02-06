#ifndef ECHO_HANDLER
#define ECHO_HANDLER

#include "request_handler.h"
#include "request.h"
#include "response.h"

class echo_handler : public request_handler {
  public:
  	echo_handler() {}
  	void handle_request(Request& req, Response& rep);
};

#endif