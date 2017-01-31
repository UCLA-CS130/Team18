webserver:
	g++ -o webserver server.cc session.cc config_parser.cc main.cc -std=c++11 -Wall -Werror -lboost_system

parser-unit-tests:
	./build_tests.sh && ./config_parser_test && gcov -r config_parser.cc

clean:
	rm webserver