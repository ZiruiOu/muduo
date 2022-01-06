#ifndef MUDUO_SRC_NET_HTTP_RANGE_PROXY_H
#define MUDUO_SRC_NET_HTTP_RANGE_PROXY_H

#include "muduo/base/Types.h"
#include "muduo/net/http/HttpClient.h"

namespace muduo {
namespace net {

class HttpClient;
class RangeProxy : noncopyable {
public:
  RangeProxy(EventLoop *loop, const InetAddress &serverAddr,
             const std::string &nameArg);

  // @brief remaining bytes for the RangeProxy
  std::uint64_t GetRemainBytes() {
    if (range_.end == 0 || range_.start + range_.received >= range_.end) {
      return 0;
    } else {
      return range_.end - range_.start - range_.received;
    }
  }

  void onGetAllHeaders(const TcpConnectionPtr &conn,
                       const HttpResponse &response);

  void onBody(const TcpConnectionPtr &conn, Buffer *buf, Timestamp timestamp);

private:
  HttpClient client_;
  struct Task {
    std::uint64_t start;
    std::uint64_t end;
    std::uint64_t received;
  };
  struct Task range_;

  // TODO (ozr) : bandwidth sampler

  // TODO (ozr) : set header callback for client

  // TODO (ozr) : set body callback for client
};
} // namespace net
} // namespace muduo

#endif // ! MUDUO_SRC_NET_HTTP_RANGE_PROXY_H