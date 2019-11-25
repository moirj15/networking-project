#include "httplib.h"

#include <codecvt>
#include <fstream>
#include <sstream>
#include <string>

int main(int argc, char **argv) {
  httplib::Server server;

  server.Get("/", [](const httplib::Request &req, httplib::Response &response) {
    std::ifstream wide_file("../../web-files/DOS Interrupts.html");
    //    wide_file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
    std::stringstream wss;
    wss << wide_file.rdbuf();
    //    std::string str(wss.str().begin(), wss.str().end());
    response.set_content(wss.str(), "text/html");
  });

  server.listen("localhost", 1234);
}
