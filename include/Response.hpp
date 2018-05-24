#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_
#include <unordered_map>
#include <string>
struct Response {
  std::string version;
  int statusCode;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
  
  enum Status { OK, BAD_REQUEST, INTERNAL_ERROR };

  void SetStatus(Status status);
  void SetHeader(std::string key, std::string value);
  void Write(const std::string &data);
  void End();
};

#endif