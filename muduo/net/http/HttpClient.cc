#include "muduo/net/http/HttpClient.h"

#include "muduo/base/Logging.h"
#include "muduo/net/http/HttpParser.h"

using namespace muduo;
using namespace muduo::net;

//  ConnectionCallback connectionCallback_;
// MessageCallback messageCallback_;
// WriteCompleteCallback writeCompleteCallback_;

HttpClient::HttpClient(EventLoop *loop, const InetAddress &serverAddr,
                       const string &nameArg)
    : client_(loop, serverAddr, nameArg) {
  // set callbacks for TCP client
  client_.setConnectionCallback(std::bind(&HttpClient::onConnection, this, _1));
  client_.setMessageCallback(
      std::bind(&HttpClient::onMessage, this, _1, _2, _3));
}

void HttpClient::onConnection(const TcpConnectionPtr &conn) {
  if (conn->connected()) {
    conn->setContext(HttpParser());
    // TODO(ozr) : send request headers
    // HttpRequest request
    // onRequest(conn, request)
  }
}

void HttpClient::onMessage(const TcpConnectionPtr &conn, Buffer *buf,
                           Timestamp timestamp) {
  HttpParser *parser = boost::any_cast<HttpParser>(conn->getMutableContext());

  // if (!parser->parseResponse(buf, timestamp)) {
  //  nothing happens
  //}

  // already parse all headers
  // if (parser->gotAll()) {
  // onRequest(conn, parser->request());
  // parser->reset();
  //}
  switch (parser->getState()) {
  case HttpParser::kExpectResponseLine:
  case HttpParser::kExpectHeaders:
    parser->parseResponse(buf, timestamp);
    break;
  case HttpParser::kGotAll:
    // TODO (ozr) : add header callback function.
    if (headerCallback_) {
      headerCallback_(conn, parser->response());
    }
    parser->setState(HttpParser::kExpectBody);
    break;
  case HttpParser::kExpectBody:
    // TODO (ozr) : add body callback function.
    if (bodyCallback_) {
      bodyCallback_(conn, buf, timestamp);
    }
    break;
  default:
    LOG_ERROR << "HttpClient[" << client_.name()
              << "] onMessage Callback : no such HttpParser state";
    break;
  }
}

void HttpClient::onRequest(const TcpConnectionPtr &conn,
                           const HttpRequest &request) {
  Buffer buf;
  request.appendToBuffer(&buf);
  conn->send(&buf);
}