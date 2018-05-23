#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_
#include <unordered_map>
#include <string>
struct Response {
  std::string version;
  int statusCode;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
  
  void Write(const string &data);
  void End();
};

#endif