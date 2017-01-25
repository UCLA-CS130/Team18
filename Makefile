echo_server:
	g++ -o echo_server server.cc session.cc main.cc -std=c++11 -Wall -Werror -lboost_system
