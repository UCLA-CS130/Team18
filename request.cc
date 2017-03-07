#include <stdlib.h>
#include "request.h"
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <iostream>
#include <map>

Request::Request(const std::string& request_string)
{
  original_string_ = request_string;
  valid_ = ParseRequestString(request_string);
}

bool Request::ParseRequestString(std::string request_string)
{
  std::istringstream resp(request_string);
  std::string header;
  std::string status;
  std::string::size_type index;
  std::string::size_type cr; // carriage reutn location
  std::getline(resp, status);
  bool good_status = DecodeStatus(status); 
  if (!good_status)
    return false;
  while (std::getline(resp, header) && header != "\r") {
    index = header.find(':', 0);
    cr = header.find('\r', index);
    if (index != std::string::npos) {
      headers_.push_back(std::make_pair(header.substr(0, index), header.substr(index+2, cr - (index + 2) )));
    }
  }
  body_ = request_string.substr(request_string.find("\r\n\r\n") + 4);
  return true; 
}

bool Request::DecodeStatus(std::string status_line)
{
  std::size_t front = 0;
  std::size_t back = 0;

  back = status_line.find(' ');
  if (back == std::string::npos)
    return false;
  method_ = status_line.substr(front, back - front);
  front = back + 1;
  
  back = status_line.find(' ', front);
  if (back == std::string::npos || back == front)
    return false;
  uri_ = status_line.substr(front, back - front);

  front = back + 1;

  back = status_line.find('\r', front);
  if (back == std::string::npos || back == front)
    return false;
  http_version_ = status_line.substr(front + 5, back - front-5);
  return true;
}

std::string Request::method() const { return method_; }
std::string Request::uri() const { return uri_; }
std::string Request::version() const { return http_version_; }
std::string Request::raw_request() const { return original_string_; }
std::vector<std::pair<std::string, std::string>> Request::headers() const { return headers_; }
std::string Request::body() const { return body_; }
std::string Request::ToString() const {
  std::string request_msg = method() + " " + uri() + " HTTP/" + version() + "\r\n";
  std::vector<std::pair<std::string,std::string>> localheaders = headers();
  for (std::vector<std::pair<std::string,std::string>>::const_iterator it = localheaders.begin(); it != localheaders.end(); ++it) {
    request_msg += (it->first + ": " + it->second + "\r\n");
  }
  request_msg += "\r\n";
  request_msg += body();
  return request_msg;
}
void Request::SetHeader(const std::string& header, const std::string& val) {
  for (unsigned int i = 0; i < headers_.size(); ++i) {
    if (headers_.at(i).first.compare(header)==0) {
      headers_.at(i) = std::make_pair(header,val);
      return;
    }
  }
  headers_.push_back(std::make_pair(header,val));
}

