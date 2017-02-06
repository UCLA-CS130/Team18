#ifndef REQUEST_HANDLER
#define REQUEST_HANDLER

class request;
class response;

class request_handler {
  public:
    virtual void handle_request(const request& req, response& rep) = 0;
};

#endif