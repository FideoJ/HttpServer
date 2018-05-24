#include "RequestParser.hpp"
#include "Request.hpp"
#define BUF_SIZE 1024
#define METHOD_LEN 16
#define URL_LEN 1024

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

void RequestParser::buildRequest(int clientFd, Request &req) {
  char buf[BUF_SIZE];
  int lineSize;
  
  char url[URL_LEN];
  char *queryString = NULL;
  int cursor = 0;

  lineSize = getLine(clientFd, buf, sizeof(buf));
  req.method = getToken(buf, lineSize, cursor, req);
  
  // i = 0;


  // i = 0;
  // while (isspace(buf[j]) && (j < sizeof(buf)))
  //   j++;
  // while (!isspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf))) {
  //   url[i] = buf[j];
  //   i++;
  //   j++;
  // }
  // url[i] = '\0';

  // queryString = url;
  // while ((*queryString != '?') && (*queryString != '\0'))
  //   queryString++;
  // if (*queryString == '?') {
  //   *queryString = '\0';
  //   queryString++;
  // }
  // // set path and queryString
  // req.path = url;
  // req.queryString = queryString;

}

std::string RequestParser::getToken(const char *line, int lineSize, int &cursor) {
  char token[METHOD_LEN];
  int index;

  while (!isspace(buf[cursor]) && index < sizeof(token) - 1 && cursor < lineSize) {
    token[index] = buf[cursor];
    ++index;
    ++cursor;
  }
  // Not implemented: consider an incomplete token
  token[index] = '\0';
  return std::string(token);
}