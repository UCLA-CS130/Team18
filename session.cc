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
                   std::size_t len = msg.find("\r\n\r\n") + size_t(4);
                   strncpy(out_buf, msg.c_str(), len);
                   std::cout << "SENDING MSG: " << msg;
                   msg = "";
                   do_write(len);
                }
                else 
                    do_read();
            }
        });
      
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


