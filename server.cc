// server.h
// Adapted from Boost documentation code
// Source:
// http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp1/echo/async_tcp_echo_server.cpp
//
//

#include "server.h"
#include "session.h"

#define DEFAULT_PORT 8080

Server::Server(config_options* options)
        : io_service_(),
          options_(options),
          acceptor_(io_service_, tcp::endpoint(tcp::v4(), options->port)),
          socket_(io_service_)
{
  int port = options->port;
  if ((0 < port) && (port < 65535))
    port_num = port;
  else 
    port_num = DEFAULT_PORT;
}

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
        std::make_shared<Session>(std::move(&socket_), options_)->start();
      }

      do_accept();
    });
}
