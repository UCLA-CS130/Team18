// server.h
// Adapted from Boost documentation code
// Source:
// http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp1/echo/async_tcp_echo_server.cpp
//
//

#include "server.h"
#include "session.h"

using boost::asio::ip::tcp;


void Server::do_accept()
{
    acceptor_.async_accept(socket_, 
        [this](boost::system::error_code ec)
        {
            if (!ec)
            {
                std::make_shared<Session>(std::move(socket_))->start();
            }
 
            do_accept();
        });
}
