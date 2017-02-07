webserver:
	g++ -o webserver server.cc session.cc request.cc response.cc echo_handler.cc bad_request_handler.cc config_parser.cc static_handler.cc main.cc -std=c++11 -Wall -Werror -lboost_system

test:
	./build_tests.sh

integration:
	python integration_test.py

clean:
	rm webserver
