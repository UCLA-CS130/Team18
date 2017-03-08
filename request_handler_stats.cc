#include "request_handler_stats.h"

bool RequestHandlerStats::instance_flag = false;
RequestHandlerStats* RequestHandlerStats::stats = NULL;
RequestHandlerStats* RequestHandlerStats::getInstance()
{
  if (!instance_flag) {
    stats = new RequestHandlerStats();
    instance_flag = true;
    return stats;
  } else {
    return stats;
  }
}

void RequestHandlerStats::SetDefaultHandler(std::string handler)
{
  default_handler_ = handler;
}

void RequestHandlerStats::InsertHandler(std::string handler, std::string uri)
{
  std::pair<std::string, std::string> handler_pair = std::make_pair(handler, uri);
  if (!(std::find(handlers_.begin(), handlers_.end(), handler_pair) != handlers_.end())) {
    handlers_.push_back(std::make_pair(handler, uri));
  }
}

void RequestHandlerStats::InsertRequest(std::string url, RequestHandler::Status response)
{
  requests_.push_back(std::make_pair(url, response));
}