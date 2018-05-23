#ifndef MIDDLEWARE_HPP_
#define MIDDLEWARE_HPP_
#include "Middleware.hpp"
#include "Request.hpp"
#include "Response.hpp"

class MiddlewareManager {
public:
  void AddMiddleware(Middleware *middleware);
  void Process(const Request &req, Response &res);

private:
  std::list<Middleware *> _middlewares;
};

#endif