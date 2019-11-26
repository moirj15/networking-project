
#include "server.h"

#include <filesystem>
#include <string>

std::string get_path_from_filename(const std::string &filename) {
  auto base = filename.substr(0, filename.find_last_of("."));
  return base + "_files";
}

int main(int argc, char **argv) {
  Server server("../web-files/big wikipedia page.html", true);
  server.listen();
}
