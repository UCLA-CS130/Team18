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
  std::string status_page = "<html>";
  status_page += GetDefaultHTML();
  status_page += GetRequestHandlerHTML();
  status_page += GetURLRequestsHTML();

  status_page += "</html>";
  std::string content_length = std::to_string((int) status_page.size());
  response->SetStatus(Response::ok);
  response->AddHeader("Content-Length", content_length);
  response->SetBody(status_page);
  return RequestHandler::Status::OK;
}

std::string StatusHandler::GetDefaultHTML()
{
  RequestHandlerStats *stats = RequestHandlerStats::getInstance();
  std::string default_html;
  default_html += "<p>Default Handler: ";
  std::string default_ = stats->GetDefaultHandler();
  if (!default_.empty()) {
    default_html += default_;
  } else {
    default_html += "Default Handler not specified";
  }
  default_html += "</p>";
  return default_html;
}

std::string StatusHandler::GetRequestHandlerHTML()
{
  RequestHandlerStats *stats = RequestHandlerStats::getInstance();
  std::string request_handler_html;
  request_handler_html += "<table><tr><th>Request Handler</th><th>URL Prefix</th></tr>";
  std::vector<std::pair<std::string, std::string>> handlers_ = stats->GetHandlers();
  for (unsigned i = 0; i < handlers_.size(); i++) {
    request_handler_html += "<tr><th>";
    request_handler_html += handlers_[i].first;
    request_handler_html += "</th><th>";
    request_handler_html += handlers_[i].second;
    request_handler_html += "</th></tr>";
  }
  request_handler_html += "</table>";

  return request_handler_html;
}

std::string StatusHandler::GetURLRequestsHTML()
{
  RequestHandlerStats *stats = RequestHandlerStats::getInstance();
  std::string url_requests_html;
  std::vector<std::pair<std::string, RequestHandler::Status>> requests_ = stats->GetRequests();
  url_requests_html += "<p>Number of Requests: ";
  int size = requests_.size();
  if (size) {
    url_requests_html += std::to_string(size);
  } else {
    url_requests_html += "0";
  }
  url_requests_html += "</p>";
  url_requests_html += "<table><tr><th>URL Request</th><th>Response Code</th></tr>";
  for (unsigned i = 0; i < requests_.size(); i++) {
    url_requests_html += "<tr><th>";
    url_requests_html += requests_[i].first;
    url_requests_html += "</th><th>";
    url_requests_html += std::to_string(requests_[i].second);
    url_requests_html += "</th></tr>";
  }
  url_requests_html += "</table>";

  return url_requests_html;
}