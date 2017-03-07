#include "proxy_handler.h"
#include "config_parser.h"
#include <boost/asio.hpp>
RequestHandler::Status ProxyHandler::Init(const std::string& uri_prefix,
                                         const NginxConfig& config)
{
  not_found_handler_ = (RequestHandler::CreateByName("NotFoundHandler"));
  not_found_handler_->Init("", config);
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
  if (!uri_prefix_.compare("/"))
  {
    file_prefix_size = 1;
  }
  if (request_uri.size() < file_prefix_size)
  {
    SetNotFound(request, response);
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
  proxy_request.SetUri("/"+partial_file_path);
  auto headers = proxy_request.headers();
  proxy_request.SetHeader("Host", host_uri_);
  proxy_request.SetHeader("Connection", "close");
  return PerformRequest(proxy_request,response,host_uri_,host_port_);
}

RequestHandler::Status ProxyHandler::PerformRequest(Request& request,
						    Response* response,
						    const std::string& host_uri,
						    const std::string& host_port) {
  try {
    boost::system::error_code ec;
  boost::asio::io_service io_service;
  boost::asio::ip::tcp::resolver resolver(io_service);
  boost::asio::ip::tcp::resolver::iterator  endpoint_iterator = resolver.resolve({host_uri,host_port},ec);
  if (ec) throw boost::system::system_error(ec);
  boost::asio::ip::tcp::socket  socket(io_service);
  boost::asio::connect(socket, endpoint_iterator,ec);
  socket.set_option(boost::asio::ip::tcp::no_delay(true));
  if (ec) throw boost::system::system_error(ec);
  boost::asio::streambuf requestBuf;
  std::ostream request_stream(&requestBuf);

  request_stream << request.ToString();
  // Send the request.
  boost::asio::write(socket, requestBuf,ec);
  if (ec) throw boost::system::system_error(ec);
  boost::asio::streambuf response_stream_buf;
  boost::asio::read(socket,response_stream_buf,boost::asio::transfer_all(),ec);
  if (ec && ec != boost::asio::error::eof) throw boost::system::system_error(ec);
  
  //parse the message
  std::ostringstream ss;
  ss << &response_stream_buf;
  auto response_p = Response::Parse(ss.str());
  if (response_p.get() != nullptr) {
    if (response_p->GetStatus().find("302")!=std::string::npos) {
      std::cout << "Found cout, redirecting" << std::endl;
      //we need to redirect
      std::string new_location = response_p->get_header("Location").substr(17)//get rid of Location http://
	;
      std::size_t first_slash_location = new_location.find("/");
      //TODO check for PORT
      request.SetUri(new_location.substr(first_slash_location));
      request.SetHeader("Host",new_location.substr(0,first_slash_location));
      PerformRequest(request,response,new_location.substr(0,first_slash_location),"80");
    }
    else {
      //no redirect, all good to go
      *response = *response_p.get(); // copy constructor
      return RequestHandler::Status::OK;
    }
  } else {
    SetNotFound(request,response);
    return RequestHandler::Status::NOT_FOUND;
  }
  return RequestHandler::Status::OK;
  }
  catch (std::exception& e)
    {
      std::cout << "Exception: " << e.what() << std::endl;
      SetNotFound(request,response);
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

void ProxyHandler::SetNotFound(const Request& req, Response* res)
{
  not_found_handler_->HandleRequest(req, res);
}
