#ifndef REQUEST_HPP_
#define REQUEST_HPP_
#include <unordered_map>
#include <string>
struct Request {
  enum Method { GET, POST };

  Method method;
  std::string path, queryString;
  std::string version;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
};

#endif