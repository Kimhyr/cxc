#include <cctype>
#include <exception>
#include <locale>
#include <compare>
#include <filesystem>
#include <stdexcept>
#include <format>
#include <fstream>
#include <sstream>

#include <cxc/compiler/lexer.hpp>
#include <cxc/utility/character.hpp>
#include <cxc/utility/macros.hpp>
#include <cxc/diagnostic.hpp>

namespace stdfs = std::filesystem;

namespace cxc
{
  void lexer::load(char const* file_path)
  {
    std::ifstream f(file_path);
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
    std::copy(str.begin(), str.end(), const_cast<char*>(m_iterator));
    m_file_path = file_path;
  }

  void lexer::next(token& t)
  {
    while (std::isspace(current())) [[likely]] {
      consume();
    }

    t = {
      .location = {
        .file_path = file_path(),
        .position  = {
          .row = position().row,
          .column = position().column - 1
        }
      }
    };

    token_type ctt = static_cast<token_type>(current());
PUSH_DIAGNOSTIC
IGNORE_DIAGNOSTIC(-Wswitch-enum)
    switch (ctt) {
POP_DIAGNOSTIC
    case token_type::quote:
      consume();
      t.type = token_type::string;
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
            consume();
        } while (std::isdigit(current())
              || std::isalpha(current())
              || current() == '_');

        buf.push_back('\0');
        t.value.identifier = std::string_view{new char[buf.size()], buf.size()};
        std::copy(buf.begin(), buf.end(), const_cast<char*>(t.value.identifier.begin()));
        break;
      }

      throw lexing_error(lexing_error::unknown_token);
    }
  }

  void lexer::consume() noexcept
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
    std::stringstream buf;
    do {
      if (current() == '\\') {
        consume();
        match_escape_sequence(buf);
      } else {
        buf.put(current());
        consume();
      }

      if (current() == '\n') {
        throw lexing_error(lexing_error::newline_in_string_token);
      }
    } while (current() != '"');
    consume();

    std::string_view view = buf.view();
    std::size_t size = view.size();
    t.value.string = std::string_view{
      new char[size], size
    }; 
    std::copy(view.begin(), view.end(), const_cast<char*>(t.value.string.data()));
  }

  void lexer::match_escape_sequence(std::stringstream& buf)
  {
    std::stringstream buf2;
    std::uint64_t n;

    switch (current()) {
    case 'x':
      consume();
      if (std::isxdigit(current())) {
        buf2 << current();
        consume();
        if (std::isxdigit(current())) {
          buf2 << current();
          consume();
        }
      }

      buf2 >> std::hex >> n;
      if (buf2.fail()) {
        throw lexing_error(lexing_error::hex_escape_sequence);
      }
      buf << n;
      return;
    case '\'':
    case '\"':
    case '\\':
      buf.put(current());
      consume();
      return;
    case 'a': buf.put('\a'); break;
    case 'b': buf.put('\b'); break;
    case 'f': buf.put('\f'); break;
    case 'n': buf.put('\n'); break;
    case 'r': buf.put('\r'); break;
    case 't': buf.put('\t'); break;
    case 'v': buf.put('\v'); break;
    default:
      if (isoctal(current())) {
        buf2.put(current());
        consume();
        if (isoctal(current())) {
          buf2.put(current());
          consume();
          if (isoctal(current())) {
            buf2.put(current());
            consume();
          }
        }

        buf2 >> n;
        if (buf2.fail()) {
          throw lexing_error(lexing_error::digit_escape_sequence);
        }
        buf << n;
        return;
      }
      throw lexing_error(lexing_error::unknown_escape_sequence);
    }
    consume();
  }

  void lexer::next_number(token& t)
  {
    std::stringstream buf;
    do {
      buf.put(current());
      consume();

      if (current() == '.') {
        if (t.type == token_type::floating) {
          throw lexing_error(lexing_error::extra_dot_in_floating_token);
        }

        t.type = token_type::floating;
        buf.put(current());
        consume();
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
      case newline_in_string_token:
        m = "new line in string token";
        break;
      case hex_escape_sequence:
        m = "failed to parse a hexadecimal escape sequence in a string token";
        break;
      case digit_escape_sequence:
        m = "failed to parse a digital escape sequence in a string token";
        break;
      case unknown_escape_sequence:
        m = "unknown escape sequence";
        break;
      }
      return m;
  }
}
