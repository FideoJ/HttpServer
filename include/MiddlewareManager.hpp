#ifndef MIDDLEWARE_HPP_
#define MIDDLEWARE_HPP_
#include "Middleware.hpp"

class MiddlewareManager {
public:
  void addMiddleware(Middleware *middleware);
  
private:
  std::list<Middleware *> _middlewares;
};

#endif