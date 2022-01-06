// created by cycloidzzz
// an asynchronous http client

#ifndef MUDUO_NET_HTTP_HTTP_CLIENT_H
#define MUDUO_NET_HTTP_HTTP_CLIENT_H

#include "muduo/net/TcpClient.h"

namespace muduo {
namespace net {

class HttpRequest;
class HttpResponse;

class HttpClient : noncopyable {
public:
  HttpClient(EventLoop *loop, const InetAddress &serverAddr,
             const string &nameArg);

  EventLoop *getLoop() { return client_.getLoop(); }

  void onConnection(const TcpConnectionPtr &conn);
  void onMessage(const TcpConnectionPtr &conn, Buffer *buf,
                 Timestamp receiveTime);

  void onRequest(const TcpConnectionPtr &conn, const HttpRequest &request);

  void setHeaderCallback(HeaderCallback callback) {
    headerCallback_ = std::move(callback);
  }

  void setBodyCallback(BodyCallback callback) {
    bodyCallback_ = std::move(callback);
  }

private:
  TcpClient client_;

  // connect callback
  HeaderCallback headerCallback_;
  BodyCallback bodyCallback_;
};

} // namespace net
} // namespace muduo

#endif // ! MUDUO_NET_HTTP_HTTP_CLIENT_H