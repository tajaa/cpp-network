#include <filesystem>
#include <fstream>
#include <httplib.h>
#include <inja/inja.hpp>
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

// logic for getting files from other paths
std::string ReadFile(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    return "error: unable to open file: " + path;
  }
  return std::string((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
}

const char *GetContentType(const std::string &path) {
  std::string ext = std::filesystem::path(path).extension().string();
  if (ext == ".html")
    return "text/html";
  if (ext == ".css")
    return "text/css";
  if (ext == ".js")
    return "application/javascript";
  if (ext == ".jpg" || ext == ".jpeg")
    return "image/jpeg";
  if (ext == ".png")
    return "image/png";
  if (ext == ".gif")
    return "image/gif";
  return "application/octet-stream";
}

void ServeFile(const httplib::Request &req, httplib::Response &res) {
  std::string filepath = "../public" + req.path;

  // Check if the path ends with '/'
  if (filepath.back() == '/') {
    filepath += "index.html";
  }
  // If file doesn't exist, try appending .html
  else if (!std::filesystem::exists(filepath) ||
           std::filesystem::is_directory(filepath)) {
    filepath += ".html";
  }

  if (std::filesystem::exists(filepath) &&
      !std::filesystem::is_directory(filepath)) {
    std::string content = ReadFile(filepath);
    if (content.substr(0, 6) != "error:") {
      res.set_content(content.c_str(), content.length(),
                      GetContentType(filepath));
    } else {
      res.status = 404;
      res.set_content("404 Not Found", 13, "text/plain");
    }
  } else {
    res.status = 404;
    res.set_content("404 Not Found", 13, "text/plain");
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

    // catch all route
    svr.Get(".*", ServeFile);

    // stop server
    svr.Get("/stop", [&](const httplib::Request &req, httplib::Response &res) {
      svr.stop();
      res.set_redirect("/");
    });

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
