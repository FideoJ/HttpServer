#include "RequestParser.hpp"
#include "Request.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#define BUF_SIZE 1024

int RequestParser::getLine(int clientFd, char *buf, int size) {
  int i = 0;
  char c = '\0';
  int n;

  while ((i < size - 1) && (c != '\n')) {
    n = recv(clientFd, &c, 1, 0);
    if (n > 0) {
      if (c == '\r') {
        n = recv(clientFd, &c, 1, MSG_PEEK);
        if (n > 0 && c == '\n')
          recv(clientFd, &c, 1, 0);
        else
          c = '\n';
      }
      buf[i] = c;
      ++i;
    } else {
      c = '\n';
    }
  }
  buf[i] = '\0';

  return i;
}

void RequestParser::BuildRequest(int clientFd, Request &req) {
  char buf[BUF_SIZE];
  int cursor = 0;

  int lineSize = getLine(clientFd, buf, sizeof(buf));

  std::string method = getToken(buf, lineSize, cursor);
  parseMethod(method, req);

  std::string url = getToken(buf, lineSize, cursor);
  parsePathAndQueryString(url, req);

  std::string version = getToken(buf, lineSize, cursor);
  parseVersion(version, req);
}

std::string RequestParser::getToken(const char *line, int lineSize, int &cursor) {
  char token[BUF_SIZE];
  int index = 0;

  while (isspace(line[cursor]) && cursor < lineSize)
    ++cursor;
  while (!isspace(line[cursor]) && index < sizeof(token) - 1 && cursor < lineSize) {
    token[index] = line[cursor];
    ++index;
    ++cursor;
  }
  // discard linefeed
  if (token[index - 1] == '\n')
    --index;
  // Not implemented: consider a empty or incomplete token
  token[index] = '\0';
  return std::string(token);
}

void RequestParser::parseMethod(const std::string &method, Request &req) {
  if (!strcasecmp(method.c_str(), "GET"))
    req.method = Request::Method::GET;
  else if (!strcasecmp(method.c_str(), "POST"))
    req.method = Request::Method::POST;
  else
    req.method = Request::Method::NO_IMPL;
}

void RequestParser::parsePathAndQueryString(const std::string &url, Request &req) {
  std::size_t qsDel = url.find_first_of('?');
  if (qsDel == std::string::npos) {
    req.path = url;
  } else {
    req.path = url.substr(0, qsDel);
    // Not implemented: consider a stupidly invalid query string
    std::size_t last = qsDel + 1;
    std::size_t kvDel;
    std::size_t pairDel = url.find_first_of('&', last);
    while (pairDel != std::string::npos) {
      kvDel = url.find_first_of('=', last);
      if (kvDel != std::string::npos)
        req.queryString[url.substr(last, kvDel - last)] = url.substr(kvDel + 1, pairDel - (kvDel + 1));
      last = pairDel + 1;
      pairDel = url.find_first_of('&', last);
    }
    kvDel = url.find_first_of('=', last);
    if (kvDel != std::string::npos)
      req.queryString[url.substr(last, kvDel - last)] = url.substr(kvDel + 1);
  }
}

void RequestParser::parseVersion(const std::string &version, Request &req) {
  std::size_t del = version.find_first_of('/');
  req.version = version.substr(del + 1);
}