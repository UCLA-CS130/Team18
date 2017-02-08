import sys
import os
import subprocess
import httplib
import time

if __name__ == "__main__":
    print "Starting integration test..."
    #create test config listening on port 12345
    config_str ="server {\n  listen 12345;\n  Echo echo;\n  Static static static_src;\n}"    
    config_name = "temp_config"
    print "Creating temporary config file"
    with open(config_name, 'w') as f:
        f.write(config_str)

    #make clean and rebuild binary
    print "Building binary..."
    subprocess.call(["make", "clean"])
    subprocess.call(["make"])

    #open and run webserver with example config
    print "Running 'webserver' on port 12345"
    FNULL = open(os.devnull, 'w')
    proc1 = subprocess.Popen(["./webserver", config_name], stdout = FNULL, \
            stderr=subprocess.STDOUT)    
    while proc1.poll() == None:
        time.sleep(5)
        try:
            conn = httplib.HTTPConnection("localhost", 12345)
            print "Connected to webserver"
            '''Format of Request:
            GET /echo HTTP/1.1
            Host: localhost:12345
            User-Agent: arbitary-agent
            Content-Length: 0
            '''
            request = "GET /echo HTTP/1.1\r\nHost: localhost:12345\r\n" + \
                      "User-Agent: arbitrary-agent\r\n" + \
                      "Content-Length: 0\r\n\r\n"
            print "Sending:\n" + request.strip() + "\n"
            conn.putrequest("GET", "/echo", skip_accept_encoding=True)
            conn.putheader("User-Agent", "arbitrary-agent")
            conn.putheader("Content-Length", "0")
            conn.endheaders()
            conn.send("")
            response_body = conn.getresponse().read()
            print "Received:\n" + response_body.strip() + "\n"
            if request == response_body:
                matching = True
                print "Request matched response!"
            else:
                matching = False
                print "Request failed to match response"
            proc1.kill()
            print "Closing 'webserver'"
            FNULL.close() 
        except:
            time.sleep(5)
    #delete created config
    print "Deleting temporary config file"
    os.unlink(config_name)

    if matching:
        print "Integration test successful, exiting with code 0"
        sys.exit(0)
    else:
        print "Integration test unsuccessful, exiting with code 1"
        sys.exit(1)
