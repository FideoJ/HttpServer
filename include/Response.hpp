#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_
#include <unordered_map>
#include <string>
class Response {
public:
  enum Status { OK, BAD_REQUEST, INTERNAL_ERROR };

  Response(int clientFd);
  void SetVersion(std::string version);
  void SetStatus(Status status);
  void SetHeader(std::string key, std::string value);
  void WriteHeaders();
  void Write(const std::string &data);
  void End(const std::string &data);
  void End();

private:
  void writeStatus(std::stringstream &stream);

  std::string _version;
  Status _status;
  std::unordered_map<std::string, std::string> _headers;
  std::string _body;
  bool _headerSent;
  int _clientFd;
};

#endif