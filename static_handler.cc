#include "static_handler.h"
#include "request.h"
#include "response.h"
#include <string>
#include <fstream>
#include <sstream>

StaticHandler::StaticHandler()
{
  ext = NOEXT; 
}

RequestHandler::Status StaticHandler::Init(const std::string& uri_prefix,
                           const NginxConfig& config)
{
  return RequestHandler::Status::OK;
}

RequestHandler::Status StaticHandler::HandleRequest(const Request& request,
                                  Response* repsponse)
{  
  std::string static_path = request.GetStaticPath();
  std::string file_path = request.GetFilePath();
  std::string request_uri = request.GetURI();
  GetExtension(request_uri);
  
  std::string partial_file_path = request_uri.substr(static_path.size() + 2);
  
  std::string full_path = file_path + "/" + partial_file_path;


  std::ifstream f(full_path.c_str(),std::ios::in|std::ios::binary|std::ios::ate);
  if (f.is_open())
  {
    std::streampos size = f.tellg();
    char* membuff = new char[size];
    f.seekg (0, std::ios::beg);
    f.read(membuff, size);
    f.close();
    std::string body(membuff, size);
    delete membuff;
    SetOk(request, repsponse, body);
  }
  else
  {
    SetNotFound(request, repsponse);
  }
  return RequestHandler::Status::OK;
}

void StaticHandler::SetNotFound(Request* req, Response* res)
{
  res->http_version = req->GetVersion();
  res->status = Response::not_found;      
  res->headers.insert(std::pair<std::string,std::string>("Content-Type", "text/plain"));
  std::string body = "Couldn't find that file";
  std::string length;
  std::ostringstream temp;
  temp  <<  ((int) body.size());
  length = temp.str();


  res->headers.insert(std::pair<std::string,std::string>("Content-Length", length));
  res->body = body;
}
void StaticHandler::SetOk(Request* req, Response* res, std::string file_body)
{
  res->http_version = req->GetVersion();
  res->status = Response::ok;
  std::string content_type = GetContentType();      
  res->headers.insert(std::pair<std::string,std::string>("Content-Type",content_type));
  std::string length;
  std::ostringstream temp;
  temp  <<  ((int) file_body.size());
  length = temp.str();
  res->headers.insert(std::pair<std::string,std::string>("Content-Length", length));
  res->body = file_body;
}

std::string StaticHandler::GetContentType()
{
  if (ext == HTML)
    return "text/html";
  else if (ext == JPG)
    return "image/jpeg";
  else if (ext == GIF)
    return "image/gif";
  else if (ext == CSS)
    return "text/css";
  else if (ext == JS)
    return "application/javascript";
  else if (ext == JSON)
    return "application/json";
  else
    return "text/plain";
}

void StaticHandler::GetExtension(std::string uri)
{
  std::size_t dot = uri.find_last_of(".");
  std::string extension_string = uri.substr(dot + 1);

  if (!extension_string.compare("html"))
    ext = HTML;
  else if (!extension_string.compare("jpg") || 
              !extension_string.compare("jpeg"))
    ext = JPG;
  else if (!extension_string.compare("css"))
    ext = CSS;
  else if (!extension_string.compare("js"))
    ext = JS;
  else if (!extension_string.compare("gif"))
    ext = GIF;
  else if (!extension_string.compare("json"))
    ext = JSON;
  else 
    ext = TXT;
}


