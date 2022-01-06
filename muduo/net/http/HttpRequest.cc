// created by cycloidzzz

#include "muduo/net/http/HttpRequest.h"
#include "muduo/base/Logging.h"
#include "muduo/net/Buffer.h"

#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

void HttpRequest::appendToBuffer(Buffer *output) const {
  // snprintf(buf, sizeof buf, "HTTP/1.1 %d ", statusCode_);
  // output->append(buf);
  // output->append(statusMessage_);
  // output->append("\r\n");

  // if (closeConnection_) {
  //   output->append("Connection: close\r\n");
  // } else {
  //   snprintf(buf, sizeof buf, "Content-Length: %zd\r\n", body_.size());
  //   output->append(buf);
  //   output->append("Connection: Keep-Alive\r\n");
  // }

  if (method_ == kInvalid) {
    LOG_ERROR << "HttpRequest.appendToBuffer : "
              << " invalid request method\n";
  }

  output->append(std::string(methodString()) + " " + path_ + " HTTP/1.1\r\n");

  for (const auto &header : headers_) {
    output->append(header.first);
    output->append(": ");
    output->append(header.second);
    output->append("\r\n");
  }

  output->append("\r\n");
}