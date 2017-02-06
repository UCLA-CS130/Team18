// session.h
// Adapted from Boost documentation code
// Source:
// http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp1/echo/async_tcp_echo_server.cpp
//
//

#ifndef SESSION
#define SESSION

#include <boost/asio.hpp>
#include <cstdlib>
#include <array>
#include <string>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket* socket)
        : socket_(std::move(*socket)) {}
    
    void start() { do_read();}
    bool check_input(std::size_t length, char* buffer);
    std::size_t prepare_response(int status, std::string body);
    std::string OutputToString();

private:
    void do_read();
    void do_write(std::size_t length);
    void send_http(std::size_t size);
    enum { max_length = 8192 };
    char data_[max_length];
    std::string to_send;
    tcp::socket socket_;
    std::string msg;
};



#endif
