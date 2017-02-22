#include "bad_request_handler.h"

RequestHandler::Status BadRequestHandler::Init(const std::string& uri_prefix,
                                               const NginxConfig& config)
{
  return RequestHandler::Status::OK;
}

RequestHandler::Status BadRequestHandler::HandleRequest(const Request& request,
                                                        Response* response)
{
  std::string bad_request_page = 
    "<html><body><h1>Bad Request!</h1></body></html>";
  std::string content_length = std::to_string((int) bad_request_page.size());
  response->SetStatus(Response::bad_request);
  response->AddHeader("Content-Length", content_length);
  response->SetBody(bad_request_page);
  return RequestHandler::Status::BAD_REQUEST;
}
