#include "session.h"
#include "request.h"
#include "config_parser.h"
#include "request_handler_stats.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

Session::Session(boost::asio::io_service& io_service, NginxConfig* config)
         : socket_(io_service),
           request(nullptr),
           response(new Response())
{
  init_handlers(config);
}

Session::~Session()
{
  delete gzip_handler_;

  for(std::map<std::string, RequestHandler*>::iterator it = handlers_.begin(); it != handlers_.end(); it++)
  {
    delete it->second;
    handlers_.erase(it);
  }
}

void Session::init_handlers(NginxConfig* config)
{
  gzip_handler_ = new GzipHandler();

  RequestHandlerStats *stats = RequestHandlerStats::getInstance();
  for (unsigned i = 0; i < (unsigned long) config->statements_.size(); i++) {
    std::shared_ptr<NginxConfigStatement> config_statement = config->statements_[i];
    if (config_statement->tokens_[0] == "path" && config_statement->tokens_.size() == 3) {
      std::string uri = config_statement->tokens_[1];
      handlers_[uri] = (RequestHandler::CreateByName(config_statement->tokens_[2].c_str()));
      handlers_[uri]->Init(config_statement->tokens_[1], *config_statement->child_block_.get());
      stats->RequestHandlerStats::InsertHandler(config_statement->tokens_[2].c_str(), uri);
    } else if (config_statement->tokens_[0] == "default" && config_statement->tokens_.size() == 2) {
      default_handler_ = (RequestHandler::CreateByName(config_statement->tokens_[1].c_str()));
      default_handler_->Init("", *config_statement->child_block_.get());
      stats->SetDefaultHandler(config_statement->tokens_[1].c_str());
    }
  }
}

void Session::do_read()
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            bool reached_end = check_input(length, data_);
            if (!ec) 
            {   if (reached_end)
                {
                    std::cout << "REACHED END" << std::endl;
                    request = Request::Parse(msg);
                    std::string matching_string = "";
                    for (std::map<std::string, RequestHandler*>::iterator it =
                         handlers_.begin(); it != handlers_.end(); it++) {
                      if (request->uri().find(it->first) == 0) {
                        if (it->first.size() > matching_string.size())
                          matching_string = it->first;
                      }
                    }
                    
                    RequestHandler::Status status;
                    if (matching_string.empty())
                      status = default_handler_->HandleRequest(*request, response);
                    else
                      status = handlers_[matching_string]->HandleRequest(*request, response);      
                    
                    std::vector<std::pair<std::string, std::string>> headers = request->headers();
                    std::pair<std::string, std::string> accept_gzip = std::make_pair("Accept-Encoding", "gzip, deflate");
                    if (std::find(headers.begin(), headers.end(), accept_gzip) != headers.end()) {
                      gzip_handler_->HandleRequest(*request, response);
                    }

                    RequestHandlerStats *stats = RequestHandlerStats::getInstance();
                    stats->InsertRequest(request->uri(), status);

                    //handlers_[request->uri()]->HandleRequest(*request, response);
                    send_http();
                    request.release();
                    delete response;
                    msg = "";
                    response = new Response();
                }
                else 
                    do_read();
            }
        });
      
}

bool Session::check_input(std::size_t length, char* buffer)
{
  std::string received(buffer, buffer + length);
  std::cout << "RCVD: " << received;
  msg += received;
  std::cout << "MSG: " << msg;
  return (msg.find("\r\n\r\n") != std::string::npos);
}

std::string Session::OutputToString()
{
    std::string s(to_send);
    return s;       
}

std::size_t Session::prepare_response(int status, std::string body)
{
  std::string status_line = "HTTP/1.1 ";
  switch(status)
  {
    case 200: 
      status_line += "200 OK\r\n";      
      break;
    default:
      status_line += "400 Bad Request\r\n";
  }
  std::size_t body_length = body.find("\r\n\r\n") + size_t(4);
  std::string content_type = "Content-Type: text/plain\r\n";
  std::string content_length = "Content-Length: " +  std::to_string((int)body_length) + "\r\n";
  std::string header = status_line + content_type + content_length + "\r\n";
  std::size_t len = header.size() + body_length;
  body = header + body;
  to_send = body; 
  std::cout << "SENDING MSG: " << body;
  msg = "";
  return len;
} 

void Session::send_http() {
    to_send = response->ToString();
    do_write();
}

void Session::do_write()
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(to_send.c_str(), to_send.size()),
        [this, self](boost::system::error_code ec, std::size_t)
        {
            if (!ec)
            {
                do_read();
            }
        });
}
