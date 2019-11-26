#ifndef SERVER_H
#define SERVER_H
#define CPPHTTPLIB_ZLIB_SUPPORT 1
#include "httplib.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
struct SizedPtr {
  size_t size;
  std::unique_ptr<char[]> data;
};

class Server {
  httplib::Server server_;
  std::unordered_map<std::string, httplib::Server::Handler> get_callbacks_;
  std::unordered_map<std::string, SizedPtr> website_images_;
  std::string html_file_;

public:
  Server(const std::string &filename, bool compress_html = false);

  inline void listen() { server_.listen("localhost", 1234); }

private:
  void load_images(const std::vector<std::string> &filenames);
  std::vector<std::string> load_file_paths(const std::string &filename);
  SizedPtr read_file(const char *filename);
  std::string get_path_from_filename(const std::string &filename) {
    auto base = filename.substr(0, filename.find_last_of("."));
    return base + "_files";
  }
  void init_callbacks();
  std::string get_response_type(const std::string &filename);
};

#endif