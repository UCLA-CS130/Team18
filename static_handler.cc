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

void StaticHandler::handle_request(Request* req, Response* rep)
{  
  std::string static_path = req->GetStaticPath();
  std::string file_path = req->GetFilePath();
  std::string request_uri = req->GetURI();
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
    SetOk(req, rep, body);
  }
  else
  {
    SetNotFound(req, rep);
  }

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
  else 
    ext = TXT;
}


