#ifndef CXC_TOKEN_H
#define CXC_TOKEN_H

#include <cstdint>
#include <limits>
#include <format>

#include <cxc/diagnostic.h>

namespace cxc
{
  enum class token_type
    : std::int8_t
  {
    identifier = std::numeric_limits<std::int8_t>::min(),
    string,
    uinteger,
    floating,
    forward_arrow,
    function, 
    value,
    u8,
    u16,
    u32,
    u64,
    i8,
    i16,
    i32,
    i64,
    f16,
    f32,
    f64,
    end               = '\0',
    period            = '.',
    quote             = '"',
    minus             = '-',
    plus              = '+',
    colon             = ':',
    left_parenthesis  = '(',
    right_parenthesis = ')',
    left_angle        = '<',
    right_angle       = '>',
    comma             = ',',
    semicolon         = ';',
  };

  union token_value
  {
    char const*   identifier;
    char          chararacter;
    char const*   string;
    std::uint64_t uinteger;
    double        floating;
  };

  struct token
  {
    token_value value;
    token_type  type;
    location    location;
  };
}

template<>
struct std::formatter<cxc::token_type>
{
  template<typename T>
  auto parse(T& ctx)
  {
    return ctx.end();
  }

  template<typename T>
  auto format(cxc::token_type i, T& ctx)
  {
    char const* f;
    switch (i) {
    case cxc::token_type::identifier: f = "identifier"; break;
    case cxc::token_type::string: f = "string"; break;
    case cxc::token_type::uinteger: f = "uinteger"; break;
    case cxc::token_type::floating: f = "floating"; break;
    case cxc::token_type::forward_arrow: f = "->"; break;
    case cxc::token_type::function: f = "function"; break;
    case cxc::token_type::value: f = "value"; break;
    case cxc::token_type::u8: f = "u8"; break;
    case cxc::token_type::u16: f = "u16"; break;
    case cxc::token_type::u32: f = "u32"; break;
    case cxc::token_type::u64: f = "u64"; break;
    case cxc::token_type::i8: f = "i8"; break;
    case cxc::token_type::i16: f = "i16"; break;
    case cxc::token_type::i32: f = "i32"; break;
    case cxc::token_type::i64: f = "i64"; break;
    case cxc::token_type::f16: f = "f16"; break;
    case cxc::token_type::f32: f = "f32"; break;     
    case cxc::token_type::f64: f = "f64"; break;
    default:
      f = {static_cast<char>(i), '\0'};
      break;
    }
    return std::format_to(ctx.out(), "{}");
  }
};

#endif