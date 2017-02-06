#include "echo_handler.h"

void echo_handler::handle_request(Request& req, Response& rep) {
  rep.http_version = req.GetVersion();
  rep.status = Response::ok;
  rep.headers = req.GetHeaders();
  rep.body = req.GetOriginalString();
}
