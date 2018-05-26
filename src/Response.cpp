#include "Response.hpp"
#include <cstring>
#include <errno.h>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define THROW_RUNTIME_ERROR throw std::runtime_error(strerror(errno));
#define DEFAULT_VERSION "HTTP/1.1"
#define CHUNKED_OPTION true
#define DEFAULT_STATUS Response::Status::OK

Response::Response(int clientFd)
    : _clientFd(clientFd), _headerSent(false), _version(DEFAULT_VERSION),
      _status(DEFAULT_STATUS), _end(false), _chunked(CHUNKED_OPTION) {
  if (_chunked)
    SetHeader("Transfer-Encoding", "chunked");
}

bool Response::SetVersion(std::string version) {
  if (_headerSent)
    return false;
  _version = version;

  // version before HTTP/1.1 does not support "Transfer-Encoding: chunked"
  if (!strcasecmp(_version.c_str(), "HTTP/0.9") ||
      !strcasecmp(_version.c_str(), "HTTP/1.0")) {
    _chunked = false;
    _headers.erase("Transfer-Encoding");
  }
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
  if (data.empty())
    return true;

  if (!_headerSent)
    WriteHeaders();

  // use "Transfer-Encoding: chunked"
  if (_chunked) {
    chunkedWrite(data);
  } else {
    normalWrite(data);
  }
  return true;
}

bool Response::End(const std::string &data) {
  if (_end)
    return false;

  // use "Transfer-Encoding: chunked"
  if (_chunked) {
    chunkedWrite(data);
    chunkedEnd();
  } else {
    // use Content-Length
    if (!_headerSent) {
      SetHeader("Content-Length", std::to_string(data.size()));
      WriteHeaders();
      normalWrite(data);
    } else {
      // use FIN
      normalWrite(data);
      normalEnd();
    }
  }
  _end = true;
  return true;
}

bool Response::End() {
  if (_end)
    return false;

  if (_chunked)
    chunkedEnd();
  else
    normalEnd();
  _end = true;
  return true;
}

void Response::normalWrite(const std::string &data) {
  int nsend = send(_clientFd, data.c_str(), data.size(), 0);
  if (nsend < 0)
    THROW_RUNTIME_ERROR
}

void Response::normalEnd() {
  if (close(_clientFd) < 0)
    THROW_RUNTIME_ERROR
}

void Response::chunkedWrite(const std::string &data) {
  std::stringstream stream;
  stream << std::hex << data.size() << "\r\n";
  stream << data << "\r\n";
  int nsend = send(_clientFd, stream.str().c_str(), stream.str().size(), 0);
  if (nsend < 0)
    THROW_RUNTIME_ERROR
}

void Response::chunkedEnd() {
  int nsend = send(_clientFd, "0\r\n\r\n", 5, 0);
  if (nsend < 0)
    THROW_RUNTIME_ERROR
}