#include "session.h"
#include "request.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <boost/bind.hpp>

Session::~Session() { delete request; }

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
                    request = new Request(msg, "echo", "static");
                    std::cout << request->GetMethod() << std::endl;
                    std::size_t response_size = prepare_response(200, msg);
                    send_http(response_size);
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

void Session::send_http(std::size_t size) {
    do_write(size);
}

void Session::do_write(std::size_t length)
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
