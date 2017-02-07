#include "echo_handler.h"

void EchoHandler::handle_request(Request* req, Response* rep) {
  rep->http_version = req->GetVersion();
  rep->status = Response::ok;
  rep->headers["Content-Type"] = "text/plain";
  std::size_t body_length = req->GetOriginalString().find("\r\n\r\n") + size_t(4);
  rep->headers["Content-Length"] = std::to_string((int) body_length);
  rep->body = req->GetOriginalString();
}
