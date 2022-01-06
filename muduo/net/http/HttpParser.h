// created by cycloidzzz
// a HTTP response parser, adapted from HttpContext, see @HttpContext.h

#ifndef MUDUO_NET_HTTP_PARSER_H
#define MUDUO_NET_HTTP_PARSER_H

#include "muduo/base/copyable.h"

#include "muduo/net/http/HttpRequest.h"
#include "muduo/net/http/HttpResponse.h"

namespace muduo {
namespace net {

class Buffer;

class HttpParser : public muduo::copyable {
public:
  enum HttpResponseParseState {
    kExpectResponseLine,
    kExpectHeaders,
    kExpectBody,
    kGotAll,
  };

  HttpParser() : state_(kExpectResponseLine) {}

  // default copy-ctor, dtor and assignment are fine

  // return false if any error
  bool parseResponse(Buffer *buf, Timestamp receiveTime);

  bool gotHeaders() const { return state_ == kExpectBody; }

  bool gotAll() const { return state_ == kGotAll; }

  HttpResponseParseState getState() const { return state_; }
  void setState(HttpResponseParseState state) { state_ = state; }

  void reset() {
    state_ = kExpectResponseLine;
    HttpResponse dummy;
    response_.swap(dummy);
  }

  const HttpResponse &response() const { return response_; }

  HttpResponse &response() { return response_; }

private:
  bool processResponseLine(const char *begin, const char *end);

  HttpResponseParseState state_;
  HttpResponse response_;
};

} // namespace net
} // namespace muduo

#endif // ! MUDOU_NET_HTTP_PARSER_H