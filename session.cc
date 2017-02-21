#include "session.h"
#include "echo_handler.h"
#include "static_handler.h"
#include "bad_request_handler.h"
#include "request.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <boost/bind.hpp>


Session::Session(tcp::socket* socket, NginxConfig* config)
         : socket_(std::move(*socket)),
           request(nullptr),
           response(new Response())
{
  init_handlers(config);
}

Session::~Session()
{
  //TODO: Delete allocated objects 
}

void Session::init_handlers(NginxConfig* config)
{
  for (unsigned i = 0; i < (unsigned long) config->statements_.size(); i++) {
    std::shared_ptr<NginxConfigStatement> config_statement = config->statements_[i];
    if (config_statement->tokens_[0] == "path" && config_statement->tokens_.size() == 3) {
      handlers_.push_back(RequestHandler::CreateByName(config_statement->tokens_[2].c_str()));
      handlers_.back()->Init(config_statement->tokens_[1], *config_statement->child_block_.get());
    } else if (config_statement->tokens_[0] == "default" && config_statement->tokens_.size() == 2) {

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
                    request = new Request(msg, options_->echo_path, options_->static_paths);
                    std::string request_type = request->GetType();

                    //TODO: Get Appropriate Handler
                    handlers_[0]->HandleRequest(*request, response);
                    send_http();
                    delete request;
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
    to_send = response->to_buffer();
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
