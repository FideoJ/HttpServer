#include "Middleware.hpp"

Middleware::Middleware() : _next(nullptr) {}

void Middleware::Process(const Request &req, Response &res) {}

void Middleware::Next(const Request &req, Response &res) {
  if (_next)
    _next->Process(req, res);
}