
#include "server.h"

#include <filesystem>
#include <string>

int main(int argc, char **argv) {
  Server server("../web-files/Large HTML page with images.html", true);
  server.listen();
}
