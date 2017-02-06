#ifndef REQUEST_HANDLER
#define REQUEST_HANDLER

class Request;
class Response;

class request_handler {
  public:
    virtual void handle_request(Request& req, Response& rep) = 0;
};

#endif