// server.h
// Adapted from Boost documentation code
// Source:
// http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp1/echo/async_tcp_echo_server.cpp
//
//

#include "server.h"
#include "session.h"

Server::Server(short port)
        : io_service_(),
          acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)),
          socket_(io_service_),
          port_num(port)
{}

void Server::start()
{
  do_accept();
}

void Server::run()
{
  io_service_.run();
}

void Server::do_accept()
{
  acceptor_.async_accept(socket_, 
    [this](boost::system::error_code ec)
    {
      if (!ec) {
        std::make_shared<Session>(std::move(&socket_))->start();
      }

      do_accept();
    });
}
