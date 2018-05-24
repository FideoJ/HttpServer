#ifndef REQUEST_PARSER_HPP_
#define REQUEST_PARSER_HPP_
#include "Request.hpp"

class RequestParser {
public:
  static void BuildRequest(int clientFd, Request &req);

private:
  static int getLine(int clientFd, char *buf, int size);

};
#endif