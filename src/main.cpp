
#include "server.h"

#include <filesystem>
#include <string>

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: <html-file> <compress:true|false>\n");
    exit(0);
  } else {
    std::string filename = argv[1];
    std::string compress = argv[2];
    Server server(filename, compress == "true");
    server.print_metrics();
    server.listen();
  }

  return 0;
}
