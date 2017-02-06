#include "response.h"
#include <stdio.h>
#include <string.h>

namespace status_strings {
  const std::string ok = "200 OK\r\n";
  const std::string bad_request = "400 Bad Request\r\n";
  const std::string not_found = "404 Not Found\r\n";
}

std::string Response::to_buffer() {
  std::string response_msg = "HTTP/" + http_version + " " + to_string(status);

  std::map<std::string, std::string>::iterator it;
  for (it = headers.begin(); it != headers.end(); it++) {
    response_msg += get_header(it->first);
  }
  response_msg += "\r\n";
  response_msg += body;

  return response_msg;
}

std::string Response::get_header(std::string header) {
  std::string formatted_header = "";
  std::string header_value = headers[header];

  if (header == "Content-Type") {
  	formatted_header = "Content-Type: " + header_value + "\r\n";
  } else if (header == "Content-Length") {
 	formatted_header = "Content-Length: " + header_value + "\r\n";
  }

  return formatted_header;
}

std::string Response::to_string(Response::status_type status) {
  switch (status) {
    case Response::ok:
      return status_strings::ok;
    case Response::bad_request:
      return status_strings::bad_request;
    case Response::not_found:
      return status_strings::not_found;
  }
}
