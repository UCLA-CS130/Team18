#ifndef STATIC_HANDLER
#define STATIC_HANDLER

#include "request_handler.h"

class StaticHandler: public RequestHandler {
  public:
    StaticHandler();
    virtual RequestHandler::Status Init(const std::string& uri_prefix,
                                        const NginxConfig& config);
    virtual RequestHandler::Status HandleRequest(const Request& request,
    							                 Response* repsponse);

  private:
    enum Extension {NOEXT, HTML, JPG, TXT, CSS, GIF, JS, JSON} ext;
    void GetExtension(std::string uri);
    std::string GetContentType();
    void SetNotFound(Request* req, Response* res);
    void SetOk(Request* req, Response* res, std::string file_body);
};

REGISTER_REQUEST_HANDLER(StaticHandler);

#endif
