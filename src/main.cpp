#include <fstream>

#include <httplib.h>
#include <iostream>
#include <json/json.h>
#include <stdexcept>

void LoadJSON(const char *filename, Json::Value &value) {
  std::ifstream ifs(filename);
  if (!ifs.is_open()) {
    throw std::runtime_error("Failed to open json file: " +
                             std::string(filename));
  }

  // Print file contents
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      std::istreambuf_iterator<char>());
  std::cout << "File contents:\n" << content << std::endl;

  // Reset file stream to beginning
  ifs.clear();
  ifs.seekg(0);

  Json::CharReaderBuilder builder;
  builder["collectComments"] = true;
  std::string errs;
  if (!Json::parseFromStream(builder, ifs, &value, &errs)) {
    throw std::runtime_error("Failed to parse json: " + errs);
  }
}
int main() {
  try {
    // Load the config file
    Json::Value conf;
    std::cout << "Attempting to load config from: ./config.jsonc" << std::endl;
    LoadJSON("./config.jsonc", conf);

    // Print parsed configuration
    std::cout << "Parsed configuration:" << std::endl;
    std::cout << "Server IP: " << conf["server_ip"].asString() << std::endl;
    std::cout << "Server Port: " << conf["server_port"].asInt() << std::endl;
    std::cout << "SSL Cert: " << conf["ssl_cert"].asString() << std::endl;
    std::cout << "SSL Key: " << conf["ssl_key"].asString() << std::endl;

    // Create server using path from config
    std::cout << "Initializing SSL server..." << std::endl;
    httplib::SSLServer svr(conf["ssl_cert"].asString().c_str(),
                           conf["ssl_key"].asString().c_str());

    if (!svr.is_valid()) {
      throw std::runtime_error("Failed to initialize SSL server. Check your "
                               "certificate and key files.");
    }

    // Set up a route
    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
      res.set_content("Hello, World!", "text/plain");
    });

    //stop server 
   

    // Listen to port
    std::cout << "Starting server on " << conf["server_ip"].asString() << ":"
              << conf["server_port"].asInt() << std::endl;
    svr.listen(conf["server_ip"].asString().c_str(),
               conf["server_port"].asInt());

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
