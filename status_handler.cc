#include "status_handler.h"
#include "request_handler_stats.h"

RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix,
                                           const NginxConfig& config)
{
  return RequestHandler::Status::OK;
}

RequestHandler::Status StatusHandler::HandleRequest(const Request& request,
                                                    Response* response)
{
  RequestHandlerStats *stats = RequestHandlerStats::getInstance();
  std::string status_page = "<html>";
  status_page += "<p>Default Handler: ";
  std::string default_ = stats->GetDefaultHandler();
  if (!default_.empty()) {
    status_page += stats->GetDefaultHandler();
  } else {
    status_page += "Default Handler not specified";
  }
  status_page += "</p>";

  status_page += "<table><tr><th>Request Handler</th><th>URL Prefix</th></tr>";
  std::vector<std::pair<std::string, std::string>> handlers_ = stats->GetHandlers();
  for (unsigned i = 0; i < handlers_.size(); i++) {
    status_page += "<tr><th>";
    status_page += handlers_[i].first;
    status_page += "</th><th>";
    status_page += handlers_[i].second;
    status_page += "</th></tr>";
  }
  status_page += "</table>";

  std::vector<std::pair<std::string, RequestHandler::Status>> requests_ = stats->GetRequests();
  status_page += "<p>Number of Requests: ";
  int size = requests_.size();
  if (size) {
    status_page += std::to_string(size);
  } else {
    status_page += "0";
  }
  status_page += "</p>";
  status_page += "<table><tr><th>URL Request</th><th>Response Code</th></tr>";
  for (unsigned i = 0; i < requests_.size(); i++) {
    status_page += "<tr><th>";
    status_page += requests_[i].first;
    status_page += "</th><th>";
    status_page += std::to_string(requests_[i].second);
    status_page += "</th></tr>";
  }
  status_page += "</table>";

  status_page += "</html>";
  std::string content_length = std::to_string((int) status_page.size());
  response->SetStatus(Response::ok);
  response->AddHeader("Content-Length", content_length);
  response->SetBody(status_page);
  return RequestHandler::Status::OK;
}

std::string GetDefaultHTML()
{
  return "Hello";
}
std::string GetRequestHandlerHTML()
{
  return "Hello";
}
std::string GetURLRequestsHTML()
{
  return "Hello";
}