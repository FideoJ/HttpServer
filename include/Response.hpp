#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_
#include <string>
#include <unordered_map>
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
  void normalWrite(const std::string &data);
  void normalEnd();
  void chunkedWrite(const std::string &data);
  void chunkedEnd();

  std::string _version;
  Status _status;
  std::unordered_map<std::string, std::string> _headers;
  std::string _body;
  bool _headerSent, _end, _chunked;
  int _clientFd;
};

#endif