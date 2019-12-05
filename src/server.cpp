#include "server.h"

#include "compressor.h"

#include <iostream>
#include <cstdio>
#include <filesystem>
Server::Server(const std::string &filename, bool compress) : html_filename_(filename), compress_(compress) {
  if (compress_) {
    auto [file, original_size, compressed_size] = compress_html_file(filename.c_str());
    html_file_ = file;
    metrics_.emplace_back(filename, original_size, compressed_size);
  } else {
    auto file = read_file(filename.c_str());
    html_file_ = {file.data.get(), file.size};
  }
  auto paths = load_file_paths(filename);
  load_data(paths);
  init_callbacks();
}

void Server::print_metrics() {
  printf("filename, original size, compressed size, percent compressed\n");
  size_t total_original = 0;
  size_t total_compressed = 0;
  for (const auto &metric : metrics_) {
    printf("%s, %lld, %lld, %f\n", metric.filename.c_str(), metric.original_size,
        metric.compressed_size, metric.percent_compressed);
    total_original += metric.original_size;
    total_compressed += metric.compressed_size;
  }
  Metrics metric("Total", total_original, total_compressed);
  printf("%s, %lld, %lld, %f\n", metric.filename.c_str(), metric.original_size,
      metric.compressed_size, metric.percent_compressed);
}

void Server::load_data(const std::vector<std::string> &filenames) {
  if (compress_) {
    for (auto filename : filenames) {
      auto filename_without_leading_dots = filename.substr(3);
      auto type =
          filename_without_leading_dots.substr(filename_without_leading_dots.find_first_of('.'), 4);
      if (type == ".svg" || type == ".css" || type == ".php" || type == ".js") {
        website_data_[filename] = read_file(filename.c_str());
        metrics_.emplace_back(filename, website_data_[filename].size, website_data_[filename].size);
      } else {
        auto [file, original_size, compressed_size] = compress_image(filename.c_str());
        metrics_.emplace_back(filename, original_size, compressed_size);
        website_data_[filename] = {compressed_size, std::unique_ptr<char[]>(file)};
      }
    }
  } else {
    for (const auto &filename : filenames) {
      website_data_[filename] = read_file(filename.c_str());
    }
  }
}

std::vector<std::string> Server::load_file_paths(const std::string &filename) {
  std::vector<std::string> ret;
  auto path = get_path_from_filename(filename);
  std::vector<std::string> resource_paths;
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    ret.push_back(entry.path().string());
  }
  return ret;
}

FatPtr Server::read_file(const char *filename) {
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
  for (const auto &pair : website_data_) {
    auto filename = pair.first;
    auto &image = pair.second;
    // auto response_type = get_response_type(filename);
    auto callback = [this, filename](const httplib::Request &req, httplib::Response &response) {
      auto response_type = get_response_type(filename);
      response.set_content(
          website_data_[filename].data.get(), website_data_[filename].size, response_type.c_str());
    };

    auto pattern = filename.substr(filename.find_last_of("/"));
#ifdef _WIN32
    pattern.replace(pattern.find_last_of("\\"), 1, "/");
#else 
    auto p = get_path_from_filename(html_filename_);
    pattern = p.substr(p.find_last_of("/")) +  pattern;
#endif
    std::cout << pattern << '\n';
    server_.Get(pattern.c_str(), callback);
  }
  server_.Get("/", [this](const httplib::Request &req, httplib::Response &response) {
    response.set_content(html_file_.c_str(), html_file_.size(), "text/html");
  });
}
std::string Server::get_response_type(const std::string &filename) {
  auto type = filename.substr(filename.find_last_of(".") + 1);
  if (type == "php" || type == "js") {
    return "text/" + type;
  }
  return "image/" + type;
}
