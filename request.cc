#include <stdlib.h>
#include "request.h"
#include <string>
#include <sstream>
#include <map>

Request::Request(std::string request_string, std::string echo, 
                 std::map<std::string,std::string> stat)
: static_map(stat)
{
  echo_string = echo;
  message_type = NONE;
  original_string = request_string;
  valid = ParseRequestString(request_string);
}

bool Request::ParseRequestString(std::string request_string)
{
  std::istringstream resp(request_string);
  std::string header;
  std::string status;
  std::string::size_type index;
  std::string::size_type cr;
  std::getline(resp, status);
  bool good_status = DecodeStatus(status); 
  if (!good_status)
    return false;
  while (std::getline(resp, header) && header != "\r") {
    index = header.find(':', 0);
    cr = header.find('\r', index);
    if (index != std::string::npos) {
      headers.insert(std::make_pair(header.substr(0, index), header.substr(index+2, cr - (index + 2) )));
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
  if (back == std::string::npos || back == front)
    return false;
  uri = status_line.substr(front, back - front);
  GetRequestType(uri);
  front = back + 1;

  back = status_line.find('\r', front);
  if (back == std::string::npos || back == front)
    return false;
  http_version = status_line.substr(front + 5, back - front-5);
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
    case ECHO_MODE:
      return "Echo";
      break;
    case STAT_MODE:
      return "Static";
      break;
    default:
      return "None";
  }
}

std::string Request::GetOriginalString() {
  return original_string;
}

bool Request::GetRequestType(std::string uri)
{
  if (!uri.substr(1,echo_string.size()).compare(echo_string))
    message_type = ECHO_MODE;
  else {
    for (std::map<std::string,std::string>::iterator it = static_map.begin();
            it != static_map.end(); it++)
    {
       std::size_t slash = uri.find("/", 1);
       if (!uri.substr(1, slash-1).compare(it->first))
       {
          message_type = STAT_MODE;
          static_path = it->first;
          file_path = it->second;
          return true;
       }
    }
    message_type = NONE;
    return false;
  }
  return true;
}

bool Request::IsValid() { return valid; }
std::string Request::GetMethod() { return method; }
std::string Request::GetURI() { return uri; }
std::string Request::GetVersion() { return http_version; }
std::map<std::string,std::string> Request::GetHeaders() { return headers; }
std::string Request::GetStaticPath() { return static_path; }
std::string Request::GetFilePath() { return file_path; }
