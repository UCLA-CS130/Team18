# Team 18 Assignment 9 Features

This page describes our two features we implemented.

## Markdown

Markdown is markup language with plaintext formatting syntax which allows the files to be converted to HTML and displayed in a browser. It is often used for README files that are including with the source code for software. Markdown files use the file extension ".md".

---
### Basic formatting examples:

\#\#\#\# example bigger header

\#\#\#\#\#\# example smaller header

The above examples render as an h4 HTML header and an h6 HTML header respectively(additional hashes correspond to smaller headers)

#### example bigger header
###### example smaller header

---

#### Markdown file handling

When you access a file with a .md extension, our request handler transparently converts it to HTML using the cpp-markdown library at https://github.com/sevenjay/cpp-markdown by Github user sevenjay. After retrieving a file, our server will check if it has the .md extension. If that is the case, the cpp-markdown library is used to attempt to render it into HTML. If it is successful, it is returned as rendered. Otherwise, we default to returning a 404 File Not Found response (this decision will be discussed later on)

In fact, this page itself is a .md file located at ec2-35-166-129-63.us-west-2.compute.amazonaws.com:8080/static/assignment_9.md

#### Design decision process

The design of this functionality was fairly straightforward, so there were not too many decision points to be made. 

First decision to be made was choice of Markdown rendering library. Fortunately, there were choices provided so we only had to pick from a handfull. One choice was the peg-markdown library. We decided against this because it depends on a third party GLib library, which would mean excessive additions for what would likely not be a giant change. Also the C API would stylistically be somewhat jarring with the rest of our C++ code. Another option was Discount, which was described as "Almost impossible to maintain," which we decided could be a hassle moving forward. Cpp-Markdown has little code dependencies, small files, and would fit easily into our code so that's how we went.

An additional decision was what to do if there was some error in the rendering of the HTML file (i.e. the function that converted the file returned indicated the read has failed). Our first idea was to return the .md file as is, and just let them view the original file. In the end, we realized that our server should be consistent, so .md files should be rendered as expected or not returned at all. In addition, we could not be sure of the possible issues that would arise in the attempt to render the file. For these reasons, we chose to return a 404 response if the file could not be rendered for some reason.

## HTTP gzip compression

When our webserver receives a request with header "Accept-Encoding: gzip, deflate", we will generate a response with the appropriate RequestHandler, and then call our GzipHandler's HandleRequest method. The GzipHandler will extract the response's body, compress it, update the response's Content-Length, and add the "Content-Encoding: gzip" header.

For example, using Firefox's network browser tools to track responses returned from our webserver, one can see the "Content-Encoding: gzip" header included when a request is sent for ec2-35-166-129-63.us-west-2.compute.amazonaws.com:8080/static/PMT.gif

We used boost's GzipFilters (http://www.boost.org/doc/libs/1_39_0/libs/iostreams/doc/classes/gzip.html). Furthermore, we used gzip.cpp (http://www.boost.org/doc/libs/1_62_0/libs/iostreams/src/gzip.cpp) and zlib.cpp (http://www.boost.org/doc/libs/1_62_0/libs/iostreams/src/zlib.cpp), both created by Jonathan Turkanis.

#### HTTP compression design process

HTTP compression had a bit more difficult design decisions than Markdown rendering.

First, we needed to once again pick a library for use in our program. This was a fairly easy choice, as boost provides functionality for Gzip, and these boost functions could be plugged in to the code fairly easy once certain linker dependencies and header file concerns were resolved. We looked at other library options briefly, but this was the clear cut most simple and efficient choice for us.

Additionally, we needed to decide how compression should be handled (i.e. where in the code should compression occur & what object handles the compression). We initially placed the compression code in the same handler that handles serving static files. We quickly realized that this was not a sustainable decision, as compression may need to be utilized for other types of requests and because we may want to turn off compression for static files depending on various factors. Our decision then was to create a handler specifically for Gzip compression, that would work on a response created by another handler. The only API change needed was to add a method to read the body of a Response object, which is not going to cause any issue in working with the code in the future. This allows for configuration of the server as well as the ability to compress our responses for different types of requests. The flexibility for future design was key in making this decision.

Another decision was whether the compression handler should act as one of the handlers that is linked to a URL path like the ones that we have previously built. We decided against that and stuck to this decision because we didn't see any use for allowing the user to access the handler from a browser (since we would prefer to choose if compression occurs) and because there really is no use for that except for very specific functionality on the user's end. In the end, we just created the handler as a member variable for the Session objects that handle requests, allowing its use for compressing the results of any other handler.