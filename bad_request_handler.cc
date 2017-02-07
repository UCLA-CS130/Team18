#include "bad_request_handler.h"

void BadRequestHandler::handle_request(Request* req, Response* rep) {
  rep->http_version = req->GetVersion();
  rep->status = Response::bad_request;
  rep->headers["Content-Length"] = "0";
}
