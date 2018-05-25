#ifndef REQUEST_PARSER_HPP_
#define REQUEST_PARSER_HPP_
#include "Request.hpp"
#include <string>

class RequestParser {
public:
  static void BuildRequest(int clientFd, Request &req);

private:
  static int getLine(int clientFd, char *buf, int size);
  static std::string getToken(const char *line, int lineSize, int &cursor);
  static void parseMethod(const std::string &method, Request &req);
  static void parsePathAndQueryString(const std::string &url, Request &req);
};
#endif