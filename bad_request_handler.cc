#include "bad_request_handler.h"

RequestHandler::Status BadRequestHandler::Init(const std::string& uri_prefix,
                           	   const NginxConfig& config)
{
  return RequestHandler::Status::ERROR;
}

RequestHandler::Status BadRequestHandler::handle_request(Request* req, Response* rep) {
  rep->http_version = req.GetVersion();
  rep->status = Response::bad_request;
  rep->headers["Content-Length"] = "0";
  return RequestHandler::Status::ERROR;
}
