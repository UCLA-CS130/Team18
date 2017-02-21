#include "echo_handler.h"

RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix,
                         				 const NginxConfig& config)
{
  return RequestHandler::Status::OK;
}

RequestHandler::Status EchoHandler::HandleRequest(const Request& request,
    											  Response* repsponse)
{
  repsponse->http_version = request.version();
  repsponse->status = Response::ok;
  repsponse->headers["Content-Type"] = "text/plain";
  std::size_t body_length = request.raw_request().find("\r\n\r\n") + size_t(4);
  repsponse->headers["Content-Length"] = std::to_string((int) body_length);
  repsponse->body = request.raw_request();
  return RequestHandler::Status::OK;
}
