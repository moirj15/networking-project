#ifndef COMPRESSOR_H
#define COMPRESSOR_H
#include <string>

std::tuple<std::string, size_t, size_t> compress_html_file(const char *filename);

#endif
