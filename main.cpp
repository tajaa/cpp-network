#include <httplib.h>
#include <iostream>

int main() {
  httplib::Server svr;

  svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
    res.set_content("<h1>Hello, World!</h1>", "text/html");
  });

  std::cout << "Server starting on http://localhost:8080" << std::endl;
  svr.listen("localhost", 8080);
}
