#include "MiddlewareManager.hpp"
#include "Middleware.hpp"

void MiddlewareManager::AddMiddleware(Middleware *middleware) {
  _middlewares.back()->_next = middleware;
  _middlewares.push_back(middleware);
}

void MiddlewareManager::Process(const Request &req, Response &res) {
  if (!_middlewares.empty())
    _middlewares.front()->Process(req, res);
}