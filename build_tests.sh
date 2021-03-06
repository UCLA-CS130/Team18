#!/bin/bash

GTEST_DIR=googletest/googletest
case `uname` in
  Linux)
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -fprofile-arcs -ftest-coverage -c ${GTEST_DIR}/src/gtest-all.cc
    ar -rv libgtest.a gtest-all.o
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread -fprofile-arcs -ftest-coverage server_test.cc gzip.cpp zlib.cpp gzip_handler.cc config_parser.cc request_handler.cc request.cc response.cc echo_handler.cc static_handler.cc bad_request_handler.cc session.cc server.cc request_handler_stats.cc markdown.cpp markdown-tokens.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o server_test -lboost_system -lboost_regex -lboost_thread -L/opt/lib -lboost_iostreams -lz
    ./server_test && gcov -r server.cc
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread -fprofile-arcs -ftest-coverage session_test.cc gzip.cpp zlib.cpp gzip_handler.cc request_handler.cc request.cc response.cc echo_handler.cc static_handler.cc bad_request_handler.cc session.cc request_handler_stats.cc markdown.cpp markdown-tokens.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o session_test -lboost_system -lboost_regex -L/opt/lib -lboost_iostreams -lz
    ./session_test && gcov -r session.cc
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread -fprofile-arcs -ftest-coverage response_test.cc response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o response_test
    ./response_test && gcov -r response.cc
   g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread -fprofile-arcs -ftest-coverage request_test.cc request.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_test
    ./request_test && gcov -r request.cc 
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread -fprofile-arcs -ftest-coverage echo_handler_test.cc request_handler.cc echo_handler.cc response.cc request.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o echo_handler_test
    ./echo_handler_test && gcov -r echo_handler.cc
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread  -fprofile-arcs -ftest-coverage static_handler_test.cc not_found_handler.cc request_handler.cc static_handler.cc response.cc request.cc config_parser.cc markdown.cpp markdown-tokens.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o static_handler_test -lboost_system -lboost_regex
    ./static_handler_test && gcov -r static_handler.cc 
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread -fprofile-arcs -ftest-coverage bad_request_handler_test.cc request_handler.cc bad_request_handler.cc response.cc request.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o bad_request_handler_test
    ./bad_request_handler_test && gcov -r bad_request_handler.cc
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread -fprofile-arcs -ftest-coverage not_found_handler_test.cc request_handler.cc not_found_handler.cc response.cc request.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o not_found_handler_test
    ./not_found_handler_test && gcov -r not_found_handler.cc
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread -fprofile-arcs -ftest-coverage status_handler_test.cc request_handler.cc request_handler_stats.cc status_handler.cc response.cc request.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o status_handler_test
    ./status_handler_test && gcov -r status_handler.cc
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread -fprofile-arcs -ftest-coverage request_handler_stats_test.cc request_handler.cc request_handler_stats.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_handler_stats_test
    ./request_handler_stats_test && gcov -r request_handler_stats.cc
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread -fprofile-arcs -ftest-coverage gzip_handler_test.cc gzip.cpp zlib.cpp gzip_handler.cc request_handler.cc request.cc response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o gzip_handler_test -lboost_system -L/opt/lib -lboost_iostreams -lz
    ./gzip_handler_test && gcov -r gzip_handler.cc
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread -fprofile-arcs -ftest-coverage config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test
    ./config_parser_test && gcov -r config_parser.cc;;
  Darwin)
    clang++ -std=c++11 -stdlib=libc++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -fprofile-arcs -ftest-coverage -c ${GTEST_DIR}/src/gtest-all.cc
    ar -rv libgtest.a gtest-all.o
    clang++ -std=c++11 -stdlib=libc++ -isystem ${GTEST_DIR}/include -pthread -fprofile-arcs -ftest-coverage config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test ;;
  *)
    echo "Unknown operating system." ;;
esac
