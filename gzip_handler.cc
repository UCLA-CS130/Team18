#include "gzip_handler.h"
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

RequestHandler::Status GzipHandler::Init(const std::string& uri_prefix,
                                         const NginxConfig& config)
{
  return RequestHandler::Status::OK;
}

RequestHandler::Status GzipHandler::HandleRequest(const Request& request,
                                                  Response* response)
{
  response->AddHeader("Content-Encoding", "gzip");
  std::string length;
  std::ostringstream temp; 
  std::stringstream compressed;
  std::stringstream data;
  boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
  in.push(boost::iostreams::gzip_compressor());
  data << response->GetBody();
  in.push(data);
  boost::iostreams::copy(in, compressed);
  
  std::string body_gzip = compressed.str();
  temp  <<  ((int) body_gzip.size());
  length = temp.str();

  response->SetOrAddHeader("Content-Length", length);
  response->SetBody(body_gzip);
  return RequestHandler::Status::OK;
}
