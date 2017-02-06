#include <stdlib.h>
#include "request.h"
#include <string>
#include <sstream>
#include <map>

bool Request::ParseRequestString(std::string request_string)
{
  std::istringstream resp(request_string);
  std::string header;
  std::string status;
  std::string::size_type index;

  std::getline(resp, status);
  bool good_status = DecodeStatus(status); 
  if (!good_status)
    return false;
  while (std::getline(resp, header) && header != "\r") {
    index = header.find(':', 0);
    if (index != std::string::npos) {
      headers.insert(std::make_pair(header.substr(0, index), header.substr(index+2)));
    }
  }
  return true; 
}

bool Request::DecodeStatus(std::string status_line)
{
  std::size_t front = 0;
  std::size_t back = 0;

  back = status_line.find(' ');
  if (back == std::string::npos)
    return false;
  method = status_line.substr(front, back - front);
  front = back + 1;
  
  back = status_line.find(' ', front);
  if (back == std::string::npos)
    return false;
  uri = status_line.substr(front, back - front);
  GetRequestType(uri);
  front = back + 1;

  back = status_line.find('\r', front);
  if (back == std::string::npos)
    return false;
  http_version = status_line.substr(front, back - front);
  return true;
}

std::string Request::GetHeader(std::string header_name)
{
  std::map<std::string, std::string>::iterator it;
  it = headers.find(header_name);
  if (it == headers.end())
    return "";
  else
    return it->second;  
}

std::string Request::GetType()
{
  switch(message_type)
  {
    case NONE:
      return "None";
      break;
    case ECHO:
      return "Echo";
      break;
    case STAT:
      return "Static";
      break;
    default:
      return "None";
  }
}

bool Request::GetRequestType(std::string uri)
{
  if (!uri.substr(1,4).compare("echo"))
    message_type = ECHO;
  else if (!uri.substr(1,6).compare("static"))
    message_type = STAT;
  else
    message_type = NONE;
}

