#ifndef CXC_COMPILER_H
#define CXC_COMPILER_H

#include <iterator>
#include <stdexcept>
#include <vector>

#include <cxc/diagnostic.h>
#include <cxc/syntax/token.h>

namespace cxc
{ 
  class lexer
  {
  public:
    lexer()             noexcept = default;
    lexer(lexer const&) noexcept = default;
    lexer(lexer&&)      noexcept = default;

    lexer& operator=(lexer const&) noexcept = default;
    lexer& operator=(lexer&&) noexcept = default;

    ~lexer() noexcept = default;

    [[nodiscard]] char const*     file_path() const noexcept { return m_file_path; }
    [[nodiscard]] char const*     iterator()  const noexcept { return m_iterator; }
    [[nodiscard]] position const& position()  const noexcept { return m_position; }
    [[nodiscard]] char            current() const noexcept { return *iterator(); }
 
    void load(char const* file_path);
    void next(token& out);

  private:
    char const*     m_file_path{nullptr};
    char const*     m_iterator{nullptr};
    struct position m_position{
      .row = 1,
      .column = 1,
    };

    void consume() noexcept;
    char peek(std::ptrdiff_t offset) const noexcept;
    void next_string(token&);
    void next_number(token&);
    void match_escape_sequence(std::stringstream& buf);
  };

  class lexing_error
    : public std::runtime_error
  {
  public:
    using base = std::runtime_error;

    enum type : std::size_t
    {
      unknown_token,
      extra_dot_in_floating_token,
      floating,
      uinteger,
      newline_in_string_token,
    };

    lexing_error(type type) noexcept
      : base{*reinterpret_cast<char**>(&type)} {}

    [[nodiscard]]
    type type() const noexcept
    {
      return *reinterpret_cast<enum type*>(const_cast<char*>(base::what()));
    }

    [[nodiscard]] char const* what() const noexcept override;
  };
}

namespace std
{
  template<>
  struct iterator_traits<cxc::lexer>
  {
    using iterator_category = forward_iterator_tag;
    using value_type        = char;
    using pointer           = value_type*;
    using reference         = value_type&;
    using difference        = ptrdiff_t;
  }; 
}

#endif
