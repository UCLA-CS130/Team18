#ifndef STATIC_HANDLER
#define STATIC_HANDLER

#include "request_handler.h"
#include <string>
class Request;
class Response;


class StaticHandler: public request_handler {
  public:
    StaticHandler(std::string filepath, std::string staticpath);
    virtual void handle_request(Request* req, Response* rep);

  private:
    enum Extension {NOEXT, HTML, JPG, TXT} ext;
    void GetExtension(std::string uri);
    std::string GetContentType();
    void SetNotFound(Request* req, Response* res);
    void SetOk(Request* req, Response* res, std::string file_body);
    std::string file_path;
    std::string static_path;
};

#endif
