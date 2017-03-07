#include "response.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
namespace status_strings {
  const std::string ok = "200 OK\r\n";
  const std::string bad_request = "400 Bad Request\r\n";
  const std::string not_found = "404 Not Found\r\n";
}

std::string Response::ToString() {
  std::string response_msg = version+" "+ status;
  std::map<std::string, std::string>::iterator it;
  for (it = headers.begin(); it != headers.end(); it++) {
    response_msg += get_header(it->first);
  }
  response_msg += "\r\n";
  response_msg += response_body;

  return response_msg;
}

std::string Response::get_header(std::string header) {
  std::string formatted_header = "";
  std::string header_value = headers[header];

  if (header == "Content-Type") {
  	formatted_header = "Content-Type: " + header_value + "\r\n";
  } else if (header == "Content-Length") {
 	formatted_header = "Content-Length: " + header_value + "\r\n";
  } else {
        formatted_header = header + ": " + header_value + "\r\n";
  }

  return formatted_header;
}

std::string Response::to_string(Response::ResponseCode status) {
  switch (status) {
    case Response::ok:
      return status_strings::ok;
    case Response::bad_request:
      return status_strings::bad_request;
    case Response::not_found:
      return status_strings::not_found;
    default:
      return status_strings::bad_request;
  }
}

void Response::SetStatus(const Response::ResponseCode response_code) { 
  status = to_string(response_code); 
}
void Response::AddHeader(const std::string& header_name, 
                        const std::string& header_value) { 
  headers.insert(std::make_pair(header_name, header_value));
}
void Response::SetBody(const std::string& body) {
  response_body = body; 
} 

Response::Response(const std::string& response_string) {
  valid_ = ParseResponseString(response_string);
}
bool Response::ParseResponseString(const std::string& response_string) {
  std::istringstream resp(response_string);
  std::string header;
  std::string status;
  std::string::size_type index;
  std::string::size_type cr; // carriage reutn location
  std::getline(resp, status);
  bool good_status = DecodeStatus(status); 
  if (!good_status)
    return false;
  while (std::getline(resp, header) && header != "\r") {
    index = header.find_first_of(':', 0);
    cr = header.find('\r', index);
    if (index != std::string::npos) {
      AddHeader(header.substr(0, index), header.substr(index+2, cr - (index + 2) ));
      
    }
  }
  SetBody(response_string.substr(response_string.find("\r\n\r\n") + 4));
  return true;
}
bool Response::DecodeStatus(std::string status_line) {
  std::size_t front = 0;
  std::size_t back = 0;
  back = status_line.find(' ');
  if (back == std::string::npos)
    return false;
  version = (status_line.substr(front, back-front));
  front = back + 1;
  back = status_line.find('\r', front);
  if (back == std::string::npos || back == front)
    return false;
  status = (status_line.substr(front, back - front));
  return true;
}
