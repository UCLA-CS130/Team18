#include "not_found_handler.h"

RequestHandler::Status NotFoundHandler::Init(const std::string& uri_prefix,
                                             const NginxConfig& config)
{
  return RequestHandler::Status::OK;
}

RequestHandler::Status NotFoundHandler::HandleRequest(const Request& request,
                                                      Response* response)
{
  std::string not_found_page = 
    "<html><body><h1>Not Found!</h1></body></html>";
  std::string content_length = std::to_string((int) not_found_page.size());
  response->SetStatus(Response::not_found);
  response->AddHeader("Content-Length", content_length);
  response->SetBody(not_found_page);
  return RequestHandler::Status::OK;
}
