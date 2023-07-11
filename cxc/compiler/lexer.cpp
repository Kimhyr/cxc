#include <cctype>
#include <filesystem>
#include <stdexcept>
#include <format>
#include <fstream>
#include <sstream>

#include <cxc/compiler/lexer.h>

namespace stdfs = std::filesystem;

namespace cxc
{
  void lexer::load(char const* file_path)
  {
    std::ifstream f(file_path);
    if (f.fail()) {
      if (!stdfs::is_regular_file(file_path)) {
        throw std::invalid_argument(std::format("file path doesn't lead to a regular file: {0}", file_path));
      }

      throw std::runtime_error(std::format("an unknown failure happend from trying to open the file path: {0}", file_path));
    }

    std::stringstream ss;
    ss << f.rdbuf();
    std::string&& str = ss.str();
    this->m_iterator = new char[str.size()];
    std::copy(str.begin(), str.end(), const_cast<char*>(this->m_iterator));
  }

  void lexer::next(token& t)
  {
    while (std::isspace(current())) {
      advance();
    }

    t = {
      .location.file_path  = file_path(),
      .location.span.start = position(),
    };

    token_type ctt = static_cast<token_type>(current());
    switch (ctt) {
      case token_type::minus:
        if (static_cast<token_type>(peek(1)) == token_type::right_angle) {
          t.type = token_type::forward_arrow;
          break;
        }
        [[fallthrough]];
      case token_type::colon:
      case token_type::left_parenthesis:
      case token_type::right_parenthesis:
      case token_type::left_angle:
      case token_type::right_angle:
      case token_type::comma:
      case token_type::semicolon:
        t.type = ctt;
        break;
      default:
        break;
    }
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
}
