# HttpServer
## Introduction
- Use 'Express-like' middleware pattern
- A toy for fun
## Example
```
class Echoer : public Middleware {
  void Process(const Request &req, Response &res) {
    Next(req, res);
    res.SetHeader("Content-Type", "text/plain");
    res.Write(req.version);
    res.Write(req.path);
    res.End(req.body);
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

int main(int argc, char *argv[]) {
  MiddlewareManager mwMgr;
  Echoer echoer;
  Logger logger;
  mwMgr.AddMiddleware(&logger);
  mwMgr.AddMiddleware(&echoer);

  int port = argc > 1 ? std::stoi(argv[1]) : DEFAULT_PORT;
  HttpServer server(&mwMgr, "0.0.0.0", port);
  server.Run();

  return 0;
}
```