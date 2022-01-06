

#include "muduo/net/http/HttpParser.h"
#include "muduo/net/Buffer.h"
#include <iostream>

using namespace muduo;
using namespace muduo::net;

bool HttpParser::processResponseLine(const char *begin, const char *end) {
  bool succeed = true;
  // const char *start = begin;
  //  const char *space = std::find(start, end, ' ');
  //   http response line : version statusCode reason
  std::string response_line(begin, end);
  char version[500], status[500], reason[500];
  sscanf(response_line.c_str(), "%s %s %s", version, status, reason);
  response_.setStatusCode(HttpResponse::k200Ok);
  response_.setStatusMessage(reason);
  return succeed;
}

// return false if any error
bool HttpParser::parseResponse(Buffer *buf, Timestamp receiveTime) {
  bool ok = true;
  bool hasMore = true;
  while (hasMore) {
    if (state_ == kExpectResponseLine) {
      // parsing the Http response line
      const char *crlf = buf->findCRLF();
      if (crlf) {
        ok = processResponseLine(buf->peek(), crlf);
        if (ok) {
          // request_.setReceiveTime(receiveTime);
          buf->retrieveUntil(crlf + 2);
          state_ = kExpectHeaders;
        } else {
          hasMore = false;
        }
      } else {
        hasMore = false;
      }
    } else if (state_ == kExpectHeaders) {
      const char *crlf = buf->findCRLF();
      if (crlf) {
        const char *colon = std::find(buf->peek(), crlf, ':');
        if (colon != crlf) {
          string field(buf->peek(), colon);
          ++colon;
          while (colon < crlf && isspace(*colon)) {
            ++colon;
          }
          string value(colon, crlf);
          while (!value.empty() && isspace(value[value.size() - 1])) {
            value.resize(value.size() - 1);
          }
          response_.addHeader(field, value);
        } else {
          // empty line, end of header
          // FIXME:
          state_ = kGotAll;
          hasMore = false;
        }
        buf->retrieveUntil(crlf + 2);
      } else {
        hasMore = false;
      }
    } else if (state_ == kExpectBody) {
      // FIXME:
      return ok;
    }
  }
  return ok;
}
