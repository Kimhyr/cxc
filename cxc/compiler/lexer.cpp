#include <exception>
#include <locale>
#include <compare>
#include <filesystem>
#include <stdexcept>
#include <format>
#include <fstream>
#include <sstream>
#include <vector>

#include <cxc/compiler/lexer.h>
#include <cxc/utilities/macros.h>

namespace stdfs = std::filesystem;

namespace cxc
{
  void lexer::load(std::string_view file_path)
  {
    std::ifstream f(file_path.data());
    if (f.fail()) {
      if (!stdfs::exists(file_path)) {
        throw std::invalid_argument(std::format("file path doesn't exist: {}", file_path));
      }
      if (!stdfs::is_regular_file(file_path)) {
        throw std::invalid_argument(std::format("file path doesn't lead to a regular file: {}", file_path));
      }
      throw std::runtime_error(std::format("an unknown failure happend from trying to open the file path: {}", file_path));
    }
    std::stringstream ss;
    ss << f.rdbuf();
    std::string&& str = ss.str();
    this->m_iterator = new char[str.size()];
    std::copy(str.begin(), str.end(), const_cast<char*>(this->m_iterator));
  }

  void lexer::next(token& t)
  {
    while (std::isspace(current())) [[likely]] {
      advance();
    }
    t = {
      .location.file_path  = file_path(),
      .location.span.start = position(),
    };
    token_type ctt = static_cast<token_type>(current());
PUSH_DIAGNOSTIC
IGNORE_DIAGNOSTIC(-Wswitch-enum)
    switch (ctt) {
POP_DIAGNOSTIC
    case token_type::quote:
      next_string(t);      
      break;
    case token_type::period:
      if (std::isdigit(peek(1))) {
        t.type = token_type::floating;
        next_number(t);
        break;
      }
      [[fallthrough]];
    case token_type::plus:
    case token_type::minus:
    case token_type::colon:
    case token_type::left_parenthesis:
    case token_type::right_parenthesis:
    case token_type::left_angle:
    case token_type::right_angle:
    case token_type::comma:
    case token_type::semicolon:
    case token_type::end:
      t.type = static_cast<token_type>(ctt);
      break;
    default:
      if (std::isdigit(current())) {
        t.type = token_type::uinteger;
        next_number(t);
        break;
      }
      if (std::isalpha(current()) || current() == '_') {
        t.type = token_type::identifier;
        std::vector<char> buf;
        do {
            buf.push_back(current());
            advance();
        } while (std::isdigit(current())
              || std::isalpha(current())
              || current() == '_');
        buf.push_back('\0');
        t.value.identifier = new char[buf.size()];
        std::copy(buf.begin(), buf.end(), const_cast<char*>(t.value.identifier));
        break;
      }
      throw lexing_error(lexing_error::unknown_token);
    }
    // I know that this would be +1 of the correct row and column
    t.location.span.end = position();
  }

  void lexer::advance() noexcept
  {
    ++m_iterator;
    if (current() == '\n') {
      ++m_position.row;
      m_position.column = 0;
    }
    ++m_position.column;
  }

  char lexer::peek(std::ptrdiff_t offset) const noexcept
  {
    return iterator()[offset];
  }

  void lexer::next_string(token& t)
  {
    throw;
  }

  void lexer::next_number(token& t)
  {
    std::stringstream buf;
    do {
      buf.put(current());
      advance();
      if (current() == '.') {
        if (t.type == token_type::floating) {
          throw lexing_error(lexing_error::extra_dot_in_floating_token);
        }
        t.type = token_type::floating;
        continue;
      }
    } while (std::isdigit(current()));
    if (t.type == token_type::uinteger) {
      buf >> t.value.uinteger;
      if (buf.fail()) {
        throw lexing_error(lexing_error::floating);
      }
    } else {
      buf >> t.value.floating;
      if (buf.fail()) {
        throw lexing_error(lexing_error::uinteger);
      }
    }
  }

  char const* lexing_error::what() const noexcept
  {
      char const* m;
      switch (type()) {
      case unknown_token:
        m = "unknown token";
        break;
      case extra_dot_in_floating_token:
        m = "extra dot in floating token";
        break;
      case uinteger:
        m = "failed to make a uinteger token";
        break;
      case floating:
        m = "failed to make a floating token";
        break;
      }
      return m;
  }
}
