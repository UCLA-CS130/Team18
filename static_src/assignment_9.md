# Team 18 Assignment 9 Features

This page describes our two features we implemented.

## Markdown

When you access a file with a .md extension, our request handler transparently converts it to HTML using the cpp-markdown library at https://github.com/sevenjay/cpp-markdown by Github user sevenjay.

In fact, this page itself is a .md file located at ec2-35-166-129-63.us-west-2.compute.amazonaws.com:8080/static/assignment_9.md

## HTTP gzip compression

When our webserver receives a request with header "Accept-Encoding: gzip, deflate", we will generate a response with the appropriate RequestHandler, and then call our GzipHandler's HandleRequest method. The GzipHandler will extract the response's body, compress it, update the response's Content-Length, and add the "Content-Encoding: gzip" header.

For example, using Firefox's network browser tools to track responses returned from our webserver, one can see the "Content-Encoding: gzip" header included when a request is sent for ec2-35-166-129-63.us-west-2.compute.amazonaws.com:8080/static/PMT.gif

We used boost's GzipFilters (http://www.boost.org/doc/libs/1_39_0/libs/iostreams/doc/classes/gzip.html). Furthermore, we used gzip.cpp (http://www.boost.org/doc/libs/1_62_0/libs/iostreams/src/gzip.cpp) and zlib.cpp (http://www.boost.org/doc/libs/1_62_0/libs/iostreams/src/zlib.cpp), both created by Jonathan Turkanis.