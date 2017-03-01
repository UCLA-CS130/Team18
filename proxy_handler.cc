#include "proxy_handler.h"
#include "config_parser.h"

RequestHandler::Status ProxyHandler::Init(const std::string& uri_prefix,
                                         const NginxConfig& config)
{
  uri_prefix_ = uri_prefix;
  std::string host;
  std::string port = "";
  if (GetHostAndPort(host,port,config)) {
    host_uri_ = host;
    host_port_ = port;
    return RequestHandler::Status::OK;
  } else {
    return RequestHandler::Status::BAD_REQUEST;
  }
}

RequestHandler::Status ProxyHandler::HandleRequest(const Request& request,
                                                  Response* response)
{
  std::string request_uri = request.uri();
  size_t file_prefix_size = uri_prefix_.size() + 1;
  if (!uri_prefix_.compare("/"))
  {
    file_prefix_size = 1;
  }
  if (request_uri.size() <= file_prefix_size)
  {
    //TODO
    //SetNotFound(request, response);
    return RequestHandler::Status::NOT_FOUND;
  }
  std::string partial_file_path = request_uri.substr(file_prefix_size);
  std::string root = host_uri_;
  if (host_port_.length() > 0) {
    root += ":";
    root += host_port_;
  }

  std::string full_path = root + "/" + partial_file_path;
  
  Request proxy_request(request);
  proxy_request.SetUri(full_path);

  response->SetStatus(Response::ok);
  response->AddHeader("Content-Type", "text/plain");
  std::size_t body_length = request.raw_request().find("\r\n\r\n") + size_t(4);
  response->AddHeader("Content-Length", std::to_string((int) body_length));
  response->SetBody(request.raw_request());
  return RequestHandler::Status::OK;
}

const bool ProxyHandler::GetHostAndPort(std::string& host, std::string& port, const NginxConfig& config) {
  bool foundHost;
  for (unsigned i = 0; i < (unsigned long) config.statements_.size(); i++) {
    std::shared_ptr<NginxConfigStatement> config_statement = config.statements_[i];
    if (config_statement->tokens_[0] == "host" && config_statement->tokens_.size() == 2) {
      host = config_statement->tokens_[1];
      foundHost = true;
    }
    else if (config_statement->tokens_[0] == "port" && config_statement->tokens_.size() == 2) {
      port = config_statement->tokens_[1];
    }
  }
  return foundHost;
}
