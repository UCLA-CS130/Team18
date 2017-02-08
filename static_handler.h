#ifndef STATIC_HANDLER
#define STATIC_HANDLER

#include "request_handler.h"
#include <string>
#include <map>
class Request;
class Response;


class StaticHandler: public request_handler {
  public:
    StaticHandler();
    virtual void handle_request(Request* req, Response* rep);

  private:
    enum Extension {NOEXT, HTML, JPG, TXT, CSS, GIF, JS, JSON} ext;
    void GetExtension(std::string uri);
    std::string GetContentType();
    void SetNotFound(Request* req, Response* res);
    void SetOk(Request* req, Response* res, std::string file_body);
};

#endif
