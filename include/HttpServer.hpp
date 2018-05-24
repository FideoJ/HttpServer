#ifndef HTTP_SERVER_HPP_
#define HTTP_SERVER_HPP_
#include "MiddlewareManager.hpp"
#include <string>
#define DEFAULT_PORT 3000
#define DEFAULT_BACKLOG 128

class HttpServer {
public:
  HttpServer(MiddlewareManager *mwMgr, const std::string &ip = {},
             int port = DEFAULT_PORT, int backlog = DEFAULT_BACKLOG);
  void Run();

private:
  static void serve(int clientFd, MiddlewareManager *mwMgr);
  static void buildRequest(int clientFd, Request &req);

private:
  MiddlewareManager *_mwMgr;
  std::string _ip;
  int _port;
  int _backlog;
  int _listenSock;
};

#endif