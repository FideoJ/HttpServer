#include "RequestParser.hpp"
#include "Request.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <errno.h>
#include <stdexcept>
#include <cstring>
#define THROW_RUNTIME_ERROR throw std::runtime_error(strerror(errno));
#define BUF_SIZE 1024

int RequestParser::getLine(int clientFd, char *buf, int size) {
  int lineSize = 0;
  char ch = '\0';
  int nrecv;

  while ((lineSize < size - 1) && (ch != '\n')) {
    nrecv = recv(clientFd, &ch, 1, 0);
    if (nrecv > 0) {
      if (ch == '\r') {
        nrecv = recv(clientFd, &ch, 1, MSG_PEEK);
        if (nrecv > 0 && ch == '\n')
          recv(clientFd, &ch, 1, 0);
        else
          ch = '\n';
      }
      buf[lineSize] = ch;
      ++lineSize;
    } else {
      ch = '\n';
    }
  }
  buf[lineSize] = '\0';

  return lineSize;
}

void RequestParser::BuildRequest(int clientFd, Request &req) {
  char buf[BUF_SIZE];
  int cursor = 0;

  int lineSize = getLine(clientFd, buf, sizeof(buf));

  std::string method = getToken(buf, lineSize, cursor);
  parseMethod(method, req);

  std::string url = getToken(buf, lineSize, cursor);
  parsePathAndQueryString(url, req);

  req.version = getToken(buf, lineSize, cursor);

  std::string key, value;
  lineSize = getLine(clientFd, buf, sizeof(buf));
  cursor = 0;
  while (lineSize > 0 && buf[0] != '\n') {
    key = getToken(buf, lineSize, cursor);
    value = getToken(buf, lineSize, cursor);
    req.headers[key.substr(0, key.size() - 1)] = value;
    lineSize = getLine(clientFd, buf, sizeof(buf));
    cursor = 0;
  }
  
  int contentLength = 0;
  if (req.headers.find("Content-Length") != req.headers.end())
    contentLength = std::stoi(req.headers["Content-Length"]);
  req.contentLength = contentLength;

  if (contentLength > 0) {
    char *bodyBuf = (char *)malloc(sizeof(char) * contentLength);
    int nrecv = recv(clientFd, bodyBuf, contentLength, 0);
    if (nrecv > 0)
      req.body.append(bodyBuf, contentLength);
    else
      THROW_RUNTIME_ERROR
    free(bodyBuf);
  }
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
