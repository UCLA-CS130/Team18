#include "echo_handler.h"

RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix,
                                         const NginxConfig& config)
{
  return RequestHandler::Status::OK;
}

RequestHandler::Status EchoHandler::HandleRequest(const Request& request,
                                                  Response* response)
{
  response->SetStatus(Response::ok);
  response->AddHeader("Content-Type", "text/plain");
  std::size_t body_length = request.raw_request().find("\r\n\r\n") + size_t(4);
  response->AddHeader("Content-Length", std::to_string((int) body_length));
  response->SetBody(request.raw_request());
  return RequestHandler::Status::OK;
}
