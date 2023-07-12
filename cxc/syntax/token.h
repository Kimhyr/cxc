#ifndef CXC_TOKEN_H
#define CXC_TOKEN_H

#include <cstdint>
#include <limits>
#include <format>
#include <type_traits>

#include <cxc/utility/macros.h>
#include <cxc/utility/string.h>
#include <cxc/utility.h>
#include <cxc/diagnostic.h>
#include <utility>

namespace cxc
{
  enum class token_type
    : std::int8_t
  {
    identifier = std::numeric_limits<std::int8_t>::min(),
    string,
    uinteger,
    floating,
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
    forward_arrow,
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

  struct token
  {
    union {
      std::string_view identifier;
      char             chararacter;
      std::string_view string;
      std::uint64_t    uinteger;
      double           floating;
    } value;
    location    location;
    token_type  type;
  };
}

namespace std
{
  template<>
  struct std::formatter<cxc::token_type>
    : formatter<string>
  {
    auto format(cxc::token_type i, format_context& ctx) const
    {
      std::string_view str;
      switch (i) {
      case cxc::token_type::identifier:        str = STRINGIFY(identifier); break;
      case cxc::token_type::string:            str = STRINGIFY(string); break;
      case cxc::token_type::uinteger:          str = STRINGIFY(uinteger); break;
      case cxc::token_type::floating:          str = STRINGIFY(floating); break;
      case cxc::token_type::forward_arrow:     str = STRINGIFY(forward_arrow); break;
      case cxc::token_type::function:          str = STRINGIFY(function); break;
      case cxc::token_type::value:             str = STRINGIFY(value); break;
      case cxc::token_type::u8:                str = STRINGIFY(u8); break;
      case cxc::token_type::u16:               str = STRINGIFY(u16); break;
      case cxc::token_type::u32:               str = STRINGIFY(u32); break;
      case cxc::token_type::u64:               str = STRINGIFY(u64); break;
      case cxc::token_type::i8:                str = STRINGIFY(i8); break;
      case cxc::token_type::i16:               str = STRINGIFY(i16); break;
      case cxc::token_type::i32:               str = STRINGIFY(i32); break;
      case cxc::token_type::i64:               str = STRINGIFY(i64); break;
      case cxc::token_type::f16:               str = STRINGIFY(f16); break;
      case cxc::token_type::f32:               str = STRINGIFY(f32); break;
      case cxc::token_type::f64:               str = STRINGIFY(f64); break;
      case cxc::token_type::end:               str = STRINGIFY(end); break;
      case cxc::token_type::period:            str = STRINGIFY(period); break;
      case cxc::token_type::quote:             str = STRINGIFY(quote); break;
      case cxc::token_type::minus:             str = STRINGIFY(minus); break;
      case cxc::token_type::plus:              str = STRINGIFY(plus); break;
      case cxc::token_type::colon:             str = STRINGIFY(colon); break;
      case cxc::token_type::left_parenthesis:  str = STRINGIFY(left_parenthesis); break;
      case cxc::token_type::right_parenthesis: str = STRINGIFY(right_parenthesis); break;
      case cxc::token_type::left_angle:        str = STRINGIFY(left_angle); break;
      case cxc::token_type::right_angle:       str = STRINGIFY(right_angle); break;
      case cxc::token_type::comma:             str = STRINGIFY(comma); break;
      case cxc::token_type::semicolon:         str = STRINGIFY(semicolon); break;
      }
      return std::formatter<string>::format(std::format("{}", str), ctx);
    }
  };

  template<>
  struct std::formatter<cxc::token>
    : formatter<string>
  {
    template<typename Ctx>
    auto format(cxc::token const& t, Ctx& ctx) const
    {
      string value;
PUSH_DIAGNOSTIC
IGNORE_DIAGNOSTIC(-Wswitch-enum)
      switch (t.type) {
POP_DIAGNOSTIC
      case cxc::token_type::identifier:    value = std::format("{}"    , t.value.identifier); break;
      case cxc::token_type::string:        value = std::format("\"{}\"", t.value.string); break;
      case cxc::token_type::uinteger:      value = std::format("{}"    , t.value.uinteger); break;
      case cxc::token_type::floating:      value = std::format("{}"    , t.value.floating); break;
      case cxc::token_type::forward_arrow: value = "->"; break;
      default:
          if (to_underlying(t.type) > 0) {
            value = to_underlying(t.type);
          } else {
            value = std::format("{}", t.type);
          }
      }
      string format = std::format("token {{ type = {}, value = {}, location = {} }}", t.type, value, t.location);
      return formatter<string>::format(format, ctx);
    }
  };
}

#endif
