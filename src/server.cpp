#include "server.h"

#include "compressor.h"

#include <cstdio>
#include <filesystem>
Server::Server(const std::string &filename, bool compress_html) {
  if (compress_html) {
    auto [file, origional_size, compressed_size] = compress_html_file(filename.c_str());
    html_file_ = file;
    printf("Original file size: %lld\n", origional_size);
    printf("Compressed file size: %lld\n", compressed_size);
    printf("Size difference: %lld\n", origional_size - compressed_size);
    printf("Difference as percent: %f\n",
        ((float)(origional_size - compressed_size)) / ((float)origional_size));
  } else {
    auto file = read_file(filename.c_str());
    html_file_ = {file.data.get(), file.size};
  }
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
  // TODO: error checking
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
    auto response_type = get_response_type(filename);
    auto callback = [this, filename](const httplib::Request &req, httplib::Response &response) {
      auto response_type = get_response_type(filename);
      response.set_content(website_images_[filename].data.get(), website_images_[filename].size,
          response_type.c_str());
    };

    auto pattern = filename.substr(filename.find_last_of("/"));
    pattern.replace(pattern.find_last_of("\\"), 1, "/");
    server_.Get(pattern.c_str(), callback);
  }
  server_.Get("/", [this](const httplib::Request &req, httplib::Response &response) {
    response.set_content(html_file_.c_str(), html_file_.size(), "text/html");
  });
}

std::string Server::get_response_type(const std::string &filename) {
  return filename.substr(filename.find_last_of(".") + 1);
}
