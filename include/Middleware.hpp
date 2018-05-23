#ifndef MIDDLEWARE_HPP_
#define MIDDLEWARE_HPP_
#include "Request.hpp"
#include "Response.hpp"

class Middleware {
public:
  Middleware();
  virtual void Process(const Request &req, Response &res);
  void Next(const Request &req, Response &res);

private:
  friend class MiddlewareManager;
  Middleware* _next;
};
#endif