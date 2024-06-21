#include <httplib.h>
#include <iostream>

int main() {
  httplib::Server svr;

  svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
    const char *html = R"(
      <!DOCTYPE html>
      <html lang="en">
      <head>
          <meta charset="UTF-8">
          <meta name="viewport" content="width=device-width, initial-scale=1.0">
          <title>Hello, World!</title>
          <style>
              html, body {
                  height: 100%;
                  margin: 0;
                  padding: 0;
              }
              body {
                  font-family: Arial, sans-serif;
                  display: flex;
                  justify-content: center;
                  align-items: center;
                  background-color: black;
              }
              h1 {
                  color: orange;
                  margin: 0;
              }
          </style>
      </head>
      <body>
          <h1>Hello, World!</h1>
      </body>
      </html>
    )";
    res.set_content(html, "text/html");
  });

  std::cout << "Server starting on http://localhost:8080" << std::endl;
  svr.listen("localhost", 8080);
}
