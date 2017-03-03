#include "partial_handler.h"

RequestHandler::Status PartialHandler::Init(const std::string& uri_prefix,
                                            const NginxConfig& config)
{
  return RequestHandler::Status::OK;
}

RequestHandler::Status PartialHandler::HandleRequest(const Request& request,
                                                     Response* response)
{
  while (true) { }
  return RequestHandler::Status::OK;
}
