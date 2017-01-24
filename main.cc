#include "config_parser.h"
#include <stdlib.h>
using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: ./webserver <path to config file>\n");
    return 1;
  }

  NginxConfigParser config_parser;
  NginxConfig config;
  config_parser.Parse(argv[1], &config);

  int config_size = (unsigned long) config.statements_[0]->child_block_->statements_.size();
  for (int i = 0; i < config_size; i++) {
  	if (config.statements_[0]->child_block_->statements_[i]->tokens_[0] == "listen") {
  		cout << stoi(config.statements_[0]->child_block_->statements_[i]->tokens_[1]) << endl;
  	}
  }
  return 0;
}
