#ifndef CXC_COMPILER_HPP
#define CXC_COMPILER_HPP

#include <iterator>
#include <stdexcept>
#include <vector>

#include <cxc/diagnostic.hpp>
#include <cxc/syntax/token.hpp>

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
      .column = 2, // this is 2 for a reason I can't explain
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
      hex_escape_sequence,
      digit_escape_sequence,
      newline_in_string_token,
      unknown_escape_sequence,
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
