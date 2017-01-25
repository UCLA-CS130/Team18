#include "config_parser.h"
#include <stdlib.h>
#include "session.h"
#include "server.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <boost/asio.hpp>
using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: ./webserver <path to config file>\n");
    return 1;
  }

  NginxConfigParser config_parser;
  NginxConfig config;
  config_parser.Parse(argv[1], &config);
  int portno;
  int config_size = (unsigned long) config.statements_[0]->child_block_->statements_.size();
  for (int i = 0; i < config_size; i++) {
  	shared_ptr<NginxConfigStatement> config_statement= config.statements_[0]->child_block_->statements_[i];
  	if (config_statement->tokens_[0] == "listen") {
  		portno = stoi(config_statement->tokens_[1]);
  	}
  }
  try
  {      
        boost::asio::io_service io_service;
        Server s(io_service, atoi(argv[1]));
        io_service.run();
  }
  catch (std::exception& e)
  {
        cerr << "Exception: " << e.what() << "\n";
  }
    return 0;
}
