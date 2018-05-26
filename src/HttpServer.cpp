#include "HttpServer.hpp"
#include "MiddlewareManager.hpp"
#include "RequestParser.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <netinet/in.h>
#include <thread>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <cctype>
#define DEFAULT_PORT 3000
#define DEFAULT_BACKLOG 128
#define THROW_RUNTIME_ERROR throw std::runtime_error(strerror(errno));

HttpServer::HttpServer(MiddlewareManager *mwMgr, const std::string &ip,
                       int port, int backlog)
    : _mwMgr(mwMgr), _ip(ip), _port(port), _backlog(backlog) {
  if (_ip.empty())
    _ip = "0.0.0.0";
}

void HttpServer::Run() {
  _listenSock = socket(AF_INET, SOCK_STREAM, 0);
  if (_listenSock < 0)
    THROW_RUNTIME_ERROR

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons((uint16_t)_port);
  inet_pton(AF_INET, _ip.c_str(), &addr.sin_addr.s_addr);

  if (bind(_listenSock, (const sockaddr *)&addr, sizeof(addr)) < 0)
    THROW_RUNTIME_ERROR

  if (listen(_listenSock, _backlog) < 0)
    THROW_RUNTIME_ERROR

  int clientFd = -1;
  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  while (1) {
    clientFd = accept(_listenSock, (sockaddr *)&clientAddr, &clientAddrLen);
    if (clientFd < 0)
      THROW_RUNTIME_ERROR
    // unimplemented: thread i/o lock
    std::thread worker(serve, clientFd, _mwMgr);
    worker.detach();
  }
}

void HttpServer::serve(int clientFd, MiddlewareManager *mwMgr) {
    Request req;
    buildRequest(clientFd, req);
    Response res(clientFd);

    mwMgr->Process(req, res);
}

void HttpServer::buildRequest(int clientFd, Request &req) {
  RequestParser::BuildRequest(clientFd, req);
}
