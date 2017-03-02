import sys
import telnetlib
import time
import os
import subprocess
#create test config listening on port 12345
config_str ="port 12345;\n\npath /echo EchoHandler {}\n\npath /static StaticHandler {\n  root static_src;\n}\n\ndefault NotFoundHandler {}" 
config_name = "temp_config"
print "Creating temporary config file"
with open(config_name, 'w') as f:
        f.write(config_str)

HOST = "localhost"
PORT = 12345
    
#open and run webserver with example config
print "Running 'webserver' on port 12345"
FNULL = open(os.devnull, 'w')
proc1 = subprocess.Popen(["./webserver", config_name], stdout = FNULL, \
            stderr=subprocess.STDOUT)    

time.sleep(5) 
tn = telnetlib.Telnet(HOST, PORT)
tn.write("GET /echo HTTP/1.1\r\n")
print "Sending Status line on connection 1\n"
tn.write("host: localhost\r\n")
print "Sending host line on connection 1\n"

tn2 = telnetlib.Telnet(HOST, PORT)
tn2.write("GET /echo HTTP/1.1\r\n")
print "Sending Status line on connection 2\n"
tn2.write("host: localhost\r\n")
print "Sending host line on connection 2\n"
tn2.write("three: four\r\n")
print "Sending useless header on connection 2\n"
tn2.write("\r\n")
print "Sending newlines on connection 2\n"
first_echo = tn2.read_until("three: four\r\n")
print first_echo

tn.write("one: two\r\n")
print "Sending useles header on connection 1\n"
tn.write("\r\n")
print "Sending newlines on connection 1\n"
second_echo = tn.read_until("one: two\r\n")
print second_echo

proc1.kill()
print "Closing 'webserver'"
FNULL.close() 
print "Deleting temporary config file"
os.unlink(config_name)

if "three: four" in first_echo and "one: two" in second_echo:
    print "Successful"
    exit(0)
else:
    print "Unsuccessful"
    exit(1)

