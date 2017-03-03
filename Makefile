webserver:
	g++ -g -o webserver server.cc session.cc request.cc response.cc echo_handler.cc bad_request_handler.cc not_found_handler.cc partial_handler.cc config_parser.cc static_handler.cc main.cc request_handler.cc request_handler_stats.cc status_handler.cc proxy_handler.cc -std=c++11 -Wall -Werror -lboost_system -lpthread

test:
	./build_tests.sh

threading-test: clean webserver
	python threading_test.py

integration:
	python integration_test.py

clean:
	rm webserver
