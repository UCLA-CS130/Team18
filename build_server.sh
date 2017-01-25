#!/bin/bash

case `uname` in
  Linux) g++ server.cc session.cc config_parser.cc main.cc -std=c++0x -g -Wall -Werror -lboost_system -o webserver;;
  Darwin) clang++ config_parser.cc main.cc -std=c++11 -g -Wall -stdlib=libc++ -o webserver;;
  *) echo "Unknown operating system";;
esac
