#ifndef MIDDLEWARE_MANAGER_HPP_
#define MIDDLEWARE_MANAGER_HPP_
#include "Request.hpp"
#include "Response.hpp"
#include "Middleware.hpp"
#include <list>

class MiddlewareManager {
public:
  void AddMiddleware(Middleware *middleware);
  void Process(const Request &req, Response &res);

private:
  std::list<Middleware *> _middlewares;
};

#endif