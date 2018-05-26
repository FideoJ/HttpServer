#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_
#include <unordered_map>
#include <string>
class Response {
public:
  enum Status { OK, BAD_REQUEST, INTERNAL_ERROR };

  Response(int clientFd);
  bool SetVersion(std::string version);
  bool SetStatus(Status status);
  bool SetHeader(std::string key, std::string value);
  bool WriteHeaders();
  bool Write(const std::string &data);
  bool End(const std::string &data);
  bool End();

private:
  void writeStatus(std::stringstream &stream);

  std::string _version;
  Status _status;
  std::unordered_map<std::string, std::string> _headers;
  std::string _body;
  bool _headerSent, _end;
  int _clientFd;
};

#endif