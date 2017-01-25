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
using boost::asio::ip::tcp;


class Server
{
public:
    Server(boost::asio::io_service& io_service, short port) 
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)), 
          socket_(io_service)
    {do_accept();}
private:
    void do_accept();
    tcp::acceptor acceptor_;
    tcp::socket socket_;
};



#endif
