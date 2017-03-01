#include "proxy_handler.h"
#include "config_parser.h"
#include <boost/asio.hpp>
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
  std::cout << "Handling Request" << std::endl;
  std::string request_uri = request.uri();
  size_t file_prefix_size = uri_prefix_.size() + 1;
  std::cout << "prefix: " << uri_prefix_ << " size: " << file_prefix_size << ". compared to: " <<request.uri() << " size: " << request.uri().length() << std::endl;
  if (!uri_prefix_.compare("/"))
  {
    file_prefix_size = 1;
  }
  if (request_uri.size() < file_prefix_size)
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
  std::cout << "Full path: " << full_path << std::endl;
  Request proxy_request(request);
  proxy_request.SetUri("/"+partial_file_path);
  return PerformRequest(request,response);
}

RequestHandler::Status ProxyHandler::PerformRequest(const Request& request,
						    Response* response) {
  boost::asio::streambuf responseBuf;
  try {
  boost::asio::io_service io_service;
  boost::asio::ip::tcp::socket  socket(io_service);
  boost::asio::ip::tcp::resolver resolver(io_service);
  boost::asio::ip::tcp::resolver::query query(host_uri_, host_port_.length()>0?host_port_:"http");
  boost::asio::ip::tcp::resolver::iterator  endpoint_iterator = resolver.resolve(query);
  
  boost::asio::connect(socket, endpoint_iterator);
  boost::asio::streambuf requestBuf;
  std::ostream request_stream(&requestBuf);
  request_stream << request.ToString();
  // Send the request.
  boost::asio::write(socket, requestBuf);


  boost::system::error_code error;
  
  std::cout<<"Reading"<<std::endl;
  while (boost::asio::read(socket, responseBuf,error));
  if (error != boost::asio::error::eof) {
    throw boost::system::system_error(error);
  }
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
  }
  std::ostringstream ss;
  ss << &responseBuf;
  std::cout<<"Parsing" << ss.str()<<std::endl;
  auto response_p = Response::Parse(ss.str());
  if (response_p.get() != nullptr) {
      std::cout<<"Memory"<<std::endl;
    *response = *response_p;
    return RequestHandler::Status::OK;
  } else {
    //ERROR HANDLING
    return RequestHandler::Status::NOT_FOUND;
  }


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
