#include "gtest/gtest.h"
#include "gzip_handler.h"
#include "request.h"
#include "response.h"
#include <string>
#include <map>

class GzipHandlerTest : public ::testing::Test {
  protected:
    void HandleRequest(std::string uncompressed) {
      response_ = new Response();
      response_->SetBody(uncompressed);
      GzipHandler handler_;
      handler_.HandleRequest(*request_, response_);
    }

    void CleanUp() {
    	request_.release();
    	delete response_;
    }
    std::unique_ptr<Request> request_;
    Response* response_;
};

TEST_F(GzipHandlerTest, StatusRequest) {
  std::string uncompressed = "Test Body Uncompressed";
  HandleRequest(uncompressed);
  EXPECT_NE(uncompressed, response_->GetBody());
  
  CleanUp();
}

