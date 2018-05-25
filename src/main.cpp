#include "Middleware.hpp"
#include "MiddlewareManager.hpp"
#include "HttpServer.hpp"
#include <iostream>

class Echoer : public Middleware {
  void Process(const Request &req, Response &res) {
    Next(req, res);
    res.SetHeader("Content-Type", "text/plain");
    res.Write(req.body);
    res.End();
  }
};

class Logger : public Middleware {
  void Process(const Request &req, Response &res) {
    std::cout << req.method << " " << req.path << " " << req.version << std::endl;

    std::cout << "QueryString:" << std::endl;
    for (const auto &queryString : req.queryString) {
      std::cout << "  " << queryString.first << "=" << queryString.second << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Headers:" << std::endl;
    for (const auto &header : req.headers) {
      std::cout << "  " << header.first << ": " << header.second << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Body(" << req.contentLength << "):" << std::endl;
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

  HttpServer server(&mwMgr, "0.0.0.0", 3000);
  server.Run();

  return 0;
}