#ifndef REQUEST_HANDLER
#define REQUEST_HANDLER

#include <map>
#include <memory>
#include <string>
#include "request.h"
#include "response.h"

class NginxConfig;

class RequestHandler {
  public:
  	enum Status {
      OK = 0,
      ERROR = 1
	  };
    static RequestHandler* CreateByName(const char* type);
    virtual ~RequestHandler() {}
    virtual Status Init(const std::string& uri_prefix,
			const NginxConfig& config) = 0;
    virtual Status HandleRequest(const Request& request,
				 Response* response) = 0;
};

extern std::map<std::string, RequestHandler* (*)(void)>* request_handler_builders;
template<typename T>
class RequestHandlerRegisterer {
 public:
  RequestHandlerRegisterer(const std::string& type) {
    if (request_handler_builders == nullptr) {
      request_handler_builders = new std::map<std::string, RequestHandler* (*)(void)>;
    }
    (*request_handler_builders)[type] = RequestHandlerRegisterer::Create;
  }
  static RequestHandler* Create() {
    return new T;
  }
};

#define REGISTER_REQUEST_HANDLER(ClassName) \
  static RequestHandlerRegisterer<ClassName> ClassName##__registerer(#ClassName)

#endif
