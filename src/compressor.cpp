#include "compressor.h"

#include <codecvt>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

struct Token {
  std::wstring_view data;
  enum Type {
    OpenAngleBracket,
    CloseAngleBracket,
    String,
    WhiteSpace,
  } type;

  Token(const wchar_t *str, size_t size, Type t) : data(str, size), type(t) {}
};

constexpr wchar_t SPACE = u8' ';
constexpr wchar_t TAB = u8'\t';
constexpr wchar_t NEW_LINE = u8'\n';
constexpr wchar_t CARRIAGE_RETURN = u8'\r';

static std::wstring read_utf8_file(const char *filename) {
  std::wifstream wide_file(filename);
  wide_file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
  std::wstringstream wss;
  wss << wide_file.rdbuf();
  return wss.str();
}

inline static bool whitespace_char(const wchar_t wc) {
  return (wc == SPACE) || (wc == TAB) || (wc == NEW_LINE) || (wc == CARRIAGE_RETURN);
}

static std::vector<Token> make_tokens(const std::wstring &contents) {
  std::vector<Token> tokens;
  for (size_t pos = 0; pos < contents.size(); pos++) {
    if (contents[pos] == u8'<') {
      tokens.emplace_back((wchar_t *)&contents[pos], 1, Token::OpenAngleBracket);
    } else if (contents[pos] == u8'>') {
      tokens.emplace_back((wchar_t *)&contents[pos], 1, Token::CloseAngleBracket);
    } else if (whitespace_char(contents[pos])) {
      tokens.emplace_back((wchar_t *)&contents[pos], 1, Token::WhiteSpace);
    } else {
      size_t start = pos;
      while ((contents[pos] != u8'<') && (contents[pos] != u8'>')
             && (!whitespace_char(contents[pos]))) {
        pos++;
      }
      size_t len = pos - start;
      tokens.emplace_back((wchar_t *)&contents[pos], len, Token::String);
    }
  }
  return tokens;
}

static std::vector<Token> remove_whitespace(const std::vector<Token> &tokens) {
  return {};
}

static std::wstring tokens_to_wstring(const std::vector<Token> &tokens) {
  return {};
}

std::wstring compress_html_file(const char *filename) {
  auto contents = read_utf8_file(filename);
  size_t size_before_compression = contents.size();
  auto tokens = make_tokens(contents);
  auto compressed_tokens = remove_whitespace(tokens);
  auto compressed_contents = tokens_to_wstring(compressed_tokens);
  size_t size_after_compression = compressed_contents.size();
  return compressed_contents;
}
