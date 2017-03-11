webserver:
	g++ -g -o webserver gzip.cpp zlib.cpp server.cc session.cc request.cc response.cc echo_handler.cc bad_request_handler.cc not_found_handler.cc partial_handler.cc config_parser.cc static_handler.cc main.cc request_handler.cc request_handler_stats.cc status_handler.cc -std=c++11 -Wall -Werror -static-libgcc -static-libstdc++ -pthread -Wl,-Bstatic -lboost_system -lboost_thread -L/opt/lib -lboost_iostreams -lz

test:
	./build_tests.sh

deploy:
	docker build -t httpserver.build .
	docker run httpserver.build > binary.tar
	mkdir deploy
	cp complex_config ./deploy
	cp -R static_src ./deploy
	cp Dockerfile.run deploy/Dockerfile
	tar -xvf binary.tar -C deploy
	docker build -t httpserver deploy

threading-test: clean webserver
	python threading_test.py

integration:
	python integration_test.py

clean:
	rm webserver
