// session.h
// Adapted from Boost documentation code
// Source:
// http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp1/echo/async_tcp_echo_server.cpp
//
//

#ifndef SESSION
#define SESSION

#include <array>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <cstdlib>
#include <memory>
#include <string>
#include "request_handler.h"
#include "response.h"

using boost::asio::ip::tcp;

class NginxConfig;

class Session : public std::enable_shared_from_this<Session>
{
public:
    explicit Session(boost::asio::io_service& io_service, NginxConfig* config);
    ~Session();
    void start() { do_read();}
    bool check_input(std::size_t length, char* buffer);
    std::size_t prepare_response(int status, std::string body);
    std::string OutputToString();
    tcp::socket& Socket() { return socket_; }
private:
    void init_handlers(NginxConfig* config);
    void do_read();
    void do_write();
    void send_http();
    enum { max_length = 8192 };
    char data_[max_length];
    std::string to_send;
    tcp::socket socket_;
    std::string msg;
    std::map<std::string, RequestHandler*> handlers_;
    RequestHandler* default_handler_;
    std::unique_ptr<Request> request;
    Response* response;
};

#endif
