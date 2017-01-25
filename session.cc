#include "session.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <boost/bind.hpp>


void Session::do_read()
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            std::string received(data_, data_ + length);
            std::cout << "RCVD: " << received;
            msg += received;
            std::cout << "MSG: " << msg;
            if (!ec) 
            {   if (msg.find("\r\n\r\n") != std::string::npos)
                {
                    send_http(200);
                }
                else 
                    do_read();
            }
        });
      
}

void Session::send_http(int code) {
    std::string status_line = "HTTP/1.1 ";
    switch(code)
    {
        case 200: 
            status_line += "200 OK\r\n";      
            break;
        default:
            status_line += "400 Bad Request\r\n";
    }
    std::string content_type = "Content-Type: text/plain\r\n";
    std::string header = status_line + content_type + "\r\n";
    std::size_t len = header.size() + msg.find("\r\n\r\n") + size_t(4);
    msg = header + msg;
    strncpy(out_buf, msg.c_str(), len);
    std::cout << "SENDING MSG: " << msg;
    msg = "";
    do_write(len);
}

void Session::do_write(std::size_t length)
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(out_buf, length),
        [this, self](boost::system::error_code ec, std::size_t)
        {
            if (!ec)
            {
                do_read();
            }
        });
}


