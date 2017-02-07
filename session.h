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
#include "response.h"
#include "request_handler.h"
#include "config_options.h"

class Request;

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket* socket, config_options* options)
        : options_(options),
          socket_(std::move(*socket)),
          handler(nullptr),
          request(nullptr),
          response(new Response()) {}
    ~Session();
    void start() { do_read();}
    bool check_input(std::size_t length, char* buffer);
    std::size_t prepare_response(int status, std::string body);
    std::string OutputToString();

private:
    void do_read();
    void do_write();
    void send_http();
    enum { max_length = 8192 };
    char data_[max_length];
    config_options* options_;
    std::string to_send;
    tcp::socket socket_;
    std::string msg;
    request_handler* handler;
    Request* request;
    Response* response;
};



#endif
