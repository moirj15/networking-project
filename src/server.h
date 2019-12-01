#ifndef SERVER_H
#define SERVER_H

//#define CPPHTTPLIB_ZLIB_SUPPORT 1
#include "httplib.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct FatPtr {
  size_t size;
  std::unique_ptr<char[]> data;
};

struct Metrics {
  std::string filename;
  size_t original_size;
  size_t compressed_size;
  float percent_compressed;
  Metrics(const std::string &f, size_t os, size_t cs) :
      filename(f), original_size(os), compressed_size(cs),
      percent_compressed(((float)(os - cs) / (float)os) * 100.0f) {}
};

class Server {
  httplib::Server server_;
  std::unordered_map<std::string, httplib::Server::Handler> get_callbacks_;
  std::unordered_map<std::string, FatPtr> website_data_;
  std::vector<Metrics> metrics_;
  std::string html_file_;
  bool compress_;

public:
  /**
   * Constructs a server.
   *
   * @param filename: The name of the html file that the server will serve.
   * @param compress: Compresses the html file if True.
   */
  explicit Server(const std::string &filename, bool compress = false);

  /**
   * Causes the server to start listening.
   */
  inline void listen() { server_.listen("localhost", 1234); }

  void print_metrics();

private:
  /**
   * Loads the images with the given filenames.
   *
   * @param filenames: The list of filenames that we'll be loading.
   */
  void load_data(const std::vector<std::string> &filenames);

  /**
   * Loads the file paths for the files contained in the given filename's associated resource
   * folder. ex: the file hello.html will yield the paths in the hello_files/ folder.
   *
   * @param filename: The name of the file that will be used to load the file paths.
   * @return: The list of file paths.
   */
  std::vector<std::string> load_file_paths(const std::string &filename);

  /**
   * Reads the file with the given filename and stores it in a FatPtr.
   *
   * @param filename: The name of the file.
   * @return: The FatPtr containing the file contents.
   */
  FatPtr read_file(const char *filename);

  /**
   * Retrieves the path for the given filename's associated _files folder.
   *
   * @param filename: The name of the file.
   * @return: The _files folder path.
   */
  std::string get_path_from_filename(const std::string &filename) {
    auto base = filename.substr(0, filename.find_last_of("."));
    return base + "_files";
  }

  /**
   * Initializes the httplib::Server callbacks.
   */
  void init_callbacks();

  /**
   * Determines the http response type for the given filename.
   *
   * @param filename: The name of the file.
   * @return: The http response type.
   */
  std::string get_response_type(const std::string &filename);
};

#endif