#!/bin/bash

case `uname` in
  Linux) g++ config_parser.cc main.cc -std=c++0x -g -Wall -o webserver;;
  Darwin) clang++ config_parser.cc main.cc -std=c++11 -g -Wall -stdlib=libc++ -o webserver;;
  *) echo "Unknown operating system";;
esac
