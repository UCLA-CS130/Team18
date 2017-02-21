// server.h
// Adapted from Boost documentation code
// Source:
// http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp1/echo/async_tcp_echo_server.cpp
//
//

#ifndef SERVER
#define SERVER

#include <cstdlib>
#include <boost/asio.hpp>
#include "config_options.h"
#include "boost/thread.hpp"
#include <vector>
#include <boost/shared_ptr.hpp>
#include "session.h"

using boost::asio::ip::tcp;

class NginxConfig;

class Server {
public:
  Server(NginxConfig* config, config_options* options);
  void start();
  void run();
  short get_port() { return port_num; }
private:
  boost::asio::io_service io_service_;
  NginxConfig* config_;
  config_options* options_;
  void do_accept();
  void handle_stop();
  //void spawn_thread(tcp::socket* sock, NginxConfig* config);
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  short port_num;
  boost::shared_ptr<Session> new_connection_;
//  boost::asio::signal_set signals_;
};

#endif
