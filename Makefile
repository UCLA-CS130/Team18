echo_server:
	g++ -o webserver server.cc session.cc config_parser.cc main.cc -std=c++11 -Wall -Werror -lboost_system
