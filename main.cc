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
    config_options* options = new config_options();
    config.GetConfigOptions(options);
    if (options->port) {
      Server s(&config, options);
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
