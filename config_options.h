#ifndef CONFIG_OPTIONS
#define CONFIG_OPTIONS

#include <string>
#include <map>

struct config_options {
  int port;
  std::string echo_path;
  std::map<std::string, std::string> static_paths;
};

#endif