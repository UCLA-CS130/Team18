#include "gtest/gtest.h"
#include "request_handler_stats.h"
#include "request_handler.h"
#include <string>

class RequestHandlerStatsTest : public ::testing::Test {
  protected:
    std::string GetDefaultHandler(std::string handler) {
      request_handler_stats_ = RequestHandlerStats::getInstance();
      request_handler_stats_->SetDefaultHandler(handler);
      return request_handler_stats_->GetDefaultHandler();
    }

    std::pair<std::string, std::string> GetHandlers(std::string handler, std::string uri) {
      request_handler_stats_ = RequestHandlerStats::getInstance();
      request_handler_stats_->InsertHandler(handler, uri);
      return request_handler_stats_->GetHandlers().front();
    }

    std::pair<std::string, RequestHandler::Status> GetRequests(std::string url, RequestHandler::Status response) {
      request_handler_stats_ = RequestHandlerStats::getInstance();
      request_handler_stats_->InsertRequest(url, response);
      return request_handler_stats_->GetRequests().front();
    }

    RequestHandlerStats* request_handler_stats_;
};

TEST_F(RequestHandlerStatsTest, DefaultHandler) {
  std::string default_handler = "DefaultHandler";
  EXPECT_EQ(default_handler, GetDefaultHandler(default_handler)) << "Expected different Default Handler";
}

TEST_F(RequestHandlerStatsTest, GetHandlers) {
  std::string handler = "Handler";
  std::string uri = "Uri";
  EXPECT_EQ(std::make_pair(handler, uri), GetHandlers(handler, uri)) << "Expected different Handler";
}

TEST_F(RequestHandlerStatsTest, GetRequests) {
  std::string url = "url";
  RequestHandler::Status status = RequestHandler::Status::OK;
  EXPECT_EQ(std::make_pair(url, status), GetRequests(url, status)) << "Expected different Handler";
}
