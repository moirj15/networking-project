#include "compressor.h"
#include "httplib.h"

#include <codecvt>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string get_path_from_filename(const std::string &filename) {
  auto base = filename.substr(0, filename.find_last_of("."));
  return base + "_files";
}

struct SizedPtr {
  size_t size;
  std::unique_ptr<char[]> data;
};

class Server {
  httplib::Server server_;
  std::unordered_map<std::string, httplib::Server::Handler> get_callbacks_;
  std::unordered_map<std::string, SizedPtr> website_images_;
  SizedPtr html_file_;

public:
  Server(const std::string &filename);

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
};
Server::Server(const std::string &filename) {
  html_file_ = read_file(filename.c_str());
  auto paths = load_file_paths(filename);
  load_images(paths);
  init_callbacks();
}

void Server::load_images(const std::vector<std::string> &filenames) {
  for (const auto &filename : filenames) {
    website_images_[filename] = read_file(filename.c_str());
  }
}

std::vector<std::string> Server::load_file_paths(const std::string &filename) {
  std::vector<std::string> ret;
  auto path = get_path_from_filename(filename);
  std::vector<std::string> resource_paths;
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    ret.push_back(entry.path().string());
    // resource_paths.emplace_back(entry.path());
  }
  return ret;
}

SizedPtr Server::read_file(const char *filename) {
  FILE *fp = nullptr;
  fp = fopen(filename, "rb");
  char *src = nullptr;
  uint64_t len = 0;

  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  rewind(fp);

  if (len == 0) {
    fprintf(stderr, "failed to get file size");
    // exit(EXIT_FAILURE);
  }
  src = new char[len + 1]();
  // src = (char *)calloc(len + 1, sizeof(char));
  len = fread(src, sizeof(char), len, fp);

  fclose(fp);

  return {len, std::unique_ptr<char[]>(src)};
}
void Server::init_callbacks() {
  for (const auto &pair : website_images_) {
    auto filename = pair.first;
    auto &image = pair.second;
    get_callbacks_[filename] = [this, filename](
                                   const httplib::Request &req, httplib::Response &response) {
      // TODO: proeprly set the response type
      response.set_content(
          website_images_[filename].data.get(), website_images_[filename].size, "text/html");
    };
  }
}

char *read_file(const char *filename, size_t *size) {
  FILE *fp = nullptr;
  fp = fopen(filename, "rb");
  char *src = nullptr;
  uint64_t len = 0;

  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  rewind(fp);

  if (len == 0) {
    fprintf(stderr, "failed to get file size");
    // exit(EXIT_FAILURE);
  }

  src = (char *)calloc(len + 1, sizeof(char));
  len = fread(src, sizeof(char), len, fp);
  *size = len;

  fclose(fp);

  return src;
}

int main(int argc, char **argv) {
  Server server("../web-files/Large HTML page with images.html");
  // httplib::Server server;
  // auto path = get_path_from_filename("../web-files/Large HTML page with images.html");
  // std::vector<std::string> resource_paths;
  // for (const auto &entry : std::filesystem::directory_iterator(path)) {
  //  // resource_paths.emplace_back(entry.path());
  //}

  // server.Get("/", [](const httplib::Request &req, httplib::Response &response) {
  //  auto [compressed_html, size_before, size_after] =
  //      compress_html_file("../web-files/Large HTML page with images.html");
  //  std::cout << "Size before compression: " << size_before << " Bytes\n";
  //  std::cout << "Size after compression: " << size_after << " Bytes\n";
  //  // auto compressed_html = compress_html_file("../web-files/hello.html");
  //  // auto compressed_html = compress_html_file("../web-files/hello.html");
  //  response.set_content(compressed_html, "text/html");
  //});

  // server.Get("/Large HTML page with images_files/stadyn_image1.gif",
  //    [](const httplib::Request &req, httplib::Response &response) {
  //      size_t size = 0;
  //      auto *data =
  //          read_file("../web-files/Large HTML page with images_files/stadyn_image1.gif", &size);
  //      response.set_content(data, size, "image/gif");
  //    });
  // server.listen("localhost", 1234);
}
