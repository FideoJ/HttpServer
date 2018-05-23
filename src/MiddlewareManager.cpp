#include "MiddlewareManager.hpp"

void MiddlewareManager::addMiddleware(Middleware *middleware) {
  _middlewares.back()->_next = middleware;
  _middlewares.push_back(middleware);
}