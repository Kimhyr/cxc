#ifndef CXC_TOKEN_H
#define CXC_TOKEN_H

#include <cstdint>
#include <limits>

#include <cxc/diagnostic.h>
#include <cxc/utilities/enumerated.h>

namespace cxc
{
  struct _token_type
  {
    friend class token_type;
    
  private:
    enum _enum : std::int8_t
    {
      identifier = std::numeric_limits<std::int8_t>::min(),
      string,
      integer,
      uinteger,
      floating,
      forward_arrow,
      minus             = '-',
      colon             = ':',
      left_parenthesis  = '(',
      right_parenthesis = ')',
      left_angle        = '<',
      right_angle       = '>',
      comma             = ',',
      semicolon         = ';',
    };
  };

  class token_type
    : public enumerated<_token_type::_enum>
  {
  public:
    using base = enumerated<_token_type::_enum>;

    constexpr token_type() noexcept = default;
    constexpr token_type(base::type type) noexcept
      : base(type) {}
  };

  void token_type() {
    class token_type tt = token_type::type::;
  }

  union token_value
  {
    char const*   identifier;
    char          chararacter;
    char const*   string;
    std::uint64_t uinteger;
    std::int64_t  integer;
    double        floating;
  };

  struct token
  {
    token_value value;
    token_type  type;
    location    location;
  };
}

#endif