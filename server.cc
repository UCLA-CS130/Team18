// server.h
// Adapted from Boost documentation code
// Source:
// http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp1/echo/async_tcp_echo_server.cpp
//
//

#include "server.h"
#include "session.h"
#include "boost/thread.hpp"
#include <vector>
#include "boost/asio.hpp"
#include <memory>

using boost::asio::ip::tcp;

#define DEFAULT_PORT 8080

Server::Server(NginxConfig* config, config_options* options)
        : io_service_(),
          config_(config),
          acceptor_(io_service_, tcp::endpoint(tcp::v4(), options->port)),
          socket_(io_service_),
          new_connection_()
{
  int port = options->port;
  if ((0 < port) && (port < 65535))
    port_num = port;
  else 
    port_num = DEFAULT_PORT;

  acceptor_.set_option(tcp::acceptor::reuse_address(true));
  //acceptor_.listen();
  do_accept();
}

void Server::start()
{
  do_accept();
}

void Server::run()
{
  //io_service_.run();
  std::vector<boost::shared_ptr<boost::thread> > threads;
  for (std::size_t i = 0; i < 10; ++i)
  {
    boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
    threads.push_back(thread);
  }

  for (std::size_t i = 0; i < threads.size(); ++i)
  {
    threads[i]->join();
  }

}


void Server::do_accept()
{
  //new_connection_.reset(new Session(io_service_, config_));
  std::shared_ptr<Session> sess= 
     std::make_shared<Session>(io_service_, config_);
  acceptor_.async_accept(sess->Socket(), 
    [this, sess](boost::system::error_code ec)
    {
      if (!ec) {        
        sess->start();
      }

      do_accept();
    });
}

void Server::handle_stop()
{
  io_service_.stop();
}
