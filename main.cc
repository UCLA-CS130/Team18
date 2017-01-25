//  #include "config_parser.h"
#include "session.h"
#include "server.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <boost/asio.hpp>

int main(int argc, char* argv[]) {
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: echo_server <port>\n";
            return 1;
        }
        
        boost::asio::io_service io_service;
        Server s(io_service, std::atoi(argv[1]));
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
