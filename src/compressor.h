#ifndef COMPRESSOR_H
#define COMPRESSOR_H
#include <string>

/**
 * Compresses the given html file.
 *
 * @param filename: The name of the html file.
 * @return: A tuple containing the compressed html file, the origional size of the html file, and
 * the compressed size of the file.
 */
// TODO: remove the last size_t
std::tuple<std::string, size_t, size_t> compress_html_file(const char *filename);

std::tuple<char *, size_t, size_t> compress_image(const char *filename);

#endif
