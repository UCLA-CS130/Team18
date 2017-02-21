#include "bad_request_handler.h"

RequestHandler::Status BadRequestHandler::Init(const std::string& uri_prefix,
                                               const NginxConfig& config)
{
  return RequestHandler::Status::OK;
}

RequestHandler::Status BadRequestHandler::HandleRequest(const Request& request,
                                                        Response* response)
{
  response->SetStatus(Response::bad_request);
  response->AddHeader("Content-Length", "0");
  return RequestHandler::Status::OK;
}
