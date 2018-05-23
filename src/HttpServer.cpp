#include "HttpServer.hpp"
#include "MiddlewareManager.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#define DEFAULT_PORT 3000
#define DEFAULT_BACKLOG 128
#define RUNTIME_ERROR throw std::runtime_error(strerror(errno));

HttpServer::HttpServer(MiddlewareManager *mwMgr, const std::string &ip,
                       int port, int backlog)
    : _mwMgr(mwMgr), _ip(ip), _port(port), _backlog(backlog) {
  if (_ip.emtpy())
    _ip = "0.0.0.0";
}

void HttpServer::Run() {
  _listenSock = socket(AF_INET, SOCK_STREAM, 0);
  if (_listenSock < 0)
    RUNTIME_ERROR

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons((uint16_t)port);
  inet_pton(AF_INET, _ip.c_str(), addr.sin_addr.s_addr);

  if (bind(_listenSock, (const sockaddr *)addr, sizeof(addr)) < 0)
    RUNTIME_ERROR

  if (listen(_listenSock, _backlog) < 0)
    RUNTIME_ERROR

  int clientSock = -1;
  struct sockaddr_in clientAddr;
  int clientAddrLen = sizeof(clientAddr);
  while (1) {
    clientSock = accept(_listenSock, (sockaddr *)clientAddr, clientAddrLen);
    if (clientSock < 0)
      RUNTIME_ERROR
  }
}