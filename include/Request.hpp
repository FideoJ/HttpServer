#ifndef REQUEST_HPP_
#define REQUEST_HPP_
#include <unordered_map>
#include <string>
struct Request {
  enum Method { GET, POST, NO_IMPL };

  Method method;
  std::string path;
  std::unordered_map<std::string, std::string> queryString;
  std::string version;
  std::unordered_map<std::string, std::string> headers;
  int contentLength;
  std::string body;
};

#endif