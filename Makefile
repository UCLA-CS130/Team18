webserver:
	g++ -o webserver server.cc session.cc request.cc config_parser.cc main.cc -std=c++11 -Wall -Werror -lboost_system

test:
	./build_tests.sh && ./config_parser_test && gcov -r config_parser.cc && ./server_test && gcov -r server.cc && ./session_test && gcov -r session.cc

integration:
	python integration_test.py

clean:
	rm webserver
