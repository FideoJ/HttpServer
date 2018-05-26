#include "Response.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdexcept>
#include <cstring>
#include <sstream>
#include <unistd.h>
#define THROW_RUNTIME_ERROR throw std::runtime_error(strerror(errno));
#define DEFAULT_VERSION "HTTP/1.1"
#define DEFAULT_STATUS Response::Status::OK

Response::Response(int clientFd) : _clientFd(clientFd), _headerSent(false),
                                   _version(DEFAULT_VERSION), _status(DEFAULT_STATUS), _end(false) {}

bool Response::SetVersion(std::string version) {
  if (_headerSent)
    return false;
  _version = version;
  return true;
}

bool Response::SetStatus(Response::Status status) {
  if (_headerSent)
    return false;
  _status = status;
  return true;
}

bool Response::SetHeader(std::string key, std::string value) {
  if (_headerSent)
    return false;
  _headers[key] = value;
  return true;
}

bool Response::WriteHeaders() {
  if (_headerSent)
    return false;
  std::stringstream stream;
  stream << _version << " ";
  writeStatus(stream);
  stream << "\r\n";
  for (auto header : _headers) {
    stream << header.first << ": " << header.second << "\r\n";
  }
  stream << "\r\n";

  std::string headers = stream.str();
  int nsend = send(_clientFd, headers.c_str(), headers.size(), 0);
  if (nsend < 0)
    THROW_RUNTIME_ERROR
  _headerSent = true;
  return true;
}

void Response::writeStatus(std::stringstream &stream) {
  switch (_status) {
  case Response::Status::OK:
    stream << "200 OK";
    break;
  case Response::Status::BAD_REQUEST:
    stream << "400 BAD_REQUEST";
    break;
  case Response::Status::INTERNAL_ERROR:
    stream << "500 INTERNAL_ERROR";
    break;
  default:
    stream << "500 INTERNAL_ERROR";
  }
}

bool Response::Write(const std::string &data) {
  if (_end)
    return false;

  if (!_headerSent)
    WriteHeaders();
  int nsend = send(_clientFd, data.c_str(), data.size(), 0);
  if (nsend < 0)
    THROW_RUNTIME_ERROR
  return true;
}

bool Response::End(const std::string &data) {
  if (_end)
    return false;

  int nsend;
  if (_headerSent) {
    nsend = send(_clientFd, data.c_str(), data.size(), 0);
    if (nsend < 0)
      THROW_RUNTIME_ERROR
    if (close(_clientFd) < 0)
      THROW_RUNTIME_ERROR
  } else {
    // persistent connection
    SetHeader("Content-Length", std::to_string(data.size()));
    WriteHeaders();
    nsend = send(_clientFd, data.c_str(), data.size(), 0);
    if (nsend < 0)
      THROW_RUNTIME_ERROR
  }
  _end = true;
  return true;
}

bool Response::End() {
  if (_end)
    return false;

  if (close(_clientFd) < 0)
    THROW_RUNTIME_ERROR
  _end = true;
}
