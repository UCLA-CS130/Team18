#include "response.h"
#include <stdio.h>
#include <string.h>

std::size_t response::to_buffer(char* response_buf) {
  std::string response_msg;
  response_msg += get_header("Status-Line");
  response_msg += get_header("Content-Type");
  response_msg += get_header("Content-Length");
  response_msg += "\r\n";
  response_msg += body;

  strncpy(response_buf, response_msg.c_str(), length);
  return length;
}

std::string response::get_header(std::string header) {
  std::string formatted_header = "";
  std::string header_value = headers[header];

  if (header == "Status-Line") {
  	formatted_header = header_value + "\r\n"; 
  } else if (header == "Content-Type") {
  	formatted_header = "Content-Type: " + header_value + "\r\n";
  } else if (header == "Content-Length") {
 	formatted_header = "Content-Length: " + header_value + "\r\n";
  }

  return formatted_header;
}
