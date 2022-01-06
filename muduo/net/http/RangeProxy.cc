#include "muduo/net/http/RangeProxy.h"

#include "muduo/base/Logging.h"
#include "muduo/net/http/HttpResponse.h"

using namespace muduo;
using namespace muduo::net;

RangeProxy::RangeProxy(EventLoop *loop, const InetAddress &serverAddr,
                       const std::string &nameArg)
    : client_(loop, serverAddr, nameArg), range_({0, 0, 0}) {
  client_.setHeaderCallback(
      std::bind(&RangeProxy::onGetAllHeaders, this, _1, _2));
  client_.setBodyCallback(std::bind(&RangeProxy::onBody, this, _1, _2, _3));
}

void RangeProxy::onGetAllHeaders(const TcpConnectionPtr &conn,
                                 const HttpResponse &response) {
  std::string result = response.getHeader(std::string("Content-Range"));
  if (result != "") {
    LOG_ERROR << "RangeProxy["
              << "name"
              << "] has no Content-Range header\n";
  } else {
    size_t file_size = 0;
    if (range_.end == 0) {
      if (sscanf(result.c_str(), "%*zu-%*zu/%zu", &file_size) < 1) {
        LOG_ERROR << "RangeProxy["
                  << "name"
                  << "] cannot get file size from header\n";
      } else {
        range_.end = file_size;
        // TODO (ozr) : add callback when get the file size.
      }
    }
  }
}

void RangeProxy::onBody(const TcpConnectionPtr &conn, Buffer *buf,
                        Timestamp timestamp) {
  // TODO (ozr) : update bandwidth

  // need to consume the buf
  std::uint64_t remaining = GetRemainBytes();
  std::uint64_t consuming = buf->readableBytes();
  consuming = std::min(remaining, consuming);

  // FIXME : simply retrieve consuming bytes from the buffer
  buf->retrieveAll();

  // TODO (ozr) : add callback when almost complete
  // to reschedule the stream

  // TODO (ozr) : add callback when complete
  // to cancell the stream
  if (remaining == 0) {
    // cancell the stream.
  }
}