#include "Middleware.hpp"
#include "MiddlewareManager.hpp"
#include "HttpServer.hpp"
#include <iostream>

class Echoer : public Middleware {
  void Process(const Request &req, Response &res) {
    Next(req, res);
    res.SetStatus(Response::Status::OK);
    res.SetHeader("content-type", "text/html");
    res.Write(req.body);
    res.End();
  }
};

class Logger : public Middleware {
  void Process(const Request &req, Response &res) {
    std::cout << "Headers:" << std::endl;
    for (const auto &header : req.headers) {
      std::cout << "  " << header.first << ": " << header.second << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Body:" << std::endl;
    std::cout << "  " << req.body << std::endl;
    Next(req, res);
  }
};

int main() {
  MiddlewareManager mwMgr;
  Echoer echoer;
  Logger logger;
  mwMgr.AddMiddleware(&logger);
  mwMgr.AddMiddleware(&echoer);

  HttpServer server(&mwMgr);
  server.Run();

  return 0;
}