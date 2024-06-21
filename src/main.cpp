#include <httplib.h>
#include <iostream>

int main() {
  httplib::SSLServer svr("localhost.crt", "localhost.key");

  svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
    res.set_content("Hello, World!", "text/plain");
  });

  std::cout << "Server starting on https://localhost:8080" << std::endl;
  svr.listen("localhost", 8080);
}
