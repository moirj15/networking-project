#include "compressor.h"

#include <codecvt>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <webp/encode.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Token {
  std::string data;
  enum Type {
    OpenAngleBracket,
    CloseAngleBracket,
    String,
    WhiteSpace,
  } type;

  Token(std::string s, Type t) : data(std::move(s)), type(t) {}
};

std::tuple<char *, size_t> read_file(const char *filename) {
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

  return {src, len};
}

static std::vector<Token> make_tokens(const std::string &contents) {
  std::vector<Token> tokens;
  size_t pos = 0;
  while (pos < contents.size()) {
    if (contents[pos] == '<') {
      //      tokens.emplace_back((char *)&contents[pos], 1, Token::OpenAngleBracket);
      tokens.emplace_back(contents.substr(pos, 1), Token::OpenAngleBracket);
      pos++;
    } else if (contents[pos] == '>') {
      //      tokens.emplace_back((char *)&contents[pos], 1, Token::CloseAngleBracket);
      tokens.emplace_back(contents.substr(pos, 1), Token::CloseAngleBracket);
      pos++;
    } else if (iswspace(contents[pos])) {
      //      tokens.emplace_back((char *)&contents[pos], 1, Token::WhiteSpace);
      tokens.emplace_back(contents.substr(pos, 1), Token::WhiteSpace);
      pos++;
    } else {
      size_t start = pos;
      size_t len = 0;
      while ((contents[pos] != '<') && (contents[pos] != '>') && (!iswspace(contents[pos]))) {
        len++;
        pos++;
      }
      //      tokens.emplace_back((char *)&contents[start], len, Token::String);
      tokens.emplace_back(contents.substr(start, len), Token::String);
    }
  }
  return tokens;
}

static std::vector<Token> remove_whitespace(const std::vector<Token> &tokens) {
  std::vector<Token> ret;
  bool safe_to_trim = false;
  for (size_t i = 0; i < tokens.size(); i++) {
    // std::cout << tokens[i].data.data() << '\n';
    if (i > 0 && i < tokens.size() - 1) {
      if (((tokens[i - 1].type != Token::WhiteSpace) && (tokens[i + 1].type != Token::WhiteSpace)
              && (tokens[i].type == Token::WhiteSpace) && (tokens[i].data[0] != '\n'))
          || tokens[i].type != Token::WhiteSpace) {
        ret.push_back(tokens[i]);
      }
    } else {
      ret.push_back(tokens[i]);
    }
  }
  return ret;
}

static std::string tokens_to_wstring(const std::vector<Token> &tokens) {
  std::string str;
  for (const auto &token : tokens) {
    str += token.data;
  }
  return str;
}

std::tuple<std::string, size_t, size_t> compress_html_file(const char *filename) {
  // auto contents = read_utf8_file(filename);
  auto [contents, size_before_compression] = read_file(filename);
  // size_t size_before_compression = contents.size();
  auto tokens = make_tokens(contents);
  delete[](contents);
  auto compressed_tokens = remove_whitespace(tokens);
  auto compressed_contents = tokens_to_wstring(compressed_tokens);
  size_t size_after_compression = compressed_contents.size();
  return {compressed_contents, size_before_compression, size_after_compression};
}

std::tuple<char *, size_t, size_t> compress_image(const char *filename) {
  int width = 0;
  int height = 0;
  int channels = 0;
  unsigned char *image = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
  unsigned char *compressed;
  size_t compressed_size = WebPEncodeRGBA(image, width, height, width * 4, 25.0f, &compressed);
  // char *compressed_copy = new char[compressed_size]();
  // memcpy(compressed_copy, compressed, compressed_size);
  return {(char *)compressed, width * height, compressed_size};
}