#include "config_parser.h"
#include "session.h"
#include "server.h"
#include <stdlib.h>
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {
  try
  {
    if (argc != 2) {
      std::cerr << "Usage: ./webserver <config_file_path>\n";
      return 1;
    }
    NginxConfigParser config_parser;
    NginxConfig config;
    config_parser.Parse(argv[1], &config);
    int port = config.getPort();
    if (port) {
      Server s(port);
      s.start();
      s.run();
    } else {
      std::cerr << "Invalid port number " << "\n";
      return 0;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
