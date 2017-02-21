#ifndef STATIC_HANDLER
#define STATIC_HANDLER

#include "request_handler.h"

class StaticHandler: public RequestHandler {
  public:
    StaticHandler();
    virtual RequestHandler::Status Init(const std::string& uri_prefix,
                                        const NginxConfig& config);
    virtual RequestHandler::Status HandleRequest(const Request& request,
    							                 Response* response);

  private:
    enum Extension {NOEXT, HTML, JPG, TXT, CSS, GIF, JS, JSON} ext;
    void GetExtension(std::string uri);
    std::string GetContentType();
    void SetNotFound(const Request& req, Response* res);
    void SetOk(const Request& req, Response* res, std::string file_body);
    const bool GetRootPath(std::string& root_path, const NginxConfig& config);
    std::string root_path_;
    std::string uri_prefix_;
};

REGISTER_REQUEST_HANDLER(StaticHandler);

#endif
