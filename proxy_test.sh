#!/bin/bash

# Build the binary
#if [ -f ./webserver ];
#then
#	make clean
#fi
#make

# Start the server
echo "port 2020;
server_name 127.0.0.1;

path /proxy ProxyHandler{
  host www.ucla.edu;
  port 80;
}
path /proxyredirect ProxyHandler{
  host ucla.edu;
  port 80;
}

default NotFoundHandler{}" > temp_config

./webserver temp_config &>/dev/null &

sleep 1

# Send request to server
curl -s www.ucla.edu:80 > temp_expected
curl -s localhost:2020/proxy/ > temp_response
curl -s ucla.edu:80 > temp_expected_redirect
curl -s localhost:2020/proxyredirect/ > temp_response_redirect

# Verify the response from the server works as expected
DIFF=$(diff temp_response temp_expected && diff temp_expected_redirect temp_response_redirect)
EXIT_STATUS=$?

# Error handling
if [ "$EXIT_STATUS" -eq 0 ]
then
    echo "SUCCESS: Integration test passed"
else
    echo "FAILED: Integration test failed"
    echo "diff:"
    echo $DIFF
fi

# Shutdown the webserver and cleanup
echo "Cleaning up and shutting down"
pkill webserver
make clean
rm temp_*

exit "$EXIT_STATUS"
