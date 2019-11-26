#include "compressor.h"

#include <codecvt>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

struct Token {
  std::string data;
  enum Type {
    OpenAngleBracket,
    CloseAngleBracket,
    String,
    WhiteSpace,
  } type;

  //  Token(const char *str, size_t size, Type t) : data(str, size), type(t) {}
  Token(std::string s, Type t) : data(s), type(t) {}
};

constexpr wchar_t SPACE = u8' ';
constexpr wchar_t TAB = u8'\t';
constexpr wchar_t NEW_LINE = u8'\n';
constexpr wchar_t CARRIAGE_RETURN = u8'\r';

static std::string read_utf8_file(const char *filename) {
  std::ifstream wide_file(filename);
  std::stringstream stringstream;
  stringstream << wide_file.rdbuf();
  return stringstream.str();
}

inline static bool whitespace_char(const wchar_t wc) {
  return (wc == SPACE) || (wc == TAB) || (wc == NEW_LINE) || (wc == CARRIAGE_RETURN);
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
  auto contents = read_utf8_file(filename);
  size_t size_before_compression = contents.size();
  auto tokens = make_tokens(contents);
  auto compressed_tokens = remove_whitespace(tokens);
  auto compressed_contents = tokens_to_wstring(compressed_tokens);
  size_t size_after_compression = compressed_contents.size();
  return {compressed_contents, size_before_compression, size_after_compression};
}
